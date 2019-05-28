#include <includes.h>
#include "keyboard.h"
#include "app_serv.h"


#define   KBRD_QUERY_LEN      4

OS_STK  KbrdTaskStk[KBRD_TASK_STK_SIZE];
OS_EVENT  *KbrdQuery = NULL;
void     *KbrdTbl[KBRD_QUERY_LEN];


void  KbrdTask(void *p_arg);

unsigned long kbrd_state;

void InitKbrd()
{
 // проинициализируем порты
/*
P2.3	MK_P24	«1»
P2.5	MK_P25	«2»
P2.11	MK_P26	«3»
P1.29	MK_P27	«a»
P2.13	MK_P32	«b»
P2.12	MK_P33	«c»
*/
    
  // подсветка кнопки OK
  //PINSEL3_bit.P1_21 = 0x0;
  //PINMODE3_bit.P1_21 = 0;
  //FIO1DIR_bit.P1_21 = 1;
  //FIO1MASK_bit.P1_21 = 0;
  //FIO1CLR_bit.P1_21 = 1;
  
  // сканирующие линии
  PINSEL4_bit.P2_3 = 0x0;
  PINSEL4_bit.P2_5 = 0x0;
  PINSEL4_bit.P2_11 = 0x0;

  PINMODE4_bit.P2_3 = 0;
  PINMODE4_bit.P2_5 = 0;
  PINMODE4_bit.P2_11 = 0;
    
  FIO2DIR_bit.P2_3 = 1;
  FIO2DIR_bit.P2_5 = 1;
  FIO2DIR_bit.P2_11 = 1;

  FIO2MASK_bit.P2_3 = 0;
  FIO2MASK_bit.P2_5 = 0;
  FIO2MASK_bit.P2_11 = 0;
  
  // входные линии
  PINSEL3_bit.P1_29 = 0x0;
  PINSEL4_bit.P2_13 = 0x0;
  PINSEL4_bit.P2_12 = 0x0;

  PINMODE3_bit.P1_29 = 0;
  PINMODE4_bit.P2_13 = 0;
  PINMODE4_bit.P2_12 = 0;
    
  FIO1DIR_bit.P1_29 = 0;
  FIO2DIR_bit.P2_13 = 0;
  FIO2DIR_bit.P2_12 = 0;

  FIO1MASK_bit.P1_29 = 0;
  FIO2MASK_bit.P2_13 = 0;
  FIO2MASK_bit.P2_12 = 0;

  // кнопка пользователя  
  //PINSEL3_bit.P1_20 = 0x0;
  //PINMODE3_bit.P1_20 = 0;
  //FIO1DIR_bit.P1_20= 0;
  //FIO1MASK_bit.P1_20 = 0;
  
  kbrd_state = 0;
  
  // создадим очередь событий клавиатуры и задачу опроса клавиатуры
  if (KbrdQuery == NULL)
    {
      KbrdQuery = OSQCreate(&KbrdTbl[0], KBRD_QUERY_LEN);
      OSTaskCreate(KbrdTask, (void *)0, (OS_STK *)&KbrdTaskStk[KBRD_TASK_STK_SIZE-1], KBRD_TASK_PRIO);
    }
}

#define KBRD_SCAN_LINE1()  {FIO2CLR_bit.P2_3 = 1; FIO2SET_bit.P2_5 = 1; FIO2SET_bit.P2_11 = 1;}
#define KBRD_SCAN_LINE2()  {FIO2SET_bit.P2_3 = 1; FIO2CLR_bit.P2_5 = 1; FIO2SET_bit.P2_11 = 1;}
#define KBRD_SCAN_LINE3()  {FIO2SET_bit.P2_3 = 1; FIO2SET_bit.P2_5 = 1; FIO2CLR_bit.P2_11 = 1;}

void  KbrdTask(void *p_arg)
{
  unsigned long prew_state = 0;
  
  while (1)
    {
      unsigned long state;
      
      OSTimeDly(17);

      state = 0;      
      KBRD_SCAN_LINE1();
      OSTimeDly(1);
      if (!FIO1PIN_bit.P1_29) state |= (1UL << KEY_F1);
      if (!FIO2PIN_bit.P2_13) state |= (1UL << KEY_F2);
      if (!FIO2PIN_bit.P2_12) state |= (1UL << KEY_F3);
      KBRD_SCAN_LINE2();
      OSTimeDly(1);
      if (!FIO1PIN_bit.P1_29) state |= (1UL << KEY_LEFT);
      if (!FIO2PIN_bit.P2_13) state |= (1UL << KEY_UP);
      if (!FIO2PIN_bit.P2_12) state |= (1UL << KEY_RIGHT);
      KBRD_SCAN_LINE3();
      OSTimeDly(1);
      if (!FIO1PIN_bit.P1_29) state |= (1UL << KEY_STOP);
      if (!FIO2PIN_bit.P2_13) state |= (1UL << KEY_DOWN);
      if (!FIO2PIN_bit.P2_12) state |= (1UL << KEY_START);

      //if (!FIO1PIN_bit.P1_20) state |=  (1UL << KEY_USER_START);

      if (prew_state == state)
        { // устранили дребезг
          if (kbrd_state ^ state)
            { // есть изменения нажатия
              for (unsigned long i = 0; i < KEY_COUNT; i++)
                {
                  // ловим передний фронт при нажатии
                  if (!(kbrd_state & (1UL << i)) && (state & (1UL << i)))
                    {
                      OSQPost(KbrdQuery, (void *)i);
                      PostUserEvent(EVENT_KEY_EMPTY+i); // пользователю тоже постим
                    }
                }
            }
          kbrd_state = state;
        }
      
      prew_state = state;
    }
    
}

int GetKbrdEvent(int* event)
{
  CPU_INT08U err = 0; 
  int evt  = (int)OSQPend(KbrdQuery, 1, &err);
  if (err != 0) return 0;
  *event = evt;
  return 1;  
}   
    