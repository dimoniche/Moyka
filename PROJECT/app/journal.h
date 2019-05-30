#ifndef _JOURNAL_H_
#define _JOURNAL_H_

#include "control.h"
#include "fiscal.h"

#define EVENT_RECORDS_COUNT  512 // число записей в журнале

#pragma pack(push, 1)
/// структура записи журнала рабочих событий
typedef struct{
  // время создания записи
  CPU_INT32U time;
  
  // тип события
  CPU_INT08U event;
    #define JOURNAL_EVENT_NO_EVENT         0  // нет события
  
    #define JOURNAL_EVENT_MONEY_NOTE_POST1 1  // событие получения купюры пост 1
    #define JOURNAL_EVENT_MONEY_NOTE_POST2 2  // событие получения купюры пост 2
    #define JOURNAL_EVENT_MONEY_NOTE_POST3 3  // событие получения купюры пост 3
    #define JOURNAL_EVENT_MONEY_NOTE_POST4 4  // событие получения купюры пост 4
    #define JOURNAL_EVENT_MONEY_NOTE_POST5 5  // событие получения купюры пост 5
    #define JOURNAL_EVENT_MONEY_NOTE_POST6 6  // событие получения купюры пост 6
  
    #define JOURNAL_EVENT_MONEY_COIN_POST1 7  // событие получения монеты (кол-во рублей) пост 1
    #define JOURNAL_EVENT_MONEY_COIN_POST2 8  // событие получения монеты (кол-во рублей) пост 2
    #define JOURNAL_EVENT_MONEY_COIN_POST3 9  // событие получения монеты (кол-во рублей) пост 3
    #define JOURNAL_EVENT_MONEY_COIN_POST4 10  // событие получения монеты (кол-во рублей) пост 4
    #define JOURNAL_EVENT_MONEY_COIN_POST5 11  // событие получения монеты (кол-во рублей) пост 5
    #define JOURNAL_EVENT_MONEY_COIN_POST6 12  // событие получения монеты (кол-во рублей) пост 6

    #define JOURNAL_EVENT_START_SESSION    13  // событие начала сеанса
    #define JOURNAL_EVENT_END_SESSION      14  // событие конца сеанса
    #define JOURNAL_EVENT_DEVICE_ON        16  // включение устройства
    #define JOURNAL_EVENT_PRINT_BILL       17  // печать чека
    #define JOURNAL_EVENT_PRINT_Z          18  // печать z-отчета
    #define JOURNAL_EVENT_PRINT_X          19  // печать x-отчета
    #define JOURNAL_EVENT_PRINT_BUF        20  // печать x-отчета
    #define JOURNAL_EVENT_CHANGE_MODE      21  // смена режима
    #define JOURNAL_EVENT_INCASSATION      22  // инкассация 
    #define JOURNAL_EVENT_PASS_FAIL        23  // неверный ввод пароля
    #define JOURNAL_EVENT_EMAIL_OK         24  // правильно отправлен email
    #define JOURNAL_EVENT_EMAIL_FAIL       25  // ошибка при отправке email

    // ошибка связи с фискальником
    #define ERROR_FR_CONN                  26

    // ВСЕ ОШИБКИ ФР ФАТАЛЬНЫЕ
    #define ERROR_FR                       27
  
    #define JOURNAL_EVENTS_COUNT             (ERROR_FR+FR_ERROR_NUMBER)  // число событий

  // канал
  CPU_INT08U channel;
  
  // данные: для получения денег - номинал купюры, для сеанса - длительность оплаченного времени, мин.
  CPU_INT16U data;

}TEventRecord;
#pragma pack(pop)

// структура для хранения счетчиков
typedef struct{
  // число запусков поканально
  CPU_INT32U  CounterChannelRun[CHANNELS_NUM];
  // Суммарное время работы поканально, сек.
  CPU_INT32U  CounterChannelTime[CHANNELS_NUM];
  // Сумма денег поканально  
  CPU_INT32U  CounterChannelMoney[CHANNELS_NUM];
  
  // общее число запусков 
  CPU_INT32U  CounterRun;
  // общее Суммарное время работы, сек.
  CPU_INT32U  CounterTime;
  // общее Сумма денег   
  CPU_INT32U  CounterMoney;
  
  // счетчики купюр в купюрнике по номиналам
  CPU_INT32U  CounterBillNominals[24];
  // общий счетчик купюр (всего в кассете)
  CPU_INT32U  BillsCount;
}TCounters;


// структура для хранения длинных счетчиков
// ведем пока только эти три длинных
typedef struct{
  // число запусков поканально
  CPU_INT32U  CounterChannelRunLong[CHANNELS_NUM];
  // Суммарное время работы поканально, сек.
  CPU_INT32U  CounterChannelTimeLong[CHANNELS_NUM];
  // Сумма денег поканально  
  CPU_INT32U  CounterChannelMoneyLong[CHANNELS_NUM];
  CPU_INT32U  CounterRunLong;
  CPU_INT32U  CounterTimeLong;
  CPU_INT32U  CounterMoneyLong;
  CPU_INT16U  crc;
}TCountersLong;


extern CPU_INT32U GetShortMoney();
extern void IncBillnomCounter(CPU_INT32U index);
extern void CheckLongCounters(void);
extern void SaveEventRecord(CPU_INT08U channel, CPU_INT08U event, CPU_INT16U data);
extern void SetErrorFlag(CPU_INT08U error);
extern void ClrErrorFlag(CPU_INT08U error);
extern int TstErrorFlag(CPU_INT08U error);
extern int TstCriticalErrors(void);
extern void ClearEventJournal(void);
extern void GetEventStr(char* str, char event);
extern int GetEventRecord(TEventRecord* record, CPU_INT32U index);
extern void IncCounter(CPU_INT08U ch, CPU_INT32U time, CPU_INT32U money);
extern void ClearCounters(void);
extern void ErrorServer(void);
extern int TstCriticalValidatorErrors(void);
extern void ClrValidatorErrors(void);
extern void PrintEventJournalRecordEng(char* str, TEventRecord *record);
extern void GetEventStrEng(char* str, char event);
extern void ClearBillnomCounter(void);

#endif //#ifndef _JOURNAL_H_
