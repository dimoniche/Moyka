#ifndef _JOURNAL_H_
#define _JOURNAL_H_

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
    #define JOURNAL_EVENT_MONEY_COIN_VACUUM1 13  // событие получения монеты (кол-во рублей) пылесос 1
    #define JOURNAL_EVENT_MONEY_COIN_VACUUM2 14  // событие получения монеты (кол-во рублей) пылесос 2

    #define JOURNAL_EVENT_START_SESSION    15  // событие начала сеанса
    #define JOURNAL_EVENT_END_SESSION      16  // событие конца сеанса
    #define JOURNAL_EVENT_DEVICE_ON        18  // включение устройства
  
    #define JOURNAL_EVENT_PRINT_BILL_POST1       19  // печать чека пост 1
    #define JOURNAL_EVENT_PRINT_BILL_POST2       20  // печать чека пост 2
    #define JOURNAL_EVENT_PRINT_BILL_POST3       21  // печать чека пост 3
    #define JOURNAL_EVENT_PRINT_BILL_POST4       22  // печать чека пост 4
    #define JOURNAL_EVENT_PRINT_BILL_POST5       23  // печать чека пост 5
    #define JOURNAL_EVENT_PRINT_BILL_POST6       24  // печать чека пост 6
    #define JOURNAL_EVENT_PRINT_BILL_VACUUM1     25  // печать чека пылесос 1
    #define JOURNAL_EVENT_PRINT_BILL_VACUUM2     26  // печать чека пылесос 2
  
    #define JOURNAL_EVENT_MONEY_BANK_POST1 27  // событие получения денег с карты пост 1
    #define JOURNAL_EVENT_MONEY_BANK_POST2 28  // событие получения денег с карты пост 2
    #define JOURNAL_EVENT_MONEY_BANK_POST3 29  // событие получения денег с карты пост 3
    #define JOURNAL_EVENT_MONEY_BANK_POST4 30  // событие получения денег с карты пост 4
    #define JOURNAL_EVENT_MONEY_BANK_POST5 31  // событие получения денег с карты пост 5
    #define JOURNAL_EVENT_MONEY_BANK_POST6 32  // событие получения денег с карты пост 6

    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST1       33  // печать чека пост 1
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST2       34  // печать чека пост 2
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST3       35  // печать чека пост 3
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST4       36  // печать чека пост 4
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST5       37  // печать чека пост 5
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST6       38  // печать чека пост 6

    #define JOURNAL_EVENT_PRINT_Z          39  // печать z-отчета
    #define JOURNAL_EVENT_PRINT_X          40  // печать x-отчета
    #define JOURNAL_EVENT_PRINT_BUF        41  // печать x-отчета
    #define JOURNAL_EVENT_CHANGE_MODE      42  // смена режима
    #define JOURNAL_EVENT_WASHING          43  // запуск мойки
    #define JOURNAL_EVENT_PASS_FAIL        44  // неверный ввод пароля
    #define JOURNAL_EVENT_EMPTY1           45  // 
    #define JOURNAL_EVENT_EMPTY2           46  // 

    // ошибка связи с фискальником
    #define ERROR_FR_CONN                  47

    // ВСЕ ОШИБКИ ФР ФАТАЛЬНЫЕ
    #define ERROR_FR                       48
  
    #define JOURNAL_EVENTS_COUNT             (ERROR_FR+FR_ERROR_NUMBER)  // число событий

  // канал
  CPU_INT08U channel;
  
  // данные: для получения денег - номинал купюры, для сеанса - длительность оплаченного времени, мин.
  CPU_INT16U data;

}TEventRecord;
#pragma pack(pop)

// структура для хранения счетчиков
typedef struct{
  
  // общее число запусков 
  CPU_INT32U  CounterRun;
  // общее Суммарное время работы, сек.
  CPU_INT32U  CounterTime;
  // общее Сумма денег   
  CPU_INT32U  CounterMoney;

}TCounters;


// структура для хранения длинных счетчиков
// ведем пока только эти три длинных
typedef struct{
  CPU_INT32U  CounterRunLong;
  CPU_INT32U  CounterTimeLong;
  CPU_INT32U  CounterMoneyLong;
  CPU_INT16U  crc;
}TCountersLong;

extern CPU_INT32U GetShortMoney();
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
extern void ErrorServer(void);
extern int TstCriticalValidatorErrors(void);
extern void ClrValidatorErrors(void);
extern void PrintEventJournalRecordEng(char* str, TEventRecord *record);
extern void GetEventStrEng(char* str, char event);

#endif //#ifndef _JOURNAL_H_
