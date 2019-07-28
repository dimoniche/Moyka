#include "iolpc2368.h"
#include "ucos_ii.h"
#include "cpu.h"
#include "app_serv.h"
#include "coin.h"
#include "data.h"
#include "datadesc.h"
#include <stdlib.h>

OS_STK  CoinTaskStk[COIN_TASK_STK_SIZE];

void  InitImpInput(void);

CPU_INT32U  CoinImpCounter[COUNT_POST + COUNT_VACUUM];
CPU_INT32U  CashImpCounter[COUNT_POST + COUNT_VACUUM];
CPU_INT32U  BankImpCounter[COUNT_POST + COUNT_VACUUM];

static CPU_INT32U coin_pulse[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U coin_pause[COUNT_POST + COUNT_VACUUM];
static char pend_coin_counter[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U pend_coin_timestamp[COUNT_POST + COUNT_VACUUM];

static CPU_INT32U cash_pulse[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U cash_pause[COUNT_POST + COUNT_VACUUM];
static char pend_cash_counter[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U pend_cash_timestamp[COUNT_POST + COUNT_VACUUM];

static CPU_INT32U bank_pulse[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U bank_pause[COUNT_POST + COUNT_VACUUM];
static char pend_bank_counter[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U pend_bank_timestamp[COUNT_POST + COUNT_VACUUM];

static CPU_INT32U signal_pulse[COUNT_POST + COUNT_VACUUM];
static char pend_upsignal_counter[COUNT_POST + COUNT_VACUUM];
static char pend_downsignal_counter[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U pend_signal_timestamp[COUNT_POST + COUNT_VACUUM];

CPU_INT32U cashLevel[COUNT_POST + COUNT_VACUUM];
CPU_INT32U coinLevel[COUNT_POST + COUNT_VACUUM];
CPU_INT32U SignalLevel[COUNT_POST + COUNT_VACUUM];
CPU_INT32U bankLevel[COUNT_POST + COUNT_VACUUM];

void SetCashPulseParam(CPU_INT32U pulse, CPU_INT32U pause, CPU_INT32U post)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  coin_pulse[post] = pulse * 1;
  coin_pause[post] = pause;
  OS_EXIT_CRITICAL();
}

void SetCoinPulseParam(CPU_INT32U pulse, CPU_INT32U pause, CPU_INT32U post)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  cash_pulse[post] = pulse * 1;
  cash_pause[post] = pause;
  OS_EXIT_CRITICAL();
}

void SetBankPulseParam(CPU_INT32U pulse, CPU_INT32U pause, CPU_INT32U post)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  bank_pulse[post] = pulse * 1;
  bank_pause[post] = pause;
  OS_EXIT_CRITICAL();
}

void SetSignalPulseParam(CPU_INT32U pulse, CPU_INT32U post)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  signal_pulse[post] = pulse * 1000;
  OS_EXIT_CRITICAL();
}

void SetLevelParam(CPU_INT32U level1, CPU_INT32U level2, CPU_INT32U level3, CPU_INT32U level4, CPU_INT32U post)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  if(post < COUNT_POST)
  {
    cashLevel[post] = level1;
    bankLevel[post] = level2;
    SignalLevel[post] = level3;
  }
  
  coinLevel[post] = level4;
  
  OS_EXIT_CRITICAL();
}

void CoinTask(void *p_arg)
{
  CPU_INT32U enable_coin[COUNT_POST + COUNT_VACUUM];
  CPU_INT32U cash_enable[COUNT_POST];
  CPU_INT32U enable_signal[COUNT_POST];
  CPU_INT32U bank_enable[COUNT_POST];

  CPU_INT32U last_coin_count[COUNT_POST];
  CPU_INT32U last_coin_time[COUNT_POST];

  CPU_INT32U last_cash_count[COUNT_POST];
  CPU_INT32U last_cash_time[COUNT_POST];

  CPU_INT32U last_bank_count[COUNT_POST];
  CPU_INT32U last_bank_time[COUNT_POST];

  CPU_INT32U last_settings_time = 0;

  while(1)
    {
        if (OSTimeGet() - last_settings_time > 1000)
        {
            for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
            {
              GetData(&EnableCoinDesc, &enable_coin[i], i, DATA_FLAG_DIRECT_INDEX);  
              
              if(i < COUNT_POST)
              {
                GetData(&EnableValidatorDesc, &cash_enable[i], i, DATA_FLAG_DIRECT_INDEX);
                GetData(&EnableSignalDesc, &enable_signal[i], i, DATA_FLAG_DIRECT_INDEX);
                GetData(&EnableBankDesc, &bank_enable[i], i, DATA_FLAG_DIRECT_INDEX);
              }
            }
            
            last_settings_time = OSTimeGet();
        }

		OSTimeDly(1);

        #if OS_CRITICAL_METHOD == 3
        OS_CPU_SR  cpu_sr = 0;
        #endif

		for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
		{
            if (enable_coin[i])
            {
              OS_ENTER_CRITICAL();
              if (pend_coin_counter[i])
              {
                // импульсы инкрементируем только после выдержки паузы
                if (OSTimeGet() - pend_coin_timestamp[i] > coin_pause[i])
                {
                  pend_coin_counter[i] = 0;
                  CoinImpCounter[i]++;
                }
              }
              OS_EXIT_CRITICAL();
                  
              if (GetCoinCount(i))
              {
                if (last_coin_count[i] == GetCoinCount(i))
                {
                    if (labs(OSTimeGet() - last_coin_time[i]) > 500)
                    {
                      PostUserEvent(EVENT_COIN_INSERTED_POST1 + i);
                    }
                }
                else
                {
                    last_coin_count[i] = GetCoinCount(i);
                    last_coin_time[i] = OSTimeGet();
                }
              }
              else
              {
                last_coin_time[i] = OSTimeGet();
              }
            }
            else
            {
				CoinDisable();
				GetResetCoinCount(i);
            }

            if(i >= COUNT_POST) continue;
            // только монетоприемников 8 - остальных каналов 6
            
            OS_ENTER_CRITICAL();
			if (enable_signal[i])
			{
				if (pend_upsignal_counter[i])
				{
                  if (OSTimeGet() - pend_signal_timestamp[i] > signal_pulse[i])
                  {
                    // есть удержание сигнала печати
                    PostUserEvent(EVENT_WAIT_CASH_PRINT_CHECK_POST1 + i);
                    pend_upsignal_counter[i] = 0;
                  }
				}
                if (pend_downsignal_counter[i])
				{
                  if (OSTimeGet() - pend_signal_timestamp[i] > signal_pulse[i])
                  {
                    // есть окончание приема денег
                    PostUserEvent(EVENT_STOP_MONEY_POST1 + i);
                    pend_downsignal_counter[i] = 0;
                  }
				}
			}
            OS_EXIT_CRITICAL();

            if (bank_enable[i])
            {
              OS_ENTER_CRITICAL();
              if (pend_bank_counter[i])
              {
                // импульсы инкрементируем только после выдержки паузы
                if (OSTimeGet() - pend_bank_timestamp[i] > bank_pause[i])
                {
                  pend_bank_counter[i] = 0;
                  BankImpCounter[i]++;
                }
              }
              OS_EXIT_CRITICAL();
                  
              if (GetbankCount(i))
              {
                if (last_bank_count[i] == GetbankCount(i))
                {
                    if (labs(OSTimeGet() - last_bank_time[i]) > 500)
                    {
                      PostUserEvent(EVENT_BANK_INSERTED_POST1 + i);
                    }
                }
                else
                {
                    last_bank_count[i] = GetbankCount(i);
                    last_bank_time[i] = OSTimeGet();
                }
              }
              else
              {
                last_bank_time[i] = OSTimeGet();
              }
            }
            else
            {
              GetResetbankCount(i);
            }

			if (!cash_enable[i]) {GetResetCashCount(i); continue;}

			OS_ENTER_CRITICAL();
			if (pend_cash_counter[i])
			{
			  // импульсы инкрементируем только после выдержки паузы
			  if (OSTimeGet() - pend_cash_timestamp[i] > cash_pause[i])
			  {
				pend_cash_counter[i] = 0;
				CashImpCounter[i]++;
			  }
			}
			OS_EXIT_CRITICAL();
				
            if (GetCashCount(i))
            {
              if (last_cash_count[i] == GetCashCount(i))
              {
                  if (labs(OSTimeGet() - last_cash_time[i]) > 1000)
                  {
                    PostUserEvent(EVENT_CASH_INSERTED_POST1 + i);
                  }
              }
              else
              {
                  last_cash_count[i] = GetCashCount(i);
                  last_cash_time[i] = OSTimeGet();
              }
            }
            else
            {
              last_cash_time[i] = OSTimeGet();
            }
		}
    }
}

void CoinDisable(void)
{

}

void CoinEnable(void)
{

}

// получить число монет
CPU_INT32U GetCoinCount(int index)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  CPU_INT32U ctr = CoinImpCounter[index];
  OS_EXIT_CRITICAL();
  return ctr;
}

// получить число монет и сбросить счетчик
CPU_INT32U GetResetCoinCount(int index)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  CPU_INT32U ctr = CoinImpCounter[index];
  CoinImpCounter[index] = 0;
  OS_EXIT_CRITICAL();
  return ctr;
}

// получить число импульсов от купюрника
CPU_INT32U GetCashCount(int index)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  CPU_INT32U ctr = CashImpCounter[index];
  OS_EXIT_CRITICAL();
  return ctr;
}

// получить число импульсов от купюрника и сбросить счетчик
CPU_INT32U GetResetCashCount(int index)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  CPU_INT32U ctr = CashImpCounter[index];
  CashImpCounter[index] = 0;
  OS_EXIT_CRITICAL();
  return ctr;
}

// получить число импульсов от купюрника
CPU_INT32U GetbankCount(int index)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  CPU_INT32U ctr = BankImpCounter[index];
  OS_EXIT_CRITICAL();
  return ctr;
}

// получить число импульсов от купюрника и сбросить счетчик
CPU_INT32U GetResetbankCount(int index)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  CPU_INT32U ctr = BankImpCounter[index];
  BankImpCounter[index] = 0;
  OS_EXIT_CRITICAL();
  return ctr;
}

CPU_INT32U period[COUNT_POST + COUNT_VACUUM];
CPU_INT32U period_cash[COUNT_POST + COUNT_VACUUM];
CPU_INT32U period_bank[COUNT_POST + COUNT_VACUUM];
CPU_INT32U period_signal[COUNT_POST + COUNT_VACUUM];

CPU_INT32U T3CR = 0;

// инициализация монетоприемника
void InitCoin(void)
{
  for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
  {
    CoinImpCounter[i] = 0;
    CashImpCounter[i] = 0;
    BankImpCounter[i] = 0;
    
    coin_pulse[i] = 50;
    coin_pause[i] = 20;
    pend_coin_counter[i] = 0;
    pend_coin_timestamp[i] = 0;
    
    cash_pulse[i] = 50;
    cash_pause[i] = 20;
    pend_cash_counter[i] = 0;
    pend_cash_timestamp[i] = 0;

    bank_pulse[i] = 50;
    bank_pause[i] = 20;
    pend_bank_counter[i] = 0;
    pend_bank_timestamp[i] = 0;

    signal_pulse[i] = 1000;
    pend_upsignal_counter[i] = 0;
    pend_downsignal_counter[i] = 0;
    pend_signal_timestamp[i] = 0;
    
    period[i] = 0;
    period_cash[i] = 0;
    period_bank[i] = 0;
    period_signal[i] = 0;
  }
  
  InitImpInput();
  OSTaskCreate(CoinTask, (void *)0, (OS_STK *)&CoinTaskStk[COIN_TASK_STK_SIZE-1], COIN_TASK_PRIO);
}

CPU_INT32U input_register()
{
  CPU_INT32U input = 0;
  
  if (FIO1PIN_bit.P1_20)
  {
     SETBIT(input, 0);
  }
  if (FIO1PIN_bit.P1_21)
  {
     SETBIT(input, 1);
  }
  if (FIO4PIN_bit.P4_28)
  {
     SETBIT(input, 2);
  }
  if (FIO1PIN_bit.P1_19)
  {
     SETBIT(input, 3);
  }
  if (FIO1PIN_bit.P1_18)
  {
     SETBIT(input, 4);
  }  
  if (FIO0PIN_bit.P0_4)
  {
     SETBIT(input, 5);
  }
  if (FIO3PIN_bit.P3_25)
  {
     SETBIT(input, 6);
  }
  if (FIO3PIN_bit.P3_26)
  {
     SETBIT(input, 7);
  }
  if (FIO1PIN_bit.P1_28)
  {
     SETBIT(input, 8);
  }
  if (FIO0PIN_bit.P0_26)
  {
     SETBIT(input, 9);
  }
  if (FIO0PIN_bit.P0_25)
  {
     SETBIT(input, 10);
  }
  if (FIO1PIN_bit.P1_27)
  {
     SETBIT(input, 11);
  }
  if (FIO0PIN_bit.P0_9)
  {
     SETBIT(input, 12);
  }
  if (FIO2PIN_bit.P2_2)
  {
     SETBIT(input, 13);
  }
  if (FIO1PIN_bit.P1_26)
  {
     SETBIT(input, 14);
  }
  if (FIO0PIN_bit.P0_7)
  {
     SETBIT(input, 15);
  }
  if (FIO0PIN_bit.P0_8)
  {
     SETBIT(input, 16);
  }
  if (FIO0PIN_bit.P0_0)
  {
     SETBIT(input, 17);
  }
  if (FIO0PIN_bit.P0_5)
  {
     SETBIT(input, 18);
  }
  if (FIO1PIN_bit.P1_25)
  {
     SETBIT(input, 19);
  }
  if (FIO0PIN_bit.P0_28)
  {
     SETBIT(input, 20);
  }
  if (FIO0PIN_bit.P0_27)
  {
     SETBIT(input, 21);
  }
  if (FIO1PIN_bit.P1_24)
  {
     SETBIT(input, 22);
  }
  if (FIO1PIN_bit.P1_23)
  {
     SETBIT(input, 23);
  }
  if (FIO0PIN_bit.P0_6)
  {
     SETBIT(input, 24);
  }
  if (FIO0PIN_bit.P0_10)
  {
     SETBIT(input, 25);
  }

  return input;
}

CPU_INT32U input_event = 0;
CPU_INT32U prev_input = 0;
CPU_INT32U curr_input = 0;

void InputCapture_ISR(void)
{
  T3IR = 0xFF;

  // наращиваем тики
  T3CR++;

  curr_input = input_register();
  input_event = curr_input^prev_input;
  prev_input = curr_input;

  // пост 1
  // купюроприемник 1
  if(TSTBIT(input_event, 0))
  {
    if ((!FIO1PIN_bit.P1_20 && cashLevel[0]) || (FIO1PIN_bit.P1_20 && !cashLevel[0]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_cash[0];
        
        if (cr > (cash_pulse[0] - COIN_IMP_SPAN))
        {
            pend_cash_counter[0] = 1;
            pend_cash_timestamp[0] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_cash[0] = T3CR;
        pend_cash_counter[0] = 0;
      }
  }

  // монетоприемник 1
  if(TSTBIT(input_event, 1))
  {
    if ((!FIO1PIN_bit.P1_21 && coinLevel[0]) || (FIO1PIN_bit.P1_21 && !coinLevel[0]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period[0];
        
        if (cr > (coin_pulse[0] - COIN_IMP_SPAN))
        {
            pend_coin_counter[0] = 1;
            pend_coin_timestamp[0] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period[0] = T3CR;
        pend_coin_counter[0] = 0;
      }
  }
  
  // сигнал печати чека 1
  if(TSTBIT(input_event, 2))
  {
    pend_signal_timestamp[0] = OSTimeGet();
    
    if ((FIO4PIN_bit.P4_28 && SignalLevel[0]) || (!FIO4PIN_bit.P4_28 && !SignalLevel[0]))
      {
          pend_upsignal_counter[0] = 1;
          pend_downsignal_counter[0] = 0;
      }
    else
      {
          pend_upsignal_counter[0] = 0;
          pend_downsignal_counter[0] = 1;
      }
  }
  
  // пост 2
  // купюроприемник 2
  if(TSTBIT(input_event, 3))
  {
    if ((!FIO1PIN_bit.P1_19 && cashLevel[1]) || (FIO1PIN_bit.P1_19 && !cashLevel[1]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_cash[1];
        
        if (cr > (cash_pulse[1] - COIN_IMP_SPAN))
        {
            pend_cash_counter[1] = 1;
            pend_cash_timestamp[1] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_cash[1] = T3CR;
        pend_cash_counter[1] = 0;
      }
  }

  // монетоприемник 2
  if(TSTBIT(input_event, 4))
  {
    if ((!FIO1PIN_bit.P1_18 && coinLevel[1]) || (FIO1PIN_bit.P1_18 && !coinLevel[1]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period[1];
        
        if (cr > (coin_pulse[1] - COIN_IMP_SPAN))
        {
            pend_coin_counter[1] = 1;
            pend_coin_timestamp[1] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period[1] = T3CR;
        pend_coin_counter[1] = 0;
      }
  }
  
  // сигнал печати чека 2
  if(TSTBIT(input_event, 5))
  {
    pend_signal_timestamp[1] = OSTimeGet();
    
    if ((FIO0PIN_bit.P0_4 && SignalLevel[1]) || (!FIO0PIN_bit.P0_4 && !SignalLevel[1]))
      {
          pend_upsignal_counter[1] = 1;
          pend_downsignal_counter[1] = 0;
      }
    else
      {
          pend_upsignal_counter[1] = 0;
          pend_downsignal_counter[1] = 1;
      }
  }

  // пост 3
  // купюроприемник 3
  if(TSTBIT(input_event, 6))
  {
    if ((!FIO3PIN_bit.P3_25 && cashLevel[2]) || (FIO3PIN_bit.P3_25 && !cashLevel[2]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_cash[2];
        
        if (cr > (cash_pulse[2] - COIN_IMP_SPAN))
        {
            pend_cash_counter[2] = 1;
            pend_cash_timestamp[2] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_cash[2] = T3CR;
        pend_cash_counter[2] = 0;
      }
  }

  // монетоприемник 3
  if(TSTBIT(input_event, 7))
  {
    if ((!FIO3PIN_bit.P3_26 && coinLevel[2]) || (FIO3PIN_bit.P3_26 && !coinLevel[2]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period[2];
        
        if (cr > (coin_pulse[2] - COIN_IMP_SPAN))
        {
            pend_coin_counter[2] = 1;
            pend_coin_timestamp[2] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period[2] = T3CR;
        pend_coin_counter[2] = 0;
      }
  }
  
  // сигнал печати чека 3
  if(TSTBIT(input_event, 8))
  {
    pend_signal_timestamp[2] = OSTimeGet();
    
    if ((FIO1PIN_bit.P1_28 && SignalLevel[2]) || (!FIO1PIN_bit.P1_28 && !SignalLevel[2]))
      {
          pend_upsignal_counter[2] = 1;
          pend_downsignal_counter[2] = 0;
      }
    else
      {
          pend_upsignal_counter[2] = 0;
          pend_downsignal_counter[2] = 1;
      }
  }

  // пост 4
  // купюроприемник 4
  if(TSTBIT(input_event, 9))
  {
    if ((!FIO0PIN_bit.P0_26 && cashLevel[3]) || (FIO0PIN_bit.P0_26 && !cashLevel[3]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_cash[3];
        
        if (cr > (cash_pulse[3] - COIN_IMP_SPAN))
        {
            pend_cash_counter[3] = 1;
            pend_cash_timestamp[3] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_cash[3] = T3CR;
        pend_cash_counter[3] = 0;
      }
  }

  // монетоприемник 4
  if(TSTBIT(input_event, 10))
  {
    if ((!FIO0PIN_bit.P0_25 && coinLevel[3]) || (FIO0PIN_bit.P0_25 && !coinLevel[3]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period[3];
        
        if (cr > (coin_pulse[3] - COIN_IMP_SPAN))
        {
            pend_coin_counter[3] = 1;
            pend_coin_timestamp[3] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period[3] = T3CR;
        pend_coin_counter[3] = 0;
      }
  }
  
  // сигнал печати чека 4
  if(TSTBIT(input_event, 11))
  {
    pend_signal_timestamp[3] = OSTimeGet();
    
    if ((FIO1PIN_bit.P1_27 && SignalLevel[2]) || (!FIO1PIN_bit.P1_27 && !SignalLevel[3]))
      {
          pend_upsignal_counter[3] = 1;
          pend_downsignal_counter[3] = 0;
      }
    else
      {
          pend_upsignal_counter[3] = 0;
          pend_downsignal_counter[3] = 1;
      }
  }

  // пост 5
  // купюроприемник 5
  if(TSTBIT(input_event, 12))
  {
    if ((!FIO0PIN_bit.P0_9 && cashLevel[4]) || (FIO0PIN_bit.P0_9 && !cashLevel[4]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_cash[4];
        
        if (cr > (cash_pulse[4] - COIN_IMP_SPAN))
        {
            pend_cash_counter[4] = 1;
            pend_cash_timestamp[4] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_cash[4] = T3CR;
        pend_cash_counter[4] = 0;
      }
  }

  // монетоприемник 5
  if(TSTBIT(input_event, 13))
  {
    if ((!FIO2PIN_bit.P2_2 && coinLevel[4]) || (FIO2PIN_bit.P2_2 && !coinLevel[4]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period[4];
        
        if (cr > (coin_pulse[4] - COIN_IMP_SPAN))
        {
            pend_coin_counter[4] = 1;
            pend_coin_timestamp[4] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period[4] = T3CR;
        pend_coin_counter[4] = 0;
      }
  }
  
  // сигнал печати чека 5
  if(TSTBIT(input_event, 14))
  {
    pend_signal_timestamp[4] = OSTimeGet();
    
    if ((FIO1PIN_bit.P1_26 && SignalLevel[4]) || (!FIO1PIN_bit.P1_26 && !SignalLevel[4]))
      {
          pend_upsignal_counter[4] = 1;
          pend_downsignal_counter[4] = 0;
      }
    else
      {
          pend_upsignal_counter[4] = 0;
          pend_downsignal_counter[4] = 1;
      }
  }

  // пост 6
  // купюроприемник 6
  if(TSTBIT(input_event, 15))
  {
    if ((!FIO0PIN_bit.P0_7 && cashLevel[5]) || (FIO0PIN_bit.P0_7 && !cashLevel[5]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_cash[5];
        
        if (cr > (cash_pulse[5] - COIN_IMP_SPAN))
        {
            pend_cash_counter[5] = 1;
            pend_cash_timestamp[5] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_cash[5] = T3CR;
        pend_cash_counter[5] = 0;
      }
  }

  // монетоприемник 6
  if(TSTBIT(input_event, 16))
  {
    if ((!FIO0PIN_bit.P0_8 && coinLevel[5]) || (FIO0PIN_bit.P0_8 && !coinLevel[5]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period[5];
        
        if (cr > (coin_pulse[5] - COIN_IMP_SPAN))
        {
            pend_coin_counter[5] = 1;
            pend_coin_timestamp[5] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period[5] = T3CR;
        pend_coin_counter[5] = 0;
      }
  }
  
  // сигнал печати чека 6
  if(TSTBIT(input_event, 17))
  {
    pend_signal_timestamp[5] = OSTimeGet();
    
    if ((FIO0PIN_bit.P0_0 && SignalLevel[5]) || (!FIO0PIN_bit.P0_0 && !SignalLevel[5]))
      {
          pend_upsignal_counter[5] = 1;
          pend_downsignal_counter[5] = 0;
      }
    else
      {
          pend_upsignal_counter[5] = 0;
          pend_downsignal_counter[5] = 1;
      }
  }

  // монетоприемник пылесос 1
  if(TSTBIT(input_event, 18))
  {
    if ((!FIO0PIN_bit.P0_5 && coinLevel[6]) || (FIO0PIN_bit.P0_5 && !coinLevel[6]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period[6];
        
        if (cr > (coin_pulse[6] - COIN_IMP_SPAN))
        {
            pend_coin_counter[6] = 1;
            pend_coin_timestamp[6] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period[6] = T3CR;
        pend_coin_counter[6] = 0;
      }
  }

  // монетоприемник пылесос 2
  if(TSTBIT(input_event, 19))
  {
    if ((!FIO1PIN_bit.P1_25 && coinLevel[7]) || (FIO1PIN_bit.P1_25 && !coinLevel[7]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period[7];
        
        if (cr > (coin_pulse[7] - COIN_IMP_SPAN))
        {
            pend_coin_counter[7] = 1;
            pend_coin_timestamp[7] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period[7] = T3CR;
        pend_coin_counter[7] = 0;
      }
  }
  
  // банк 1
  if(TSTBIT(input_event, 20))
  {
    if ((!FIO0PIN_bit.P0_28 && bankLevel[0]) || (FIO0PIN_bit.P0_28 && !bankLevel[0]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_bank[0];
        
        if (cr > (bank_pulse[0] - COIN_IMP_SPAN))
        {
            pend_bank_counter[0] = 1;
            pend_bank_timestamp[0] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_bank[0] = T3CR;
        pend_bank_counter[0] = 0;
      }
  }

  // банк 2
  if(TSTBIT(input_event, 21))
  {
    if ((!FIO0PIN_bit.P0_27 && bankLevel[1]) || (FIO0PIN_bit.P0_27 && !bankLevel[1]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_bank[1];
        
        if (cr > (bank_pulse[1] - COIN_IMP_SPAN))
        {
            pend_bank_counter[1] = 1;
            pend_bank_timestamp[1] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_bank[1] = T3CR;
        pend_bank_counter[1] = 0;
      }
  }
  
  // банк 3
  if(TSTBIT(input_event, 22))
  {
    if ((!FIO1PIN_bit.P1_24 && bankLevel[2]) || (FIO1PIN_bit.P1_24 && !bankLevel[2]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_bank[2];
        
        if (cr > (bank_pulse[2] - COIN_IMP_SPAN))
        {
            pend_bank_counter[2] = 1;
            pend_bank_timestamp[2] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_bank[2] = T3CR;
        pend_bank_counter[2] = 0;
      }
  }
  
  // банк 4
  if(TSTBIT(input_event, 23))
  {
    if ((!FIO1PIN_bit.P1_23 && bankLevel[3]) || (FIO1PIN_bit.P1_23 && !bankLevel[3]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_bank[3];
        
        if (cr > (bank_pulse[3] - COIN_IMP_SPAN))
        {
            pend_bank_counter[3] = 1;
            pend_bank_timestamp[3] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_bank[3] = T3CR;
        pend_bank_counter[3] = 0;
      }
  }
  
  // банк 5
  if(TSTBIT(input_event, 24))
  {
    if ((!FIO0PIN_bit.P0_6 && bankLevel[4]) || (FIO0PIN_bit.P0_6 && !bankLevel[4]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_bank[4];
        
        if (cr > (bank_pulse[4] - COIN_IMP_SPAN))
        {
            pend_bank_counter[4] = 1;
            pend_bank_timestamp[4] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_bank[4] = T3CR;
        pend_bank_counter[4] = 0;
      }
  }
  
  // банк 6
  if(TSTBIT(input_event, 25))
  {
    if ((!FIO0PIN_bit.P0_10 && bankLevel[5]) || (FIO0PIN_bit.P0_10 && !bankLevel[5]))
      { // пришел задний фронт
        CPU_INT32U cr=T3CR;
        cr -= period_bank[5];
        
        if (cr > (bank_pulse[5] - COIN_IMP_SPAN))
        {
            pend_bank_counter[5] = 1;
            pend_bank_timestamp[5] = OSTimeGet();
        }
      }
    else
      { // пришел передний фронт
        period_bank[5] = T3CR;
        pend_bank_counter[5] = 0;
      }
  }
}

extern CPU_INT32U  BSP_CPU_PclkFreq (CPU_INT08U  pclk);

void InitInputPorts()
{
    // купюроприемник 1
    PINSEL3_bit.P1_20 = 0;
    if(cashLevel[0])PINMODE3_bit.P1_20 = 3;
    else PINMODE3_bit.P1_20 = 0;
    FIO1DIR_bit.P1_20  = 0;
    FIO1MASK_bit.P1_20 = 0;
    
    // монетоприемник 1
    PINSEL3_bit.P1_21 = 0;
    if(coinLevel[0])PINMODE3_bit.P1_21 = 3;
    else PINMODE3_bit.P1_21 = 0;
    FIO1DIR_bit.P1_21  = 0;
    FIO1MASK_bit.P1_21 = 0;
  
    // сигнал печати чека пост 1
    PINSEL9_bit.P4_28 = 0;
    if(SignalLevel[0])PINMODE9_bit.P4_28 = 3;
    else PINMODE9_bit.P4_28 = 0;
    FIO4DIR_bit.P4_28  = 0;
    FIO4MASK_bit.P4_28 = 0;

    // купюроприемник 2
    PINSEL3_bit.P1_19 = 0;
    if(cashLevel[1])PINMODE3_bit.P1_19 = 3;
    else PINMODE3_bit.P1_19 = 0;
    FIO1DIR_bit.P1_19  = 0;
    FIO1MASK_bit.P1_19 = 0;
    
    // монетоприемник 2
    PINSEL3_bit.P1_18 = 0;
    if(coinLevel[1])PINMODE3_bit.P1_18 = 3;
    else PINMODE3_bit.P1_18 = 0;
    FIO1DIR_bit.P1_18  = 0;
    FIO1MASK_bit.P1_18 = 0;
  
    // сигнал печати чека пост 2
    PINSEL0_bit.P0_4 = 0;
    if(SignalLevel[1])PINMODE0_bit.P0_4 = 3;
    else PINMODE0_bit.P0_4 = 0;
    FIO0DIR_bit.P0_4  = 0;
    FIO0MASK_bit.P0_4 = 0;
    
    // купюроприемник 3
    PINSEL7_bit.P3_25 = 0;
    if(cashLevel[2])PINMODE7_bit.P3_25 = 3;
    else PINMODE7_bit.P3_25 = 0;
    FIO3DIR_bit.P3_25  = 0;
    FIO3MASK_bit.P3_25 = 0;
    
    // монетоприемник 3
    PINSEL7_bit.P3_26 = 0;
    if(coinLevel[2])PINMODE7_bit.P3_26 = 3;
    else PINMODE7_bit.P3_26 = 0;
    FIO3DIR_bit.P3_26  = 0;
    FIO3MASK_bit.P3_26 = 0;
  
    // сигнал печати чека пост 3
    PINSEL3_bit.P1_28 = 0;
    if(SignalLevel[2])PINMODE3_bit.P1_28 = 3;
    else PINMODE3_bit.P1_28 = 0;
    FIO1DIR_bit.P1_28  = 0;
    FIO1MASK_bit.P1_28 = 0;
    
    // купюроприемник 4
    PINSEL1_bit.P0_26 = 0;
    if(cashLevel[3])PINMODE1_bit.P0_26 = 3;
    else PINMODE1_bit.P0_26 = 0;
    FIO0DIR_bit.P0_26  = 0;
    FIO0MASK_bit.P0_26 = 0;
    
    // монетоприемник 4
    PINSEL1_bit.P0_25 = 0;
    if(coinLevel[3])PINMODE1_bit.P0_25 = 3;
    else PINMODE1_bit.P0_25 = 0;
    FIO0DIR_bit.P0_25  = 0;
    FIO0MASK_bit.P0_25 = 0;
  
    // сигнал печати чека пост 4
    PINSEL3_bit.P1_27 = 0;
    PINMODE3_bit.P1_27 = 0;
    if(SignalLevel[3])PINMODE3_bit.P1_27 = 3;
    else PINMODE3_bit.P1_27 = 0;
    FIO1DIR_bit.P1_27  = 0;
    FIO1MASK_bit.P1_27 = 0;
    
    // купюроприемник 5
    PINSEL0_bit.P0_9 = 0;
    if(cashLevel[4])PINMODE0_bit.P0_9 = 3;
    else PINMODE0_bit.P0_9 = 0;
    FIO0DIR_bit.P0_9  = 0;
    FIO0MASK_bit.P0_9 = 0;
    
    // монетоприемник 5
    PINSEL4_bit.P2_2 = 0;
    if(coinLevel[4])PINMODE3_bit.P1_21 = 3;
    else PINMODE4_bit.P2_2 = 0;
    FIO2DIR_bit.P2_2  = 0;
    FIO2MASK_bit.P2_2 = 0;
  
    // сигнал печати чека пост 5
    PINSEL3_bit.P1_26 = 0;
    if(SignalLevel[4])PINMODE3_bit.P1_26 = 3;
    else PINMODE3_bit.P1_26 = 0;
    FIO1DIR_bit.P1_26  = 0;
    FIO1MASK_bit.P1_26 = 0;
    
    // купюроприемник 6
    PINSEL0_bit.P0_7 = 0;
    if(cashLevel[5])PINMODE0_bit.P0_7 = 3;
    else PINMODE0_bit.P0_7 = 0;
    FIO0DIR_bit.P0_7  = 0;
    FIO0MASK_bit.P0_7 = 0;
    
    // монетоприемник 6
    PINSEL0_bit.P0_8 = 0;
    if(coinLevel[5])PINMODE0_bit.P0_8 = 3;
    else PINMODE0_bit.P0_8 = 0;
    FIO0DIR_bit.P0_8  = 0;
    FIO0MASK_bit.P0_8 = 0;
  
    // сигнал печати чека пост 6
    PINSEL0_bit.P0_0 = 0;
    if(SignalLevel[5])PINMODE0_bit.P0_0 = 3;
    else PINMODE0_bit.P0_0 = 0;
    FIO0DIR_bit.P0_0  = 0;
    FIO0MASK_bit.P0_0 = 0;

    // монетоприемник пылесос 1
    PINSEL0_bit.P0_5 = 0;
    if(coinLevel[6])PINMODE0_bit.P0_5 = 3;
    else PINMODE0_bit.P0_5 = 0;
    FIO0DIR_bit.P0_5  = 0;
    FIO0MASK_bit.P0_5 = 0;
      
    // монетоприемник пылесос 2
    PINSEL3_bit.P1_25 = 0;
    if(coinLevel[7])PINMODE3_bit.P1_25 = 3;
    else PINMODE3_bit.P1_25 = 0;
    FIO1DIR_bit.P1_25  = 0;
    FIO1MASK_bit.P1_25 = 0;
  
    // банк 1
    PINSEL1_bit.P0_28 = 0;
    if(bankLevel[0])PINMODE1_bit.P0_28 = 3;
    else PINMODE1_bit.P0_28 = 0;
    FIO0DIR_bit.P0_28  = 0;
    FIO0MASK_bit.P0_28 = 0;

    // банк 2
    PINSEL1_bit.P0_27 = 0;
    if(bankLevel[1])PINMODE1_bit.P0_27 = 3;
    else PINMODE1_bit.P0_27 = 0;
    FIO0DIR_bit.P0_27  = 0;
    FIO0MASK_bit.P0_27 = 0;

    // банк 3
    PINSEL3_bit.P1_24 = 0;
    if(bankLevel[2])PINMODE1_bit.P0_24 = 3;
    else PINMODE1_bit.P0_24 = 0;
    FIO1DIR_bit.P1_24  = 0;
    FIO1MASK_bit.P1_24 = 0;

    // банк 4
    PINSEL3_bit.P1_23 = 0;
    if(bankLevel[3])PINMODE1_bit.P0_23 = 3;
    else PINMODE1_bit.P0_23 = 0;
    FIO1DIR_bit.P1_23  = 0;
    FIO1MASK_bit.P1_23 = 0;

    // банк 5
    PINSEL0_bit.P0_6 = 0;
    if(bankLevel[4])PINMODE0_bit.P0_6 = 3;
    else PINMODE0_bit.P0_6 = 0;
    FIO0DIR_bit.P0_6  = 0;
    FIO0MASK_bit.P0_6 = 0;

    // банк 6
    PINSEL0_bit.P0_10 = 0;
    if(bankLevel[5])PINMODE0_bit.P0_10 = 3;
    else PINMODE0_bit.P0_10 = 0;
    FIO0DIR_bit.P0_10  = 0;
    FIO0MASK_bit.P0_10 = 0;
}

// инициализация импульсных входов
void  InitImpInput (void)
{
    #define INPUT_CAPTURE_FREQ  1000  // частота тактирования частотных входов
  
    CPU_INT32U  pclk_freq;
    CPU_INT32U  rld_cnts;

    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr = 0;
    #endif

    OnChangeCoinPulseLen();
    OnChangeCashPulseLen();
    OnChangeSinalPulseLen();
    OnChangeBankPulseLen();
    OnChangeLevelWithoutInit();
     
    OS_ENTER_CRITICAL();
    
    // назначим все ножки
    InitInputPorts();
 
    PCONP_bit.PCTIM3 = 1;
    PCLKSEL1_bit.PCLK_TIMER3 = 2;

    pclk_freq         =   BSP_CPU_PclkFreq(23);
    rld_cnts          =   pclk_freq / INPUT_CAPTURE_FREQ / 2;

    T3CTCR_bit.CTM    =   0;
    T3CTCR_bit.CIS    =   0;          // select CAP3.0 input
    T3PR              =   rld_cnts-1;
    
    T3MR0             =   1;
    T3MCR             =   3;
    
    T3CCR             =   0x00;
    T3EMR             =   0;
    T3TCR             =   0x03;
    T3TCR             =   0x01;
    
    VICINTSELECT     &= ~(1 << VIC_TIMER3);
    VICVECTADDR27     =  (CPU_INT32U)InputCapture_ISR;
    VICVECTPRIORITY27 =  10;                       
    VICINTENABLE      =  (1 << VIC_TIMER3);       

    T3IR = 0xFF;
    
    prev_input = curr_input = input_register();

    OS_EXIT_CRITICAL();
}

