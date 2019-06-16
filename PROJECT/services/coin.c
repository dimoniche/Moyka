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

static CPU_INT32U cash_pulse[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U cash_pause[COUNT_POST + COUNT_VACUUM];

static char pend_cash_counter[COUNT_POST + COUNT_VACUUM];
static CPU_INT32U pend_cash_timestamp[COUNT_POST + COUNT_VACUUM];

static CPU_INT32U signal_pulse[COUNT_POST + COUNT_VACUUM];
static char pend_signal_counter[COUNT_POST + COUNT_VACUUM];

void SetCashPulseParam(CPU_INT32U pulse, CPU_INT32U pause, CPU_INT32U post)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  cash_pulse[post] = pulse * 1;
  cash_pause[post] = pause;
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

void CoinTask(void *p_arg)
{
  CPU_INT32U enable_coin[COUNT_POST + COUNT_VACUUM];
  CPU_INT32U cash_enable[COUNT_POST + COUNT_VACUUM];
  CPU_INT32U enable_signal[COUNT_POST + COUNT_VACUUM];

  CPU_INT32U last_cash_count[COUNT_POST + COUNT_VACUUM];
  CPU_INT32U last_cash_time[COUNT_POST + COUNT_VACUUM];
  CPU_INT32U last_settings_time = 0;

  while(1)
    {
        if (OSTimeGet() - last_settings_time > 1000)
        {
            for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
            {
              GetData(&EnableCoinDesc, &enable_coin[i], i, DATA_FLAG_DIRECT_INDEX);  
              GetData(&EnableValidatorDesc, &cash_enable[i], i, DATA_FLAG_DIRECT_INDEX);
              GetData(&EnableSignalDesc, &enable_signal[i], i, DATA_FLAG_DIRECT_INDEX);
            }
            
            last_settings_time = OSTimeGet();
        }

		for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
		{
			OSTimeDly(1);

			if (enable_coin[i])
			{
				if (GetCoinCount(i))
				{
					PostUserEvent(EVENT_COIN_INSERTED_POST1 + i);
				}
			}
			else
			{
				CoinDisable();
				GetResetCoinCount(i);
			}

			if (enable_signal[i])
			{
				if (pend_signal_counter[i])
				{
                  // ���� ��������� ������� ������
				  PostUserEvent(EVENT_CASH_PRINT_CHECK_POST1 + i);
                  pend_signal_counter[i] = 0;
				}
			}

			if (!cash_enable[i]) {GetResetCashCount(i); continue;}
            if(i >= COUNT_POST) continue;

			#if OS_CRITICAL_METHOD == 3
			OS_CPU_SR  cpu_sr = 0;
			#endif
			OS_ENTER_CRITICAL();
			
			if (pend_cash_counter[i])
			{
			  // �������� �������������� ������ ����� �������� �����
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
                  if (labs(OSTimeGet() - last_cash_time[i]) > 500)
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

// �������� ����� �����
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

// �������� ����� ����� � �������� �������
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

// �������� ����� ��������� �� ���������
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

// �������� ����� ��������� �� ��������� � �������� �������
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

CPU_INT32U period[COUNT_POST + COUNT_VACUUM];
CPU_INT32U period_cash[COUNT_POST + COUNT_VACUUM];
CPU_INT32U period_signal[COUNT_POST + COUNT_VACUUM];
CPU_INT32U T3CR = 0;

// ������������� ���������������
void InitCoin(void)
{
  for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
  {
    CoinImpCounter[i] = 0;
    CashImpCounter[i] = 0;
    
    cash_pulse[i] = 0;
    cash_pause[i] = 0;

    pend_cash_counter[i] = 0;
    pend_cash_timestamp[i] = 0;

    signal_pulse[i] = 0;
    pend_signal_counter[i] = 0;
    
    period[i] = 0;
    period_cash[i] = 0;
    period_signal[i] = 0;
  }
  
  InitImpInput();
  OSTaskCreate(CoinTask, (void *)0, (OS_STK *)&CoinTaskStk[COIN_TASK_STK_SIZE-1], COIN_TASK_PRIO);
}

CPU_INT32U input_register()
{
  CPU_INT32U input = 0xFFFFFFFF;
  
  if (!FIO1PIN_bit.P1_20)
  {
     CLRBIT(input, 0);
  }
  if (!FIO1PIN_bit.P1_21)
  {
     CLRBIT(input, 1);
  }
  if (!FIO4PIN_bit.P4_28)
  {
     CLRBIT(input, 2);
  }
  if (!FIO1PIN_bit.P1_19)
  {
     CLRBIT(input, 3);
  }
  if (!FIO1PIN_bit.P1_18)
  {
     CLRBIT(input, 4);
  }  
  if (!FIO0PIN_bit.P0_4)
  {
     CLRBIT(input, 5);
  }
  if (!FIO3PIN_bit.P3_25)
  {
     CLRBIT(input, 6);
  }
  if (!FIO3PIN_bit.P3_26)
  {
     CLRBIT(input, 7);
  }
  if (!FIO1PIN_bit.P1_28)
  {
     CLRBIT(input, 8);
  }
  if (!FIO0PIN_bit.P0_26)
  {
     CLRBIT(input, 9);
  }
  if (!FIO0PIN_bit.P0_25)
  {
     CLRBIT(input, 10);
  }
  if (!FIO1PIN_bit.P1_27)
  {
     CLRBIT(input, 11);
  }
  if (!FIO0PIN_bit.P0_9)
  {
     CLRBIT(input, 12);
  }
  if (!FIO2PIN_bit.P2_2)
  {
     CLRBIT(input, 13);
  }
  if (!FIO1PIN_bit.P1_26)
  {
     CLRBIT(input, 14);
  }
  if (!FIO0PIN_bit.P0_7)
  {
     CLRBIT(input, 15);
  }
  if (!FIO0PIN_bit.P0_8)
  {
     CLRBIT(input, 16);
  }
  if (!FIO0PIN_bit.P0_0)
  {
     CLRBIT(input, 17);
  }
  if (!FIO0PIN_bit.P0_5)
  {
     CLRBIT(input, 18);
  }
  if (!FIO0PIN_bit.P0_6)
  {
     CLRBIT(input, 19);
  }
  if (!FIO1PIN_bit.P1_25)
  {
     CLRBIT(input, 20);
  }
  if (!FIO0PIN_bit.P0_10)
  {
     CLRBIT(input, 21);
  }
  
  return input;
}

CPU_INT32U input_event = 0xFFFFFFFF;
CPU_INT32U prev_input = 0xFFFFFFFF;
CPU_INT32U curr_input = 0xFFFFFFFF;

void InputCapture_ISR(void)
{
  T3IR = 0xFF;

  // ���������� ����
  T3CR++;

  curr_input = input_register();
  input_event = curr_input^prev_input;
  prev_input = curr_input;

  // ���� 1
  // �������������� 1
  if(TSTBIT(input_event, 0))
  {
    if (FIO1PIN_bit.P1_20)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_cash[0];
        
        if (cr > (cash_pulse[0] - COIN_IMP_SPAN))
        {
            pend_cash_counter[0] = 1;
            pend_cash_timestamp[0] = OSTimeGet();
        }
      }
    else
      { // ������ �������� �����
        period_cash[0] = T3CR;
        pend_cash_counter[0] = 0;
      }
  }

  // �������������� 1
  if(TSTBIT(input_event, 1))
  {
    if (FIO1PIN_bit.P1_21)
      { // ������ ������ �����
        if ((T3CR-period[0]) > COIN_IMP_MIN_LEN)
          {
              CoinImpCounter[0]++;
          }
      }
    else
      { // ������ �������� �����
        period[0] = T3CR;
      }
  }
  
  // ������ ������ ���� 1
  if(TSTBIT(input_event, 2))
  {
    if (FIO4PIN_bit.P4_28)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_signal[0];
        
        if (cr > (signal_pulse[0] - COIN_IMP_SPAN))
        {
          pend_signal_counter[0] = 1;
        }
      }
    else
      { // ������ �������� �����
        period_signal[0] = T3CR;
        pend_signal_counter[0] = 0;
      }
  }
  
  // ���� 2
  // �������������� 2
  if(TSTBIT(input_event, 3))
  {
    if (FIO1PIN_bit.P1_19)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_cash[1];
        
        if (cr > (cash_pulse[1] - COIN_IMP_SPAN))
        {
            pend_cash_counter[1] = 1;
            pend_cash_timestamp[1] = OSTimeGet();
        }
      }
    else
      { // ������ �������� �����
        period_cash[1] = T3CR;
        pend_cash_counter[1] = 0;
      }
  }

  // �������������� 2
  if(TSTBIT(input_event, 4))
  {
    if (FIO1PIN_bit.P1_18)
      { // ������ ������ �����
        if ((T3CR-period[1]) > COIN_IMP_MIN_LEN)
          {
              CoinImpCounter[1]++;
          }
      }
    else
      { // ������ �������� �����
        period[1] = T3CR;
      }
  }
  
  // ������ ������ ���� 2
  if(TSTBIT(input_event, 5))
  {
    if (FIO0PIN_bit.P0_4)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_signal[1];
        
        if (cr > (signal_pulse[1] - COIN_IMP_SPAN))
        {
            pend_signal_counter[0] = 1;
        }
      }
    else
      { // ������ �������� �����
        period_signal[1] = T3CR;
        pend_signal_counter[1] = 0;
      }
  }

  // ���� 3
  // �������������� 3
  if(TSTBIT(input_event, 6))
  {
    if (FIO3PIN_bit.P3_25)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_cash[2];
        
        if (cr > (cash_pulse[2] - COIN_IMP_SPAN))
        {
            pend_cash_counter[2] = 1;
            pend_cash_timestamp[2] = OSTimeGet();
        }
      }
    else
      { // ������ �������� �����
        period_cash[2] = T3CR;
        pend_cash_counter[2] = 0;
      }
  }

  // �������������� 3
  if(TSTBIT(input_event, 7))
  {
    if (FIO3PIN_bit.P3_26)
      { // ������ ������ �����
        if ((T3CR-period[2]) > COIN_IMP_MIN_LEN)
          {
              CoinImpCounter[2]++;
          }
      }
    else
      { // ������ �������� �����
        period[2] = T3CR;
      }
  }
  
  // ������ ������ ���� 3
  if(TSTBIT(input_event, 8))
  {
    if (FIO1PIN_bit.P1_28)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_signal[2];
        
        if (cr > (signal_pulse[2] - COIN_IMP_SPAN))
        {
            pend_signal_counter[2] = 1;
        }
      }
    else
      { // ������ �������� �����
        period_signal[2] = T3CR;
        pend_signal_counter[2] = 0;
      }
  }

  // ���� 4
  // �������������� 4
  if(TSTBIT(input_event, 9))
  {
    if (FIO0PIN_bit.P0_26)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_cash[3];
        
        if (cr > (cash_pulse[3] - COIN_IMP_SPAN))
        {
            pend_cash_counter[3] = 1;
            pend_cash_timestamp[3] = OSTimeGet();
        }
      }
    else
      { // ������ �������� �����
        period_cash[3] = T3CR;
        pend_cash_counter[3] = 0;
      }
  }

  // �������������� 4
  if(TSTBIT(input_event, 10))
  {
    if (FIO0PIN_bit.P0_25)
      { // ������ ������ �����
        if ((T3CR-period[3]) > COIN_IMP_MIN_LEN)
          {
              CoinImpCounter[3]++;
          }
      }
    else
      { // ������ �������� �����
        period[3] = T3CR;
      }
  }
  
  // ������ ������ ���� 4
  if(TSTBIT(input_event, 11))
  {
    if (FIO1PIN_bit.P1_27)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_signal[3];
        
        if (cr > (signal_pulse[3] - COIN_IMP_SPAN))
        {
            pend_signal_counter[3] = 1;
        }
      }
    else
      { // ������ �������� �����
        period_signal[3] = T3CR;
        pend_signal_counter[3] = 0;
      }
  }

  // ���� 5
  // �������������� 5
  if(TSTBIT(input_event, 12))
  {
    if (FIO0PIN_bit.P0_9)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_cash[4];
        
        if (cr > (cash_pulse[4] - COIN_IMP_SPAN))
        {
            pend_cash_counter[4] = 1;
            pend_cash_timestamp[4] = OSTimeGet();
        }
      }
    else
      { // ������ �������� �����
        period_cash[4] = T3CR;
        pend_cash_counter[4] = 0;
      }
  }

  // �������������� 5
  if(TSTBIT(input_event, 13))
  {
    if (FIO2PIN_bit.P2_2)
      { // ������ ������ �����
        if ((T3CR-period[4]) > COIN_IMP_MIN_LEN)
          {
              CoinImpCounter[4]++;
          }
      }
    else
      { // ������ �������� �����
        period[4] = T3CR;
      }
  }
  
  // ������ ������ ���� 5
  if(TSTBIT(input_event, 14))
  {
    if (FIO1PIN_bit.P1_26)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_signal[4];
        
        if (cr > (signal_pulse[4] - COIN_IMP_SPAN))
        {
            pend_signal_counter[4] = 1;
        }
      }
    else
      { // ������ �������� �����
        period_signal[4] = T3CR;
        pend_signal_counter[4] = 0;
      }
  }

  // ���� 6
  // �������������� 6
  if(TSTBIT(input_event, 15))
  {
    if (FIO0PIN_bit.P0_7)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_cash[5];
        
        if (cr > (cash_pulse[5] - COIN_IMP_SPAN))
        {
            pend_cash_counter[5] = 1;
            pend_cash_timestamp[5] = OSTimeGet();
        }
      }
    else
      { // ������ �������� �����
        period_cash[5] = T3CR;
        pend_cash_counter[5] = 0;
      }
  }

  // �������������� 6
  if(TSTBIT(input_event, 16))
  {
    if (FIO0PIN_bit.P0_8)
      { // ������ ������ �����
        if ((T3CR-period[5]) > COIN_IMP_MIN_LEN)
          {
              CoinImpCounter[5]++;
          }
      }
    else
      { // ������ �������� �����
        period[5] = T3CR;
      }
  }
  
  // ������ ������ ���� 6
  if(TSTBIT(input_event, 17))
  {
    if (FIO0PIN_bit.P0_0)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_signal[5];
        
        if (cr > (signal_pulse[5] - COIN_IMP_SPAN))
        {
            pend_signal_counter[5] = 1;
        }
      }
    else
      { // ������ �������� �����
        period_signal[5] = T3CR;
        pend_signal_counter[5] = 0;
      }
  }

  // �������������� ������� 1
  if(TSTBIT(input_event, 18))
  {
    if (FIO0PIN_bit.P0_5)
      { // ������ ������ �����
        if ((T3CR-period[6]) > COIN_IMP_MIN_LEN)
          {
              CoinImpCounter[6]++;
          }
      }
    else
      { // ������ �������� �����
        period[6] = T3CR;
      }
  }
  
  // ������ ������ ���� ������� 1
  if(TSTBIT(input_event, 19))
  {
    if (FIO0PIN_bit.P0_6)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_signal[6];
        
        if (cr > (signal_pulse[6] - COIN_IMP_SPAN))
        {
            pend_signal_counter[6] = 1;
        }
      }
    else
      { // ������ �������� �����
        period_signal[6] = T3CR;
        pend_signal_counter[6] = 0;
      }
  }

  // �������������� ������� 2
  if(TSTBIT(input_event, 20))
  {
    if (FIO1PIN_bit.P1_25)
      { // ������ ������ �����
        if ((T3CR-period[7]) > COIN_IMP_MIN_LEN)
          {
              CoinImpCounter[7]++;
          }
      }
    else
      { // ������ �������� �����
        period[7] = T3CR;
      }
  }
  
  // ������ ������ ���� ������� 2
  if(TSTBIT(input_event, 21))
  {
    if (FIO0PIN_bit.P0_10)
      { // ������ ������ �����
        CPU_INT32U cr=T3CR;
        cr -= period_signal[7];
        
        if (cr > (signal_pulse[7] - COIN_IMP_SPAN))
        {
            pend_signal_counter[7] = 1;
        }
      }
    else
      { // ������ �������� �����
        period_signal[7] = T3CR;
        pend_signal_counter[7] = 0;
      }
  }
}

extern CPU_INT32U  BSP_CPU_PclkFreq (CPU_INT08U  pclk);

// ������������� ���������� ������
void  InitImpInput (void)
{
    #define INPUT_CAPTURE_FREQ  1000  // ������� ������������ ��������� ������
  
    CPU_INT32U  pclk_freq;
    CPU_INT32U  rld_cnts;

    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr = 0;
    #endif

    OnChangeCashPulseLen();
    OnChangeSinalPulseLen();
        
    OS_ENTER_CRITICAL();
    
    // �������� ��� �����
    
    // �������������� 1
    PINSEL3_bit.P1_20 = 0;
    PINMODE3_bit.P1_20 = 0;
    FIO1DIR_bit.P1_20  = 0;
    FIO1MASK_bit.P1_20 = 0;
    
    // �������������� 1
    PINSEL3_bit.P1_21 = 0;
    PINMODE3_bit.P1_21 = 0;
    FIO1DIR_bit.P1_21  = 0;
    FIO1MASK_bit.P1_21 = 0;
  
    // ������ ������ ���� ���� 1
    PINSEL9_bit.P4_28 = 0;
    PINMODE9_bit.P4_28 = 0;
    FIO4DIR_bit.P4_28  = 0;
    FIO4MASK_bit.P4_28 = 0;

    // �������������� 2
    PINSEL3_bit.P1_19 = 0;
    PINMODE3_bit.P1_19 = 0;
    FIO1DIR_bit.P1_19  = 0;
    FIO1MASK_bit.P1_19 = 0;
    
    // �������������� 2
    PINSEL3_bit.P1_18 = 0;
    PINMODE3_bit.P1_18 = 0;
    FIO1DIR_bit.P1_18  = 0;
    FIO1MASK_bit.P1_18 = 0;
  
    // ������ ������ ���� ���� 2
    PINSEL0_bit.P0_4 = 0;
    PINMODE0_bit.P0_4 = 0;
    FIO0DIR_bit.P0_4  = 0;
    FIO0MASK_bit.P0_4 = 0;
    
    // �������������� 3
    PINSEL7_bit.P3_25 = 0;
    PINMODE7_bit.P3_25 = 0;
    FIO3DIR_bit.P3_25  = 0;
    FIO3MASK_bit.P3_25 = 0;
    
    // �������������� 3
    PINSEL7_bit.P3_26 = 0;
    PINMODE7_bit.P3_26 = 0;
    FIO3DIR_bit.P3_26  = 0;
    FIO3MASK_bit.P3_26 = 0;
  
    // ������ ������ ���� ���� 3
    PINSEL3_bit.P1_28 = 0;
    PINMODE3_bit.P1_28 = 0;
    FIO1DIR_bit.P1_28  = 0;
    FIO1MASK_bit.P1_28 = 0;
    
    // �������������� 4
    PINSEL1_bit.P0_26 = 0;
    PINMODE1_bit.P0_26 = 0;
    FIO0DIR_bit.P0_26  = 0;
    FIO0MASK_bit.P0_26 = 0;
    
    // �������������� 4
    PINSEL1_bit.P0_25 = 0;
    PINMODE1_bit.P0_25 = 0;
    FIO0DIR_bit.P0_25  = 0;
    FIO0MASK_bit.P0_25 = 0;
  
    // ������ ������ ���� ���� 4
    PINSEL3_bit.P1_27 = 0;
    PINMODE3_bit.P1_27 = 0;
    FIO1DIR_bit.P1_27  = 0;
    FIO1MASK_bit.P1_27 = 0;
    
    // �������������� 5
    PINSEL0_bit.P0_9 = 0;
    PINMODE0_bit.P0_9 = 0;
    FIO0DIR_bit.P0_9  = 0;
    FIO0MASK_bit.P0_9 = 0;
    
    // �������������� 5
    PINSEL4_bit.P2_2 = 0;
    PINMODE4_bit.P2_2 = 0;
    FIO2DIR_bit.P2_2  = 0;
    FIO2MASK_bit.P2_2 = 0;
  
    // ������ ������ ���� ���� 5
    PINSEL3_bit.P1_26 = 0;
    PINMODE3_bit.P1_26 = 0;
    FIO1DIR_bit.P1_26  = 0;
    FIO1MASK_bit.P1_26 = 0;
    
    // �������������� 6
    PINSEL0_bit.P0_7 = 0;
    PINMODE0_bit.P0_7 = 0;
    FIO0DIR_bit.P0_7  = 0;
    FIO0MASK_bit.P0_7 = 0;
    
    // �������������� 6
    PINSEL0_bit.P0_8 = 0;
    PINMODE0_bit.P0_8 = 0;
    FIO0DIR_bit.P0_8  = 0;
    FIO0MASK_bit.P0_8 = 0;
  
    // ������ ������ ���� ���� 6
    PINSEL0_bit.P0_0 = 0;
    PINMODE0_bit.P0_0 = 0;
    FIO0DIR_bit.P0_0  = 0;
    FIO0MASK_bit.P0_0 = 0;

    // �������������� ������� 1
    PINSEL0_bit.P0_5 = 0;
    PINMODE0_bit.P0_5 = 0;
    FIO0DIR_bit.P0_5  = 0;
    FIO0MASK_bit.P0_5 = 0;
  
    // ������ ������ ���� ������� 1
    PINSEL0_bit.P0_6 = 0;
    PINMODE0_bit.P0_6 = 0;
    FIO0DIR_bit.P0_6  = 0;
    FIO0MASK_bit.P0_6 = 0;
    
    // �������������� ������� 2
    PINSEL3_bit.P1_25 = 0;
    PINMODE3_bit.P1_25 = 0;
    FIO1DIR_bit.P1_25  = 0;
    FIO1MASK_bit.P1_25 = 0;
  
    // ������ ������ ���� ������� 2
    PINSEL0_bit.P0_10 = 0;
    PINMODE0_bit.P0_10 = 0;
    FIO0DIR_bit.P0_10  = 0;
    FIO0MASK_bit.P0_10 = 0;
    
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
    
    OS_EXIT_CRITICAL();
}

