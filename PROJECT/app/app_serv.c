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
#include "control.h"
#include "menu.h"
#include "journal.h"
#include "fr.h"
#include "CRC16.h"
#include "host_app.h"
#include "console.h"
#include "keyboard.h"

// если определить этот макрос, будут вноситься деньги по кнопке F1
//#define _DEBUG_MONEY

CPU_INT32U SystemTime;
CPU_INT32U money_timestamp[COUNT_POST + COUNT_VACUUM];
CPU_INT08U EnabledChannelsNum;
CPU_INT08U RecentChannel;
CPU_INT08U UserMenuState;
  #define USER_STATE_FIRST_PAGE                 0
  #define USER_STATE_ACCEPT_MONEY               1
  #define USER_STATE_SHOW_THANKS                2
CPU_INT08U ThanksCtr;

CPU_INT08U ChannelsState[CHANNELS_NUM];
  #define CHANNEL_STATE_FREE            0
  #define CHANNEL_STATE_BUSY            1
  #define CHANNEL_STATE_DISABLED        2
CPU_INT32U ChannelsCounters[CHANNELS_NUM];
CPU_INT32U ChannelsPayedTime[CHANNELS_NUM];

CPU_INT32U incas_bill_nom_counter[24];
CPU_INT32U incas_common_bill_counter;

#define USER_QUERY_LEN  64

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

/*!
 Сервер обработки событий пользователя
*/
void UserAppTask(void *p_arg)
{
  CPU_INT32U print_timeout;
  CPU_INT32U accmoney;
  int event;
  CPU_BOOLEAN dontRedraw = DEF_FALSE;
  
#ifdef BOARD_CENTRAL_CFG
  
  static CPU_INT08U fr_conn_ctr = 0;
    
  incassation = 0;
  was_critical_error = 0;
#endif
      
  while (1)
    {
      if (GetUserEvent(&event))
        {
          switch (event){
#ifdef BOARD_CENTRAL_CFG
            case EVENT_SEC:
              // проверка режима
              CheckMode();
             
              // прочитаем текущее время
              SystemTime = GetTimeSec();
                
              // рабочий сервер - счетчики, состояния и т.п.
              WorkServer();

              // проверим фискальник, если он отвалился
              if ((++fr_conn_ctr % 10) == 0)
              {
                 if ((FiscalConnState == FISCAL_NOCONN) || (TstCriticalFiscalError()))
                 {
                    if (ConnectFiscalFast() == 0)
                    {
                        CheckFiscalStatus();
                    }
                 }
              }

              // сервер ошибок
              ErrorServer();
                
              // дальше только в рабочем режиме
              if (GetMode() != MODE_WORK)
              {
                  break;
              }
                
              // если есть ошибки, не работаем
              if (TstCriticalErrors()) 
              {
                UserPrintErrorMenu(); 
                RefreshMenu(); 
                // выключим прием денег
                if (was_critical_error == 0)
                {
                    CoinDisable();
                    was_critical_error = 1;
                }
                break;
              }
              
              // включим заново прием денег, если была ошибка
              if (was_critical_error)
              {
                  was_critical_error = 0;
                  break;
              }

              dontRedraw = DEF_FALSE;

              for(int post = 0; post < COUNT_POST + COUNT_VACUUM; post++)
              {
                accmoney = GetAcceptedMoney(post);

                if (accmoney > 0)
                {
                    UserPrintMoneyMenu(post);
                    RefreshMenu();

                    dontRedraw = DEF_TRUE;

                    LED_OK_ON();
                    CheckFiscalStatus();

                    {
                        // печать по внешнему сигналу, ждем таймаут отмены
                        GetData(&PrintTimeoutAfterDesc, &print_timeout, 0, DATA_FLAG_SYSTEM_INDEX);
                        if (labs(OSTimeGet() - money_timestamp[post]) > 1000UL * print_timeout)
                        {
                            SetAcceptedMoney(0, post);
                            UserPrintThanksMenu(post);
                            RefreshMenu();
                            OSTimeDly(1000);
                            LED_OK_OFF();
                        }
                    }
                }
                else
                {
                    LED_OK_OFF();
                }
                
                // принимаем деньги
                if(!dontRedraw)
                {
                  UserPrintMoneyMenu(0xFFFF);
                  RefreshMenu();
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
                CPU_INT32U money, accmoney;
                int number_post = event - EVENT_COIN_INSERTED_POST1;
                
                GetData(&CoinPerPulseDesc, &cpp, number_post, DATA_FLAG_DIRECT_INDEX);
                
                money = cpp*GetResetCoinCount(number_post);

                accmoney = GetAcceptedMoney(number_post);
                accmoney += money;

                SetAcceptedMoney(accmoney, number_post);
                money_timestamp[number_post] = OSTimeGet();
                
                if (UserMenuState == USER_STATE_ACCEPT_MONEY)
                {
                    UserPrintMoneyMenu(number_post);  
                    RefreshMenu();
                }
                
                if (money) SaveEventRecord(RecentChannel, JOURNAL_EVENT_MONEY_COIN_POST1 + number_post, money);
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
                CPU_INT32U money, accmoney;
                int number_post = event - EVENT_CASH_INSERTED_POST1;
                
                GetData(&CashPerPulseDesc, &cpp, number_post, DATA_FLAG_DIRECT_INDEX);
                
                money = cpp * 10 /*GetResetCashCount(number_post)*/;
                
                accmoney = GetAcceptedMoney(number_post);
                accmoney += money;
                SetAcceptedMoney(accmoney, number_post);
                money_timestamp[number_post] = OSTimeGet();
                
                if (UserMenuState == USER_STATE_ACCEPT_MONEY)
                {
                    UserPrintMoneyMenu(number_post);  
                    RefreshMenu();
                }
                
                if (money) SaveEventRecord(RecentChannel, JOURNAL_EVENT_MONEY_NOTE_POST1 + number_post, money);
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
                    { // печатаем X-отчет
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
                   { // печатаем Z-отчет
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
                   { // печатаем Z-отчеты из буфера
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
                  break;
                }
            
              if (TstCriticalErrors()) 
              {
                UserPrintErrorMenu(); 
                RefreshMenu(); 
                break;
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
            if (GetMode() == MODE_WORK) // печатаем только в рабочем режиме
            {
              int number_post = event - EVENT_CASH_PRINT_CHECK_POST1;

              // здесь событие старта печати чека - включили насос или пылесос
              CPU_INT32U accmoney = GetAcceptedMoney(number_post);
              
              if (accmoney > 0)
              { 
                  UserPrintPrintBillMenu(number_post);
                  RefreshMenu();
                  
                  // напечатаем чек
                  if (IsFiscalConnected())
                  {
                    if (PrintFiscalBill(accmoney) == 0) // здесь добавить с какого поста чек
                    {
                        SaveEventRecord(RecentChannel, JOURNAL_EVENT_PRINT_BILL, GetTimeSec());
                    }
                  }

                  IncCounter(RecentChannel, ChannelsPayedTime[RecentChannel], accmoney);
                  SetAcceptedMoney(0, number_post);
                  OSTimeDly(1000);
                 
                  // повесим меню "СПАСИБО"                      
                  if (IsFiscalConnected())
                  {
                      UserPrintThanksMenu(number_post);
                      RefreshMenu();
                  }
                  
                  OSTimeDly(1000);
                  LED_OK_OFF();
              }
            }
            break;

            case EVENT_KEY_F1:
                PostUserEvent(EVENT_CASH_INSERTED_POST1);
            break;
            case EVENT_KEY_F2:
                PostUserEvent(EVENT_CASH_INSERTED_POST2);
            break;
            case EVENT_KEY_F3:
                PostUserEvent(EVENT_CASH_PRINT_CHECK_POST1);
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
 Пользовательская инициализация
*/
void UserStartupFunc(void)
{
#ifdef BOARD_CENTRAL_CFG
  // инициализация режима работы
  InitMode();
    
  // инициализация данных
  CheckAllData();
    
  OnChangeInitByDefault();
      
  // проверим длинные счетчики
  CheckLongCounters();
      
  // восстановим деньги
  LoadAcceptedMoney();
  
  // проверим пароль
  InitPass();
    
  // инициализация меню
  InitMenu();

  OSTimeDly(1000);
  
  InitFiscal();

  // проинициализируем часы
  InitRTC();

  // сделаем запись о включении
  SaveEventRecord(0, JOURNAL_EVENT_DEVICE_ON, GetTimeSec());

  // запустим монетник
  InitCoin();

#endif
  
  // создадим очередь и задачу
  if (UserQuery == NULL)
    {    
      UserQuery = OSQCreate(&UserTbl[0], USER_QUERY_LEN);
      OSTaskCreate(UserAppTask, (void *)0, (OS_STK *)&UserTaskStk[USER_TASK_STK_SIZE-1], USER_TASK_PRIO);
    }

  InitConsole();
  
#ifdef BOARD_CENTRAL_CFG
  InitHostApp();
#endif
      
  SystemTime = GetTimeSec();
  
#ifdef BOARD_CENTRAL_CFG
  // перейдем в стартовое меню, если работа
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
  for (int i = 0; i < CHANNELS_NUM; i++)
  {
    CPU_INT32U en = 0;
    GetData(&EnableChannelDesc, &en, i, DATA_FLAG_DIRECT_INDEX);
    if (en)
    {
        ChannelsState[i] = CHANNEL_STATE_FREE;
    }
    else
    {
        ChannelsState[i] = CHANNEL_STATE_DISABLED;
    }
  }
}

void UserPrintMoneyMenu(int post)
{
    char buf[32];
    CPU_INT32U accmoney;

    strcpy(buf, " ");
    PrintUserMenuStr(buf, 0);
    sprintf(buf, " Внесите деньги");
    PrintUserMenuStr(buf, 1);
    accmoney = GetAcceptedMoney(post);
    sprintf(buf, "Принято %d руб.", accmoney);
    PrintUserMenuStr(buf, 2);
    
    if(post < COUNT_POST)
      sprintf(buf, " Пост %d", post + 1);
    else if(post < COUNT_POST + COUNT_VACUUM)
      sprintf(buf, "Пылесос %d", post + 1 - COUNT_POST);
    else 
      sprintf(buf, " ");

    PrintUserMenuStr(buf, 3);
}
                         
// вывод меню о невозможости работы
void UserPrintErrorMenu(void)
{
  char buf[32];
  
  if (TstErrorFlag(ERROR_FR_CONN))
    {
      sprintf(buf, "ОШИБКА");
      PrintUserMenuStr(buf, 0);
      sprintf(buf, "НЕТ СВЯЗИ С ФР");
      PrintUserMenuStr(buf, 1);
      sprintf(buf, "");
      PrintUserMenuStr(buf, 2);
      PrintUserMenuStr(buf, 3);
    }
  else if (TstCriticalFiscalError())
    {
      sprintf(buf, "ОШИБКА");
      PrintUserMenuStr(buf, 0);
      CPU_INT08U errcode = 0;
      sprintf(buf, "ОШИБКА ФР");
      PrintUserMenuStr(buf, 1);
      GetFirstCriticalFiscalError(&errcode);
      GetDataItem(&JournalErrorNumberDesc0, (CPU_INT08U*)buf, errcode);
      PrintUserMenuStr(buf, 2);
      GetDataItem(&JournalErrorNumberDesc1, (CPU_INT08U*)buf, errcode);
      PrintUserMenuStr(buf, 3);
    }
}

int CheckChannelEnabled(CPU_INT08U channel)
{
    CPU_INT32U en = 0;
    GetData(&EnableChannelDesc, &en, channel, DATA_FLAG_DIRECT_INDEX);
    if (en)
    {
        return 1;
    }
    return 0;
}


void WorkServer(void)
{

}

void UserPrintPrintBillMenu(int post)
{
  char buf[32];
  sprintf(buf, " ");
  PrintUserMenuStr(buf, 0);
  sprintf(buf, "Идeт печать");
  PrintUserMenuStr(buf, 1);
  sprintf(buf, "   чека");
  PrintUserMenuStr(buf, 2);
  
  if(post < COUNT_POST)
    sprintf(buf, " Пост %d", post + 1);
  else 
    sprintf(buf, "Пылесос %d", post + 1 - COUNT_POST);

  PrintUserMenuStr(buf, 3);
}

void UserPrintThanksMenu(int post)
{
  char buf[32];
  sprintf(buf, " ");
  PrintUserMenuStr(buf, 0);
  sprintf(buf, "   СПАСИБО");
  PrintUserMenuStr(buf, 1);
  sprintf(buf, " ");
  PrintUserMenuStr(buf, 2);
  
  if(post < COUNT_POST)
    sprintf(buf, " Пост %d", post + 1);
  else 
    sprintf(buf, "Пылесос %d", post + 1 - COUNT_POST);

  PrintUserMenuStr(buf, 3);
}


int ChannelBusy(CPU_INT08U ch)
{
    return 0;
}
            
void UserPrintFirstMenu(void)
{
    char buf[32];
    sprintf(buf, " ");
    PrintUserMenuStr(buf, 0);
    sprintf(buf, "    ВНЕСИТЕ");
    PrintUserMenuStr(buf, 1);
    sprintf(buf, "    ДЕНЬГИ");
    PrintUserMenuStr(buf, 2);
    sprintf(buf, " ");
    PrintUserMenuStr(buf, 3);
}


// проверка, были ли сохранены деньги до выключения питания
void LoadAcceptedMoney(void)
{
  CPU_INT32U m,crc,crct;

  for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
  {
    // считаем cохраненные деньги из FRAM
    GetData(&AcceptedMoneyDesc, &m, i, DATA_FLAG_DIRECT_INDEX);    
    // считаем crc16 этих денег из FRAM 
    GetData(&AcceptedMoneyCRC16Desc, &crc, i, DATA_FLAG_DIRECT_INDEX);    
    
    crct = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
  
    if (crct != crc)
      { // обнуляем, если crc не сошлась
        m = 0;
        crc = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
        SetData(&AcceptedMoneyDesc, &m, i, DATA_FLAG_DIRECT_INDEX);
        SetData(&AcceptedMoneyCRC16Desc, &crc, i, DATA_FLAG_DIRECT_INDEX);
      }
  }
}

// добавить денег
void SetAcceptedMoney(CPU_INT32U money, int post)
{
  CPU_INT32U m,crc;
  m=money;
  crc = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
  SetData(&AcceptedMoneyDesc, &m, post, DATA_FLAG_DIRECT_INDEX);
  SetData(&AcceptedMoneyCRC16Desc, &crc, post, DATA_FLAG_DIRECT_INDEX);
}

// очистить счетчик денег
void ClearAcceptedMoney(int post)
{
  CPU_INT32U m,crc;
  m=0;
  crc = crc16((unsigned char*)&m, sizeof(CPU_INT32U));
  SetData(&AcceptedMoneyDesc, &m, post, DATA_FLAG_DIRECT_INDEX);
  SetData(&AcceptedMoneyCRC16Desc, &crc, post, DATA_FLAG_DIRECT_INDEX);
}

// очистить счетчик денег
CPU_INT32U GetAcceptedMoney(int post)
{
  CPU_INT32U m;
  GetData(&AcceptedMoneyDesc, &m, post, DATA_FLAG_DIRECT_INDEX);
  return m;
}

// инициализация пароля
void InitPass(void)
{
  CPU_INT32U pass,crc,crct;
  GetData(&PassDesc, &pass, 0, DATA_FLAG_SYSTEM_INDEX);    
  GetData(&PassCRCDesc, &crc, 0, DATA_FLAG_SYSTEM_INDEX);    
  
  crct = crc16((unsigned char*)&pass, sizeof(CPU_INT32U));

  if (crct != crc)
    { // обнуляем, если crc не сошлась
      pass = DEFAULT_PASSWORD;
      crc = crc16((unsigned char*)&pass, sizeof(CPU_INT32U));
      SetData(&PassDesc, &pass, 0, DATA_FLAG_SYSTEM_INDEX);
      SetData(&PassCRCDesc, &crc, 0, DATA_FLAG_SYSTEM_INDEX);
    }
}
