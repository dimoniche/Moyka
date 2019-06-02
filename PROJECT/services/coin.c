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
CPU_INT32U  CashImpCounter[COUNT_POST];

static CPU_INT32U cash_pulse = 5000;
static CPU_INT32U cash_pause = 2000;

static char pend_cash_counter[COUNT_POST];
static CPU_INT32U pend_cash_timestamp[COUNT_POST];

void SetCashPulseParam(CPU_INT32U pulse, CPU_INT32U pause)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  cash_pulse = pulse * 100;
  cash_pause = pause;
  OS_EXIT_CRITICAL();
}

void CoinTask(void *p_arg)
{
  CPU_INT32U enable_coin;
  CPU_INT32U cash_mode;
  CPU_INT32U cash_enable;
  CPU_INT32U last_cash_count[COUNT_POST];
  CPU_INT32U last_cash_time[COUNT_POST];
  CPU_INT32U last_settings_time = 0;

  while(1)
    {
		for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
		{
			if (OSTimeGet() - last_settings_time > 1000)
			{
				last_settings_time = OSTimeGet();
				GetData(&EnableCoinDesc, &enable_coin, i, DATA_FLAG_DIRECT_INDEX);  
				GetData(&CashModeDesc, &cash_mode, i, DATA_FLAG_DIRECT_INDEX);  
				GetData(&EnableValidatorDesc, &cash_enable, i, DATA_FLAG_DIRECT_INDEX);
			}

			OSTimeDly(1);

			if (enable_coin)
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

			if (!cash_enable) {GetResetCashCount(i); continue;}
            if(i >= COUNT_POST) continue;

			#if OS_CRITICAL_METHOD == 3
			OS_CPU_SR  cpu_sr = 0;
			#endif
			OS_ENTER_CRITICAL();
			
			if (pend_cash_counter[i])
			{
			  // �������� �������������� ������ ����� �������� �����
			  if (OSTimeGet() - pend_cash_timestamp[i] > cash_pause)
			  {
				pend_cash_counter[i] = 0;
				CashImpCounter[i]++;
			  }
			}
			OS_EXIT_CRITICAL();
				
			if (cash_mode == 1)
			{
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
			else
			{
			  GetResetCashCount(i);
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

// ������������� ���������������
void InitCoin(void)
{
  for(int i = 0; i < COUNT_POST; i++)
  {
    CoinImpCounter[i] = 0;
    CashImpCounter[i] = 0;

    pend_cash_counter[i] = 0;
  }
  
  InitImpInput();
  OSTaskCreate(CoinTask, (void *)0, (OS_STK *)&CoinTaskStk[COIN_TASK_STK_SIZE-1], COIN_TASK_PRIO);
}

void InputCapture_ISR(void)
{
  static CPU_INT32U period[COUNT_POST + 2];
  static CPU_INT32U period_cash[COUNT_POST];
  static CPU_INT32U T3CR = 0;

  // ���������� ����
  T3CR++;
  
  // ���� 1
  // �������������� 1
  if (FIO1PIN_bit.P1_20)
	{ // ������ ������ �����
	  CPU_INT32U cr=T3CR;
	  cr -= period_cash[0];
	  
	  if ((cr > (cash_pulse - COIN_IMP_SPAN))
		   &&  (cr < (cash_pulse + COIN_IMP_SPAN)))
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

  // �������������� 1
  if (FIO1PIN_bit.P1_21)
	{ // ������ ������ �����
	  if (((T3CR-period[0]) > COIN_IMP_MIN_LEN)
	  &&  ((T3CR-period[0]) < COIN_IMP_MAX_LEN))
		{
			CoinImpCounter[0]++;
		}
	}
  else
	{ // ������ �������� �����
	  period[0] = T3CR;
	}
  
  // ������ ������ ���� 1
  if (FIO4PIN_bit.P4_28)
  {
  }
  
  // ���� 2
  // �������������� 2
  if (FIO1PIN_bit.P1_19)
	{ // ������ ������ �����
	  CPU_INT32U cr=T3CR;
	  cr -= period_cash[1];
	  
	  if ((cr > (cash_pulse - COIN_IMP_SPAN))
		   &&  (cr < (cash_pulse + COIN_IMP_SPAN)))
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

  // �������������� 2
  if (FIO1PIN_bit.P1_18)
	{ // ������ ������ �����
	  if (((T3CR-period[1]) > COIN_IMP_MIN_LEN)
	  &&  ((T3CR-period[1]) < COIN_IMP_MAX_LEN))
		{
			CoinImpCounter[1]++;
		}
	}
  else
	{ // ������ �������� �����
	  period[1] = T3CR;
	}
  
  // ������ ������ ���� 2
  if (FIO0PIN_bit.P0_4)
  {
  }
  
  // ���� 3
  // �������������� 3
  if (FIO3PIN_bit.P3_25)
	{ // ������ ������ �����
	  CPU_INT32U cr=T3CR;
	  cr -= period_cash[2];
	  
	  if ((cr > (cash_pulse - COIN_IMP_SPAN))
		   &&  (cr < (cash_pulse + COIN_IMP_SPAN)))
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

  // �������������� 3
  if (FIO3PIN_bit.P3_26)
	{ // ������ ������ �����
	  if (((T3CR-period[2]) > COIN_IMP_MIN_LEN)
	  &&  ((T3CR-period[2]) < COIN_IMP_MAX_LEN))
		{
			CoinImpCounter[2]++;
		}
	}
  else
	{ // ������ �������� �����
	  period[2] = T3CR;
	}
  
  // ������ ������ ���� 3
  if (FIO1PIN_bit.P1_28)
  {
  }
  
  // ���� 4
  // �������������� 4
  if (FIO0PIN_bit.P0_26)
	{ // ������ ������ �����
	  CPU_INT32U cr=T3CR;
	  cr -= period_cash[3];
	  
	  if ((cr > (cash_pulse - COIN_IMP_SPAN))
		   &&  (cr < (cash_pulse + COIN_IMP_SPAN)))
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

  // �������������� 4
  if (FIO0PIN_bit.P0_25)
	{ // ������ ������ �����
	  if (((T3CR-period[3]) > COIN_IMP_MIN_LEN)
	  &&  ((T3CR-period[3]) < COIN_IMP_MAX_LEN))
		{
			CoinImpCounter[3]++;
		}
	}
  else
	{ // ������ �������� �����
	  period[3] = T3CR;
	}
  
  // ������ ������ ���� 4
  if (FIO1PIN_bit.P1_27)
  {
  }
  
  // ���� 5
  // �������������� 5
  if (FIO0PIN_bit.P0_9)
	{ // ������ ������ �����
	  CPU_INT32U cr=T3CR;
	  cr -= period_cash[4];
	  
	  if ((cr > (cash_pulse - COIN_IMP_SPAN))
		   &&  (cr < (cash_pulse + COIN_IMP_SPAN)))
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

  // �������������� 5
  if (FIO2PIN_bit.P2_2)
	{ // ������ ������ �����
	  if (((T3CR-period[4]) > COIN_IMP_MIN_LEN)
	  &&  ((T3CR-period[4]) < COIN_IMP_MAX_LEN))
		{
			CoinImpCounter[4]++;
		}
	}
  else
	{ // ������ �������� �����
	  period[4] = T3CR;
	}
  
  // ������ ������ ���� 5
  if (FIO1PIN_bit.P1_26)
  {
  }
  
  // ���� 6
  // �������������� 6
  if (FIO0PIN_bit.P0_7)
	{ // ������ ������ �����
	  CPU_INT32U cr=T3CR;
	  cr -= period_cash[5];
	  
	  if ((cr > (cash_pulse - COIN_IMP_SPAN))
		   &&  (cr < (cash_pulse + COIN_IMP_SPAN)))
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

  // �������������� 6
  if (FIO0PIN_bit.P0_8)
	{ // ������ ������ �����
	  if (((T3CR-period[5]) > COIN_IMP_MIN_LEN)
	  &&  ((T3CR-period[5]) < COIN_IMP_MAX_LEN))
		{
			CoinImpCounter[5]++;
		}
	}
  else
	{ // ������ �������� �����
	  period[5] = T3CR;
	}
  
  // ������ ������ ���� 5
  if (FIO0PIN_bit.P0_0)
  {
  }

  // �������������� ������� 1
  if (FIO0PIN_bit.P0_5)
	{ // ������ ������ �����
	  if (((T3CR-period[6]) > COIN_IMP_MIN_LEN)
	  &&  ((T3CR-period[6]) < COIN_IMP_MAX_LEN))
		{
			CoinImpCounter[6]++;
		}
	}
  else
	{ // ������ �������� �����
	  period[6] = T3CR;
	}
  
  // ������ ������ ���� ������� 1
  if (FIO0PIN_bit.P0_6)
  {
  }

  // �������������� ������� 2
  if (FIO1PIN_bit.P1_25)
	{ // ������ ������ �����
	  if (((T3CR-period[7]) > COIN_IMP_MIN_LEN)
	  &&  ((T3CR-period[7]) < COIN_IMP_MAX_LEN))
		{
			CoinImpCounter[7]++;
		}
	}
  else
	{ // ������ �������� �����
	  period[7] = T3CR;
	}
  
  // ������ ������ ���� ������� 2
  if (FIO0PIN_bit.P0_10)
  {
  }
  
}

extern CPU_INT32U  BSP_CPU_PclkFreq (CPU_INT08U  pclk);

// ������������� ���������� ������
void  InitImpInput (void)
{
    #define INPUT_CAPTURE_FREQ  100000  // ������� ������������ ��������� ������
  
    CPU_INT32U  pclk_freq;
    CPU_INT32U  rld_cnts;

    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr = 0;
    #endif

    OnChangeCashPulseLen();
        
    OS_ENTER_CRITICAL();

    PCONP_bit.PCTIM3 = 1;
    PCLKSEL1_bit.PCLK_TIMER3 = 2;
    
    // �������� ��� �����
    
    // �������������� 1
    PINSEL3_bit.P1_20 = 0x3;
    PINMODE3_bit.P1_20 = 0;
    FIO1DIR_bit.P1_20  = 0;
    FIO1MASK_bit.P1_20 = 0;
    
    // �������������� 1
    PINSEL3_bit.P1_21 = 0x3;
    PINMODE3_bit.P1_21 = 0;
    FIO1DIR_bit.P1_21  = 0;
    FIO1MASK_bit.P1_21 = 0;
  
    // ������ ������ ���� ���� 1
    PINSEL9_bit.P4_28 = 0x3;
    PINMODE9_bit.P4_28 = 0;
    FIO4DIR_bit.P4_28  = 0;
    FIO4MASK_bit.P4_28 = 0;

    // �������������� 2
    PINSEL3_bit.P1_19 = 0x3;
    PINMODE3_bit.P1_19 = 0;
    FIO1DIR_bit.P1_19  = 0;
    FIO1MASK_bit.P1_19 = 0;
    
    // �������������� 2
    PINSEL3_bit.P1_18 = 0x3;
    PINMODE3_bit.P1_18 = 0;
    FIO1DIR_bit.P1_18  = 0;
    FIO1MASK_bit.P1_18 = 0;
  
    // ������ ������ ���� ���� 2
    PINSEL0_bit.P0_4 = 0x3;
    PINMODE0_bit.P0_4 = 0;
    FIO0DIR_bit.P0_4  = 0;
    FIO0MASK_bit.P0_4 = 0;
    
    // �������������� 3
    PINSEL7_bit.P3_25 = 0x3;
    PINMODE7_bit.P3_25 = 0;
    FIO3DIR_bit.P3_25  = 0;
    FIO3MASK_bit.P3_25 = 0;
    
    // �������������� 3
    PINSEL7_bit.P3_26 = 0x3;
    PINMODE7_bit.P3_26 = 0;
    FIO3DIR_bit.P3_26  = 0;
    FIO3MASK_bit.P3_26 = 0;
  
    // ������ ������ ���� ���� 3
    PINSEL3_bit.P1_28 = 0x3;
    PINMODE3_bit.P1_28 = 0;
    FIO1DIR_bit.P1_28  = 0;
    FIO1MASK_bit.P1_28 = 0;
    
    // �������������� 4
    PINSEL1_bit.P0_26 = 0x3;
    PINMODE1_bit.P0_26 = 0;
    FIO0DIR_bit.P0_26  = 0;
    FIO0MASK_bit.P0_26 = 0;
    
    // �������������� 4
    PINSEL1_bit.P0_25 = 0x3;
    PINMODE1_bit.P0_25 = 0;
    FIO0DIR_bit.P0_25  = 0;
    FIO0MASK_bit.P0_25 = 0;
  
    // ������ ������ ���� ���� 4
    PINSEL3_bit.P1_27 = 0x3;
    PINMODE3_bit.P1_27 = 0;
    FIO1DIR_bit.P1_27  = 0;
    FIO1MASK_bit.P1_27 = 0;
    
    // �������������� 5
    PINSEL0_bit.P0_9 = 0x3;
    PINMODE0_bit.P0_9 = 0;
    FIO0DIR_bit.P0_9  = 0;
    FIO0MASK_bit.P0_9 = 0;
    
    // �������������� 5
    PINSEL4_bit.P2_2 = 0x3;
    PINMODE4_bit.P2_2 = 0;
    FIO2DIR_bit.P2_2  = 0;
    FIO2MASK_bit.P2_2 = 0;
  
    // ������ ������ ���� ���� 5
    PINSEL3_bit.P1_26 = 0x3;
    PINMODE3_bit.P1_26 = 0;
    FIO1DIR_bit.P1_26  = 0;
    FIO1MASK_bit.P1_26 = 0;
    
    // �������������� 6
    PINSEL0_bit.P0_7 = 0x3;
    PINMODE0_bit.P0_7 = 0;
    FIO0DIR_bit.P0_7  = 0;
    FIO0MASK_bit.P0_7 = 0;
    
    // �������������� 6
    PINSEL0_bit.P0_8 = 0x3;
    PINMODE0_bit.P0_8 = 0;
    FIO0DIR_bit.P0_8  = 0;
    FIO0MASK_bit.P0_8 = 0;
  
    // ������ ������ ���� ���� 6
    PINSEL0_bit.P0_0 = 0x3;
    PINMODE0_bit.P0_0 = 0;
    FIO0DIR_bit.P0_0  = 0;
    FIO0MASK_bit.P0_0 = 0;

    // �������������� ������� 1
    PINSEL0_bit.P0_5 = 0x3;
    PINMODE0_bit.P0_5 = 0;
    FIO0DIR_bit.P0_5  = 0;
    FIO0MASK_bit.P0_5 = 0;
  
    // ������ ������ ���� ������� 1
    PINSEL0_bit.P0_6 = 0x3;
    PINMODE0_bit.P0_6 = 0;
    FIO0DIR_bit.P0_6  = 0;
    FIO0MASK_bit.P0_6 = 0;
    
    // �������������� ������� 2
    PINSEL3_bit.P1_25 = 0x3;
    PINMODE3_bit.P1_25 = 0;
    FIO1DIR_bit.P1_25  = 0;
    FIO1MASK_bit.P1_25 = 0;
  
    // ������ ������ ���� ������� 2
    PINSEL0_bit.P0_10 = 0x3;
    PINMODE0_bit.P0_10 = 0;
    FIO0DIR_bit.P0_10  = 0;
    FIO0MASK_bit.P0_10 = 0;
    
    pclk_freq         =   BSP_CPU_PclkFreq(23);
    rld_cnts          =   pclk_freq / INPUT_CAPTURE_FREQ;

    T3CTCR_bit.CTM    =   0;
    T3CTCR_bit.CIS    =   0;          // select CAP3.0 input
    T3PR              =   rld_cnts-1;
    T3MCR             =   0;
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

