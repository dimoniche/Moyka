#include <includes.h>
#include "app_serv.h"
#include "coin.h"
#include "time.h"
#include "fiscal.h"
#include "menu.h"
#include "data.h"
#include "mode.h"
#include "menudesc.h"
#include "datadesc.h"
#include "menu.h"
#include "journal.h"
#include "fr.h"
#include "CRC16.h"
#include "host_app.h"
#include "console.h"
#include "keyboard.h"

// ���� ���������� ���� ������, ����� ��������� ������ �� ������ F1
//#define _DEBUG_MONEY

CPU_INT32U SystemTime;
CPU_INT32U money_timestamp[COUNT_POST + COUNT_VACUUM];

//CPU_INT08U ChannelsState[COUNT_POST + COUNT_VACUUM];
  #define CHANNEL_STATE_FREE            0
  #define CHANNEL_STATE_BUSY            1
  #define CHANNEL_STATE_DISABLED        2
//CPU_INT32U ChannelsCounters[COUNT_POST + COUNT_VACUUM];
CPU_INT32U ChannelsPayedTime[COUNT_POST + COUNT_VACUUM];

#define USER_QUERY_LEN  512

OS_STK    UserTaskStk[USER_TASK_STK_SIZE];
OS_EVENT *UserQuery = NULL;
void     *UserTbl[USER_QUERY_LEN];

int GetUserEvent(int* event);
void UserPrintMoneyMenu(int post);
void WorkServer(void);
void UserPrintThanksMenu(int post);
void UserPrintFirstMenu(void);
void UserPrintErrorMenu(void);
CPU_INT32U GetChannelsTimeForFree(CPU_INT08U ch);

void LoadAcceptedMoney(void);
void SetAcceptedMoney(CPU_INT32U money,int post);
void ClearAcceptedMoney(int post);
CPU_INT32U GetAcceptedMoney(int post);

void SetAcceptedBankMoney(CPU_INT32U money,int post);
void ClearAcceptedBankMoney(int post);
CPU_INT32U GetAcceptedBankMoney(int post);

void InitPass(void);
int CheckChannelEnabled(CPU_INT08U channel);
int ChannelBusy(CPU_INT08U ch);
void UserPrintIpDeviceErrMenu(CPU_INT08U post);
void UserPrintPrintBillMenu(int post);

#ifdef BOARD_CENTRAL_CFG
static char incassation;
static char was_critical_error;
#endif

#ifdef BOARD_POST_CFG

static int out_pulse_count = 0;
static int out_pulse_len = 0;

void AddOutPulses(int count, int len_ms)
{
    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr = 0;
    #endif
    OS_ENTER_CRITICAL();
    
    out_pulse_count += count;
    out_pulse_len = len_ms;

    OS_EXIT_CRITICAL();
}

#endif

static int currentPosition = 0;

typedef enum {
  
  waitMoney = 0,
  insertMoney,
  washing,
  printCheck
    
} washStateEnum;

static washStateEnum wash_State[COUNT_POST + COUNT_VACUUM] = {waitMoney, waitMoney, waitMoney, waitMoney, waitMoney, waitMoney, waitMoney, waitMoney};

static int countSecWait[COUNT_POST + COUNT_VACUUM] = {0, 0, 0, 0, 0, 0, 0, 0};
static CPU_INT32U enable_coin[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U cash_enable[COUNT_POST];
static CPU_INT32U bank_enable[COUNT_POST];
static CPU_INT32U enable_signal[COUNT_POST];
static CPU_INT32U fiscal_enable;

// ���� ��� ���� ������ ���� ����� ������ ������ �����������
static CPU_INT16U time_out_print_check[COUNT_POST + COUNT_VACUUM] = {0, 0, 0, 0, 0, 0, 0, 0};

void DrawMenu(void)
{
  if (GetMode() != MODE_WORK) return;
    
  if((SystemTime%2))
  {
    if(currentPosition >= COUNT_POST + COUNT_VACUUM)
    {
      currentPosition = 0;
    }

    for( ;currentPosition < COUNT_POST + COUNT_VACUUM; currentPosition++)
    {
      if((enable_coin[currentPosition] 
         || cash_enable[currentPosition] 
           || bank_enable[currentPosition]
              || enable_signal[currentPosition])
                && currentPosition < COUNT_POST) break;
      else if((enable_coin[currentPosition])
              && currentPosition >= COUNT_POST) break;
    }
    
    if(currentPosition >= COUNT_POST + COUNT_VACUUM)
    {
      currentPosition = 0;
      return;
    }

    UserPrintMoneyMenu(currentPosition);
    currentPosition++;
  }
}

/*
 ������ ��������� ������� ������������
*/
void UserAppTask(void *p_arg)
{
  CPU_INT32U print_timeout;
  int event;
  
#ifdef BOARD_CENTRAL_CFG
  
  static CPU_INT08U fr_conn_ctr = 0;
    
  incassation = 0;
  was_critical_error = 0;
#endif

  UserPrintFirstMenu();

  int testMoney = 0;

  while (1)
    {
      if (GetUserEvent(&event))
        {
          switch (event){
#ifdef BOARD_CENTRAL_CFG
            case EVENT_UPATE_RTC:
              {
                // ��������� ������� �����
                CPU_INT32U time = GetTimeSec();

                if(SystemTime != time)
                {
                  SystemTime = time;
                  PostUserEvent(EVENT_SEC);
                }
                
                // �������� ������
                CheckMode();

              }
            break;
            case EVENT_SEC:
            {
              CPU_INT32U accmoney = 0;

              // ������� ������ - ��������, ��������� � �.�.
              WorkServer();

              GetData(&EnableFiscalDesc, &fiscal_enable, 0, DATA_FLAG_SYSTEM_INDEX);

              if(!fiscal_enable)
              {
                  FiscalConnState = FISCAL_NOCONN;
              }

              // �������� ����������, ������ ��� ���������
              if ((++fr_conn_ctr % 5) == 0)
              {
                  if (ConnectFiscalFast() == 0)
                  {
                      CheckFiscalStatus();
                  }
              }

              // ������ ������
              ErrorServer();
                
              // ������ ������ � ������� ������
//              if (GetMode() != MODE_WORK)
//              {
//                  break;
//              }

              for(int post = 0; post < COUNT_POST + COUNT_VACUUM; post++)
              {
                GetData(&EnableCoinDesc, &enable_coin[post], post, DATA_FLAG_DIRECT_INDEX);  
                
                if(post < COUNT_POST)
                {
                  GetData(&EnableValidatorDesc, &cash_enable[post], post, DATA_FLAG_DIRECT_INDEX);
                  GetData(&EnableBankDesc, &bank_enable[post], post, DATA_FLAG_DIRECT_INDEX);
                  GetData(&EnableSignalDesc, &enable_signal[post], post, DATA_FLAG_DIRECT_INDEX);
                }

                accmoney = GetAcceptedMoney(post);
                accmoney += GetAcceptedBankMoney(post);

                if (accmoney > 0 && !was_critical_error)
                {
                    CPU_INT32U tick = labs(OSTimeGet() - money_timestamp[post]);
                    
                    // ���� ������ � ��� ������
                    if(wash_State[post] != washing)
                    {
                      // ���� ����� ���� ��� ������ ����� � �� ������ - ��������� ����� - ������ ��� ������ �����
                      GetData(&SignalTimeOutDesc, &print_timeout, 0, DATA_FLAG_SYSTEM_INDEX);
                      if((print_timeout > 0) && (tick > print_timeout * 1000L) && (post < COUNT_POST))
                      {
                          PostUserEvent(EVENT_STOP_MONEY_POST1 + post); // �������� �����

                          // ������� ������� - � ���������� ������
                          continue;
                      }

                      // ��������� ���� ��� �������� ������ �����
                      if (tick > 300000UL)
                      {
                          countSecWait[post] = 0;
                          PostUserEvent(EVENT_CASH_PRINT_CHECK_POST1 + post);
                            
                          // ������� ������� - � ���������� ������
                          continue;
                      }
                    }
                    else
                    {
                      // � ������ ����� ���� ���� ��� � ����������� �� ����� � ���� ������ �����
                      GetData(&CashPerMinuteDesc, &print_timeout, 0, DATA_FLAG_SYSTEM_INDEX);
                      if(print_timeout)
                      {
                        // ���� ������� ��������� ������ - �� ���������� ������� ����� ������������� ��������� ������ ����
                        if (tick > ((60000UL * accmoney) / print_timeout))
                        {
                            countSecWait[post] = 0;
                            PostUserEvent(EVENT_CASH_PRINT_CHECK_POST1 + post);
                              
                            // ������� ������� - � ���������� ������
                            continue;
                        }
                      }
                      else
                      {
                          // ���� ��� ���� ������ - ��� � ���� ���� �����
                          money_timestamp[post] = OSTimeGet();
                      }
                    }
                }
                
                if(countSecWait[post])
                {
                  countSecWait[post]--;

                  // ������ ����� ������ ����, �� �� ������ ���� ������ - �������� ���
                  if(!countSecWait[post] && !was_critical_error) PostUserEvent(EVENT_CASH_PRINT_CHECK_POST1 + post);
                  // ������ ����� ������ ���� - ���� ������
                  else if(!countSecWait[post]) wash_State[post] = waitMoney;
                }
              }

              // ��������������� ��� ��������� ��� ������� ��������� ������ ����� - ������ �� ���� ����
              for(int post = COUNT_POST; post < COUNT_POST + COUNT_VACUUM; post++)
              {
                  accmoney = GetAcceptedMoney(post);
                  
                  if (accmoney > 0 && !was_critical_error)
                  {   // ���� ������ � ��� ������
                      // ��� ���������������� ��������� - ������ ���� �� ��������
                      GetData(&CoinTimeOutDesc, &print_timeout, post, DATA_FLAG_DIRECT_INDEX);
                   
                      if (labs(OSTimeGet() - money_timestamp[post]) > 1000UL * print_timeout)
                      {
                          PostUserEvent(EVENT_CASH_PRINT_CHECK_POST1 + post);
                      }
                  }
              }

              // ��������� ������
              DrawMenu();

              // ���� ���� ������, �� ��������
              if (TstCriticalErrors()) 
              {
                UserPrintErrorMenu(); 
                RefreshMenu();

                // �������� ����� �����, ������ ����������� - �� ���� �� �������� - �����
                if (was_critical_error == 0)
                {
                    for(int post = 0; post < COUNT_POST + COUNT_VACUUM; post++)
                    {
                          // ������� ����������, ���� ���������� � ������ ������ ���� ��� ��� ��������
                          // ��� ���� ����� ����� ������ ������ �����������
                          if(wash_State[post] == printCheck)
                          { 
                              wash_State[post] = waitMoney;
                              countSecWait[post] = 0;
                          }
                          
                          time_out_print_check[post] = 0;
                    }

                    was_critical_error = 1;
                }
                break;
              }
              
              // ������� ������ ����� �����, ���� ���� ������
              if (was_critical_error)
              {
                  was_critical_error = 0;
                  
                  // ������ ��������� - ���������� ��� � ������, ������� ���������� �� ����� ��������� ������, 
                  // ���� ������� ����
                  for(int post = 0; post < COUNT_POST + COUNT_VACUUM; post++)
                  {
                      accmoney = GetAcceptedMoney(post);
                      accmoney += GetAcceptedBankMoney(post);
                      
                      // ������ �� ������ ������ ������� ��������� � ������ ����  � ���� ��� ��� ��������
                      if(wash_State[post] == printCheck) 
                      { 
                          wash_State[post] = waitMoney;
                          countSecWait[post] = 0;
                      }

                      if (accmoney > 0)
                      {
                         // ������������ ���� ���� - ��� �� �� ���� ���� �� ������� �� �������� �����������
                         money_timestamp[post] = OSTimeGet();
                         
                         // ������� ������� ������ ���� - ������� �� ������
                         time_out_print_check[post] = 35;

                         // ����� ������ ������ ���� �������� �� ����� - � ����� ���� ��� ������, ����� �� ��� �����������?
                         PostUserEvent(EVENT_WAIT_CASH_PRINT_CHECK_POST1 + post);
                      }
                  }
                  
                  break;
              }
              
              // ���� ��� �������� ������ ����� ������ ����������� - 35 ������
              for(int post = 0; post < COUNT_POST + COUNT_VACUUM; post++)
              {
                  if(time_out_print_check[post])
                  {
                      time_out_print_check[post]--;

                      if(!time_out_print_check[post] && wash_State[post] == printCheck)
                      {
                          wash_State[post] = waitMoney;
                          countSecWait[post] = 0;
                      }
                  }
              }
            }
            break;
              
            case EVENT_MODE_CHANGE:
              ReInitMenu();
              SaveEventRecord(0, JOURNAL_EVENT_CHANGE_MODE, GetMode());
              break;

            case EVENT_COIN_INSERTED_POST1:
            case EVENT_COIN_INSERTED_POST2:
            case EVENT_COIN_INSERTED_POST3:
            case EVENT_COIN_INSERTED_POST4:
            case EVENT_COIN_INSERTED_POST5:
            case EVENT_COIN_INSERTED_POST6:
            case EVENT_COIN_INSERTED_VACUUM1:
            case EVENT_COIN_INSERTED_VACUUM2:
              {
                CPU_INT32U cpp = 1;
                CPU_INT32U money, accmoney = 0;
                int number_post = event - EVENT_COIN_INSERTED_POST1;
                
                if(wash_State[number_post] != waitMoney && wash_State[number_post] != insertMoney) break;
                if(!enable_coin[number_post]) break;

                GetData(&CoinPerPulseDesc, &cpp, number_post, DATA_FLAG_DIRECT_INDEX);
                
                money = cpp * GetResetCoinCount(number_post) + testMoney;

                accmoney = GetAcceptedMoney(number_post);
                accmoney += money;

                SetAcceptedMoney(accmoney, number_post);
                money_timestamp[number_post] = OSTimeGet();
                                
                if (money) SaveEventRecord(number_post, JOURNAL_EVENT_MONEY_COIN_POST1 + number_post, money);
                
                wash_State[number_post] = insertMoney;
                testMoney = 0;
              }
              break;
            case EVENT_CASH_INSERTED_POST1:
            case EVENT_CASH_INSERTED_POST2:
            case EVENT_CASH_INSERTED_POST3:
            case EVENT_CASH_INSERTED_POST4:
            case EVENT_CASH_INSERTED_POST5:
            case EVENT_CASH_INSERTED_POST6:
              {
                CPU_INT32U cpp = 1;
                CPU_INT32U money, accmoney = 0;
                int number_post = event - EVENT_CASH_INSERTED_POST1;
                
                if(wash_State[number_post] != waitMoney && wash_State[number_post] != insertMoney) break;
                if(!cash_enable[number_post]) break;

                GetData(&CashPerPulseDesc, &cpp, number_post, DATA_FLAG_DIRECT_INDEX);
                
                money = cpp * GetResetCashCount(number_post) + testMoney;
                
                accmoney = GetAcceptedMoney(number_post);
                accmoney += money;
                SetAcceptedMoney(accmoney, number_post);
                money_timestamp[number_post] = OSTimeGet();
                                
                if (money) SaveEventRecord(number_post, JOURNAL_EVENT_MONEY_NOTE_POST1 + number_post, money);
                
                wash_State[number_post] = insertMoney;
                testMoney = 0;
              }
              break;
             
            case EVENT_BANK_INSERTED_POST1:
            case EVENT_BANK_INSERTED_POST2:
            case EVENT_BANK_INSERTED_POST3:
            case EVENT_BANK_INSERTED_POST4:
            case EVENT_BANK_INSERTED_POST5:
            case EVENT_BANK_INSERTED_POST6:
              {
                CPU_INT32U cpp = 1;
                CPU_INT32U money, accmoney = 0;
                int number_post = event - EVENT_BANK_INSERTED_POST1;
                
                if(wash_State[number_post] != waitMoney && wash_State[number_post] != insertMoney) break;
                if(!bank_enable[number_post]) break;

                GetData(&BankPerPulseDesc, &cpp, number_post, DATA_FLAG_DIRECT_INDEX);
                
                money = cpp * GetResetbankCount(number_post) + testMoney;
                
                accmoney = GetAcceptedBankMoney(number_post);
                accmoney += money;
                SetAcceptedBankMoney(accmoney, number_post);
                money_timestamp[number_post] = OSTimeGet();
                                
                if (money) SaveEventRecord(number_post, JOURNAL_EVENT_MONEY_BANK_POST1 + number_post, money);
                
                wash_State[number_post] = insertMoney;
                testMoney = 0;
              }
              break;

            case EVENT_KEY_CANSEL:
              break;
              
            case EVENT_KEY_START:
            case EVENT_KEY_USER_START:
              if (incassation) break;
              if (GetMode() != MODE_WORK)
                {
                  if (!FlagForPrintReport) break;
                  if (GetCurrentMenu() == xReportMenuPanel)
                    { // �������� X-�����
                     CPU_INT08U err;
                     if (IsFiscalConnected())
                      {
                        FPend();
                        FiscPrintDayReportNoClear(30, &err);
                        FPost();
                        if (err) {SetFiscalErrorByCode(err);}
                        SaveEventRecord(0, JOURNAL_EVENT_PRINT_X, GetTimeSec());
                        GoToPreviousMenu();
                      }
                    }
                  else if (GetCurrentMenu() == zReportMenuPanel)
                   { // �������� Z-�����
                     CPU_INT08U err;
                     if (IsFiscalConnected()) 
                      {
                        FPend();
                        FiscPrintDayReportClear(30, &err);
                        FPost();
                        if (err) {SetFiscalErrorByCode(err);}
                        SaveEventRecord(0, JOURNAL_EVENT_PRINT_Z, GetTimeSec());
                        GoToPreviousMenu();
                        ClrFiscalErrorByCode(FR_ERROR_CODE_4E);
                      }
                   }
                  else if (GetCurrentMenu() == bufReportMenuPanel)
                   { // �������� Z-������ �� ������
                     CPU_INT08U err;
                     if (IsFiscalConnected()) 
                      {
                        FPend();
                        FiscPrintDayReportsFromBuf(30, &err);
                        FPost();
                        if (err) {SetFiscalErrorByCode(err);}
                        SaveEventRecord(0, JOURNAL_EVENT_PRINT_BUF, GetTimeSec());
                        GoToPreviousMenu();
                      }
                   }
                  else if (GetCurrentMenu() == CanselCheckMenuPanel)
                    {
                      int res = CanselFiscalBill();
                      SaveEventRecord(0, JOURNAL_EVENT_PRINT_X, res);
                      CheckFiscalStatus();
                      GoToPreviousMenu();
                    }
                  break;
                }
            
              if (TstCriticalErrors()) 
              {
                UserPrintErrorMenu(); 
                RefreshMenu(); 
                break;
              }
                  
              break;
            case EVENT_STOP_MONEY_POST1:
            case EVENT_STOP_MONEY_POST2:
            case EVENT_STOP_MONEY_POST3:
            case EVENT_STOP_MONEY_POST4:
            case EVENT_STOP_MONEY_POST5:
            case EVENT_STOP_MONEY_POST6:
            //if (GetMode() == MODE_WORK) //
            {
                int number_post = event - EVENT_STOP_MONEY_POST1;
                CPU_INT32U accmoney = 0;

                accmoney = GetAcceptedMoney(number_post);
                accmoney += GetAcceptedBankMoney(number_post);
                
                if ((accmoney > 0) && (wash_State[number_post] != washing) && (wash_State[number_post] != printCheck))
                {
                    wash_State[number_post] = washing;
                    SaveEventRecord(number_post, JOURNAL_EVENT_WASHING, number_post);
                }
                
                money_timestamp[number_post] = OSTimeGet();
            }
            break;

            case EVENT_WAIT_CASH_PRINT_CHECK_POST1:
            case EVENT_WAIT_CASH_PRINT_CHECK_POST2:
            case EVENT_WAIT_CASH_PRINT_CHECK_POST3:
            case EVENT_WAIT_CASH_PRINT_CHECK_POST4:
            case EVENT_WAIT_CASH_PRINT_CHECK_POST5:
            case EVENT_WAIT_CASH_PRINT_CHECK_POST6:
              //if (GetMode() == MODE_WORK) //
              {
                  int number_post = event - EVENT_WAIT_CASH_PRINT_CHECK_POST1;
                  int count_delay = 0;
                  CPU_INT32U accmoney = 0;

                  accmoney = GetAcceptedMoney(number_post);
                  accmoney += GetAcceptedBankMoney(number_post);

                  if ((accmoney > 0) && (wash_State[number_post] != printCheck) && !was_critical_error)
                  {
                    // �������� �������� ������ ����, ���� ��� ������
                    GetData(&PrintTimeoutDesc, &count_delay, 0, DATA_FLAG_DIRECT_INDEX);
                    countSecWait[number_post] = count_delay;
  
                    wash_State[number_post] = printCheck;

                    // ���� �������� ��� - ����� ��������
                    if(countSecWait[number_post] == 0) PostUserEvent(EVENT_CASH_PRINT_CHECK_POST1 + number_post);
                  }
                  else if (was_critical_error)
                  {
                    // ���� ���� ������ - ������ ���������� ������� ����� �����
                    wash_State[number_post] = waitMoney;
                  }
              }
            break;
              
            case EVENT_CASH_PRINT_CHECK_POST1:
            case EVENT_CASH_PRINT_CHECK_POST2:
            case EVENT_CASH_PRINT_CHECK_POST3:
            case EVENT_CASH_PRINT_CHECK_POST4:
            case EVENT_CASH_PRINT_CHECK_POST5:
            case EVENT_CASH_PRINT_CHECK_POST6:
            case EVENT_CASH_PRINT_CHECK_VACUUM1:
            case EVENT_CASH_PRINT_CHECK_VACUUM2:

            //if (GetMode() == MODE_WORK) // ����� ����� ���� ������
            {
              int number_post = event - EVENT_CASH_PRINT_CHECK_POST1;
              CPU_INT32U accmoney = 0;

              // ��������� � �������� ������ �����
              wash_State[number_post] = waitMoney;
              // ������� ������� �������� ������ ���� ����� ������ ������
              time_out_print_check[number_post] = 0;

              if (was_critical_error)
              {
                  break;
              }
              
              // ����� ������� ������ ������ ���� - �������� ����� ��� �������
              accmoney = GetAcceptedMoney(number_post);
              
              if (accmoney > 0)
              { 
                  UserPrintPrintBillMenu(number_post);
                  RefreshMenu();
                  
                  // ���������� �������� ���
                  if (IsFiscalConnected())
                  {
                    if (PrintFiscalBill(accmoney, number_post, 0) == 0) // ����� �������� � ������ ����� ���
                    {
                        SaveEventRecord(number_post, JOURNAL_EVENT_PRINT_BILL_POST1 + number_post, accmoney);
                    }
                  }

                  // ���� ���� ������, �� �������� - ���������� ������ �� ��������
                  if (TstCriticalErrors())
                  {
                    // �������� ����� �����
                    if (was_critical_error == 0) {was_critical_error = 1;}
                    break;
                  }

                  IncCounter(number_post, ChannelsPayedTime[number_post], accmoney);
                  SetAcceptedMoney(0, number_post);
                  if (GetMode() == MODE_WORK) OSTimeDly(1000);
                 
                  // ������� ���� "�������"                      
                  if (IsFiscalConnected())
                  {
                      UserPrintThanksMenu(number_post);
                      RefreshMenu();
                  }
                  
                  if (GetMode() == MODE_WORK) OSTimeDly(1000);
              }
              
              // ����� ������� ������ ������ ���� - �������� ����� ��� �������
              accmoney = GetAcceptedBankMoney(number_post);
              
              if (accmoney > 0)
              { 
                  UserPrintPrintBillMenu(number_post);
                  RefreshMenu();
                  
                  // ���������� ����������� ���
                  if (IsFiscalConnected())
                  {
                    if (PrintFiscalBill(accmoney,number_post, 1) == 0) // ����� �������� � ������ ����� ���
                    {
                        SaveEventRecord(number_post, JOURNAL_EVENT_PRINT_BILL_ONLINE_POST1 + number_post, accmoney);
                    }
                  }

                  // ���� ���� ������, �� �������� - ���������� ������ �� ��������
                  if (TstCriticalErrors()) 
                  {    
                    // �������� ����� �����
                    if (was_critical_error == 0) {was_critical_error = 1;}
                    break;
                  }

                  IncCounter(number_post, ChannelsPayedTime[number_post], accmoney);
                  SetAcceptedBankMoney(0, number_post);
                  if (GetMode() == MODE_WORK) OSTimeDly(1000);
                 
                  // ������� ���� "�������"                      
                  if (IsFiscalConnected())
                  {
                      UserPrintThanksMenu(number_post);
                      RefreshMenu();
                  }
                  
                  if (GetMode() == MODE_WORK) OSTimeDly(1000);
              }
            }
            break;

            case EVENT_KEY_F1:
//                testMoney = 100;
//                PostUserEvent(EVENT_COIN_INSERTED_POST1);
//                PostUserEvent(EVENT_COIN_INSERTED_POST2);
//                PostUserEvent(EVENT_COIN_INSERTED_POST3);
//                PostUserEvent(EVENT_COIN_INSERTED_POST4);
//                PostUserEvent(EVENT_COIN_INSERTED_POST5);
//                PostUserEvent(EVENT_COIN_INSERTED_POST6);
//                PostUserEvent(EVENT_COIN_INSERTED_VACUUM1);
//                PostUserEvent(EVENT_COIN_INSERTED_VACUUM2);

                /*FIO4SET_bit.P4_28 = 1;
                OSTimeDly(50);
                FIO4CLR_bit.P4_28 = 1; 
                OSTimeDly(50);
                FIO4SET_bit.P4_28 = 1;
                OSTimeDly(50);
                FIO4CLR_bit.P4_28 = 1; 
                OSTimeDly(50);
                FIO4SET_bit.P4_28 = 1;
                OSTimeDly(50);
                FIO4CLR_bit.P4_28 = 1;
                OSTimeDly(50);
                FIO4SET_bit.P4_28 = 1;
                OSTimeDly(50);
                FIO4CLR_bit.P4_28 = 1; 
                OSTimeDly(50);
                FIO4SET_bit.P4_28 = 1;
                OSTimeDly(50);
                FIO4CLR_bit.P4_28 = 1;*/
            break;
            case EVENT_KEY_F2:
                testMoney = 0;
//                PostUserEvent(EVENT_STOP_MONEY_POST1);
//                PostUserEvent(EVENT_STOP_MONEY_POST2);
//                PostUserEvent(EVENT_STOP_MONEY_POST3);
//                PostUserEvent(EVENT_STOP_MONEY_POST4);
//                PostUserEvent(EVENT_STOP_MONEY_POST5);
//                PostUserEvent(EVENT_STOP_MONEY_POST6);
            break;
            case EVENT_KEY_F3:
                //testMoney = 100;
//                PostUserEvent(EVENT_WAIT_CASH_PRINT_CHECK_POST1);
//                PostUserEvent(EVENT_WAIT_CASH_PRINT_CHECK_POST2);
//                PostUserEvent(EVENT_WAIT_CASH_PRINT_CHECK_POST3);
//                PostUserEvent(EVENT_WAIT_CASH_PRINT_CHECK_POST4);
//                PostUserEvent(EVENT_WAIT_CASH_PRINT_CHECK_POST5);
//                PostUserEvent(EVENT_WAIT_CASH_PRINT_CHECK_POST6);
            break;
#endif
            default:
              break;
          }
        }
      else 
        {
          OSTimeDly(1);
        }
    }
}

/*!
 ���������������� �������������
*/
void UserStartupFunc(void)
{
#ifdef BOARD_CENTRAL_CFG
  // ������������� ������ ������
  InitMode();
    
  // ������������� ������
  CheckAllData();
    
  //OnChangeInitByDefault();
      
  // �������� ������� ��������
  CheckLongCounters();
      
  // ����������� ������
  LoadAcceptedMoney();
  
  // �������� ������
  InitPass();
    
  // ������������� ����
  InitMenu();

  OSTimeDly(1000);
  
  InitFiscal();

  // ����������������� ����
  InitRTC();

  // ������� ������ � ���������
  SaveEventRecord(0, JOURNAL_EVENT_DEVICE_ON, GetTimeSec());

  // �������� ��������
  InitCoin();

#endif
  
  // �������� ������� � ������
  if (UserQuery == NULL)
    {    
      UserQuery = OSQCreate(&UserTbl[0], USER_QUERY_LEN);
      OSTaskCreate(UserAppTask, (void *)0, (OS_STK *)&UserTaskStk[USER_TASK_STK_SIZE-1], USER_TASK_PRIO);
    }

  //InitConsole();
  
#ifdef BOARD_CENTRAL_CFG
  //InitHostApp();
#endif
      
  SystemTime = GetTimeSec();
  
#ifdef BOARD_CENTRAL_CFG
  // �������� � ��������� ����, ���� ������
  if (GetMode() == MODE_WORK) {SetMenu(WORK_MENU);}
  else SetMenu(SERVICE_MENU);
#endif
  
}

int GetUserEvent(int* event)
{
  CPU_INT08U err = 0; 
  int evt  = (int)OSQPend(UserQuery, 1, &err);
  if (err != 0) return 0;
  *event = evt;
  return 1;  
}   


void PostUserEvent(int event)
{
  OSQPost(UserQuery, (void *)event);
}


void InitUserMenu(void)
{
}

void UserPrintMoneyMenu(int post)
{
    char buf[32];
    CPU_INT32U accmoney;

    if (GetMode() != MODE_WORK) return;
      
    strcpy(buf, " ");
    PrintUserMenuStr(buf, 0);

    accmoney = GetAcceptedMoney(post) + GetAcceptedBankMoney(post);

    if((wash_State[post] == waitMoney) && (accmoney > 0))
    {
        // ���� ���� ������������ � ���� ������ - ����� ��������� �� �������� �����
        wash_State[post] = insertMoney;
    }

    if(wash_State[post] == waitMoney)
    {
      sprintf(buf, " ������� ������");
      PrintUserMenuStr(buf, 1);

      sprintf(buf, " ");
      PrintUserMenuStr(buf, 2);
    }
    else if(wash_State[post] == insertMoney)
    {
      sprintf(buf, " ������� ������");
      PrintUserMenuStr(buf, 1);

      sprintf(buf, "������� %d ���.", accmoney);
      PrintUserMenuStr(buf, 2);
    }
    else if(wash_State[post] == washing)
    {
      sprintf(buf, "�������� �����");
      PrintUserMenuStr(buf, 1);

      sprintf(buf, " ");
      PrintUserMenuStr(buf, 2);
    }
    else if(wash_State[post] == printCheck)
    {
      sprintf(buf, "�������� ������");
      PrintUserMenuStr(buf, 1);

      sprintf(buf, " ");
      PrintUserMenuStr(buf, 2);
    }

    if(post < COUNT_POST)
      sprintf(buf, " ���� %d", post + 1);
    else if(post < COUNT_POST + COUNT_VACUUM)
      sprintf(buf, "������� %d", post + 1 - COUNT_POST);
    else 
      sprintf(buf, " ");

    PrintUserMenuStr(buf, 3);
}
                         
// ����� ���� � ������������� ������
void UserPrintErrorMenu(void)
{
  char buf[32];
  
  if (GetMode() != MODE_WORK) return;
    
  if (TstErrorFlag(ERROR_FR_CONN))
    {
      sprintf(buf, "������");
      PrintUserMenuStr(buf, 0);
      sprintf(buf, "��� ����� � ��");
      PrintUserMenuStr(buf, 1);
      sprintf(buf, "");
      PrintUserMenuStr(buf, 2);
      PrintUserMenuStr(buf, 3);
    }
  else if (TstCriticalFiscalError())
    {
      sprintf(buf, "������");
      PrintUserMenuStr(buf, 0);
      CPU_INT08U errcode = 0;
      sprintf(buf, "������ ��");
      PrintUserMenuStr(buf, 1);
      GetFirstCriticalFiscalError(&errcode);
      GetDataItem(&JournalErrorNumberDesc0, (CPU_INT08U*)buf, errcode);
      PrintUserMenuStr(buf, 2);
      GetDataItem(&JournalErrorNumberDesc1, (CPU_INT08U*)buf, errcode);
      PrintUserMenuStr(buf, 3);
    }
}

void WorkServer(void)
{

}

void UserPrintPrintBillMenu(int post)
{
  char buf[32];
  
  if (GetMode() != MODE_WORK) return;
    
  sprintf(buf, " ");
  PrintUserMenuStr(buf, 0);
  sprintf(buf, "��e� ������");
  PrintUserMenuStr(buf, 1);
  sprintf(buf, "   ����");
  PrintUserMenuStr(buf, 2);
  
  if(post < COUNT_POST)
    sprintf(buf, " ���� %d", post + 1);
  else 
    sprintf(buf, "������� %d", post + 1 - COUNT_POST);

  PrintUserMenuStr(buf, 3);
}

void UserPrintThanksMenu(int post)
{
  char buf[32];
  
  if (GetMode() != MODE_WORK) return;
    
  sprintf(buf, " ");
  PrintUserMenuStr(buf, 0);
  sprintf(buf, "   �������");
  PrintUserMenuStr(buf, 1);
  sprintf(buf, " ");
  PrintUserMenuStr(buf, 2);
  
  if(post < COUNT_POST)
    sprintf(buf, " ���� %d", post + 1);
  else 
    sprintf(buf, "������� %d", post + 1 - COUNT_POST);

  PrintUserMenuStr(buf, 3);
}


int ChannelBusy(CPU_INT08U ch)
{
    return 0;
}
            
void UserPrintFirstMenu(void)
{
    char buf[32];
    
    if (GetMode() != MODE_WORK) return;

    sprintf(buf, " ");
    PrintUserMenuStr(buf, 0);
    sprintf(buf, "    ��� �����");
    PrintUserMenuStr(buf, 1);
    sprintf(buf, "    ���������");
    PrintUserMenuStr(buf, 2);
    sprintf(buf, " ");
    PrintUserMenuStr(buf, 3);
}


// ��������, ���� �� ��������� ������ �� ���������� �������
void LoadAcceptedMoney(void)
{
  CPU_INT32U m,crc,crct;

  for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
  {
    // ������� c���������� ������ �� FRAM
    GetData(&AcceptedMoneyDesc, &m, i, DATA_FLAG_DIRECT_INDEX);    
    // ������� crc16 ���� ����� �� FRAM 
    GetData(&AcceptedMoneyCRC16Desc, &crc, i, DATA_FLAG_DIRECT_INDEX);    
    
    crct = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
  
    if (crct != crc)
      { // ��������, ���� crc �� �������
        m = 0;
        crc = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
        SetData(&AcceptedMoneyDesc, &m, i, DATA_FLAG_DIRECT_INDEX);
        SetData(&AcceptedMoneyCRC16Desc, &crc, i, DATA_FLAG_DIRECT_INDEX);
      }
  }
  
  for(int i = 0; i < COUNT_POST; i++)
  {
    // ������� c���������� ������ �� FRAM
    GetData(&AcceptedBankMoneyDesc, &m, i, DATA_FLAG_DIRECT_INDEX);    
    // ������� crc16 ���� ����� �� FRAM 
    GetData(&AcceptedBankMoneyCRC16Desc, &crc, i, DATA_FLAG_DIRECT_INDEX);    
    
    crct = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
  
    if (crct != crc)
      { // ��������, ���� crc �� �������
        m = 0;
        crc = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
        SetData(&AcceptedBankMoneyDesc, &m, i, DATA_FLAG_DIRECT_INDEX);
        SetData(&AcceptedBankMoneyCRC16Desc, &crc, i, DATA_FLAG_DIRECT_INDEX);
      }
  }
}

// �������� �����
void SetAcceptedMoney(CPU_INT32U money, int post)
{
  CPU_INT32U m,crc;
  m=money;
  crc = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
  SetData(&AcceptedMoneyDesc, &m, post, DATA_FLAG_DIRECT_INDEX);
  SetData(&AcceptedMoneyCRC16Desc, &crc, post, DATA_FLAG_DIRECT_INDEX);
}

// �������� ������� �����
void ClearAcceptedMoney(int post)
{
  CPU_INT32U m,crc;
  m=0;
  crc = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
  SetData(&AcceptedMoneyDesc, &m, post, DATA_FLAG_DIRECT_INDEX);
  SetData(&AcceptedMoneyCRC16Desc, &crc, post, DATA_FLAG_DIRECT_INDEX);
}

// �������� ������� �����
CPU_INT32U GetAcceptedMoney(int post)
{
  CPU_INT32U m;
  GetData(&AcceptedMoneyDesc, &m, post, DATA_FLAG_DIRECT_INDEX);
  return m;
}

// �������� �����
void SetAcceptedBankMoney(CPU_INT32U money, int post)
{
  CPU_INT32U m,crc;
  
  if(post >= COUNT_POST) return;

  m=money;
  crc = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
  SetData(&AcceptedBankMoneyDesc, &m, post, DATA_FLAG_DIRECT_INDEX);
  SetData(&AcceptedBankMoneyCRC16Desc, &crc, post, DATA_FLAG_DIRECT_INDEX);
}

// �������� ������� �����
void ClearAcceptedBankMoney(int post)
{
  CPU_INT32U m,crc;
  
  if(post >= COUNT_POST) return;

  m=0;
  crc = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
  SetData(&AcceptedBankMoneyDesc, &m, post, DATA_FLAG_DIRECT_INDEX);
  SetData(&AcceptedBankMoneyCRC16Desc, &crc, post, DATA_FLAG_DIRECT_INDEX);
}

// �������� ������� �����
CPU_INT32U GetAcceptedBankMoney(int post)
{
  CPU_INT32U m;

  if(post >= COUNT_POST) return 0;

  GetData(&AcceptedBankMoneyDesc, &m, post, DATA_FLAG_DIRECT_INDEX);
  return m;
}

// ������������� ������
void InitPass(void)
{
  CPU_INT32U pass,crc,crct;
  GetData(&PassDesc, &pass, 0, DATA_FLAG_SYSTEM_INDEX);    
  GetData(&PassCRCDesc, &crc, 0, DATA_FLAG_SYSTEM_INDEX);    
  
  crct = crc16((unsigned char*)&pass, sizeof(CPU_INT32U));

  if (crct != crc)
    { // ��������, ���� crc �� �������
      pass = DEFAULT_PASSWORD;
      crc = crc16((unsigned char*)&pass, sizeof(CPU_INT32U));
      SetData(&PassDesc, &pass, 0, DATA_FLAG_SYSTEM_INDEX);
      SetData(&PassCRCDesc, &crc, 0, DATA_FLAG_SYSTEM_INDEX);
    }
}
