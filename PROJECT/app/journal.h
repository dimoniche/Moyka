#ifndef _JOURNAL_H_
#define _JOURNAL_H_

#include "fiscal.h"

#define EVENT_RECORDS_COUNT  512 // ����� ������� � �������

#pragma pack(push, 1)
/// ��������� ������ ������� ������� �������
typedef struct{
  // ����� �������� ������
  CPU_INT32U time;
  
  // ��� �������
  CPU_INT08U event;
    #define JOURNAL_EVENT_NO_EVENT         0  // ��� �������
  
    #define JOURNAL_EVENT_MONEY_NOTE_POST1 1  // ������� ��������� ������ ���� 1
    #define JOURNAL_EVENT_MONEY_NOTE_POST2 2  // ������� ��������� ������ ���� 2
    #define JOURNAL_EVENT_MONEY_NOTE_POST3 3  // ������� ��������� ������ ���� 3
    #define JOURNAL_EVENT_MONEY_NOTE_POST4 4  // ������� ��������� ������ ���� 4
    #define JOURNAL_EVENT_MONEY_NOTE_POST5 5  // ������� ��������� ������ ���� 5
    #define JOURNAL_EVENT_MONEY_NOTE_POST6 6  // ������� ��������� ������ ���� 6
  
    #define JOURNAL_EVENT_MONEY_COIN_POST1 7  // ������� ��������� ������ (���-�� ������) ���� 1
    #define JOURNAL_EVENT_MONEY_COIN_POST2 8  // ������� ��������� ������ (���-�� ������) ���� 2
    #define JOURNAL_EVENT_MONEY_COIN_POST3 9  // ������� ��������� ������ (���-�� ������) ���� 3
    #define JOURNAL_EVENT_MONEY_COIN_POST4 10  // ������� ��������� ������ (���-�� ������) ���� 4
    #define JOURNAL_EVENT_MONEY_COIN_POST5 11  // ������� ��������� ������ (���-�� ������) ���� 5
    #define JOURNAL_EVENT_MONEY_COIN_POST6 12  // ������� ��������� ������ (���-�� ������) ���� 6
    #define JOURNAL_EVENT_MONEY_COIN_VACUUM1 13  // ������� ��������� ������ (���-�� ������) ������� 1
    #define JOURNAL_EVENT_MONEY_COIN_VACUUM2 14  // ������� ��������� ������ (���-�� ������) ������� 2

    #define JOURNAL_EVENT_START_SESSION    15  // ������� ������ ������
    #define JOURNAL_EVENT_END_SESSION      16  // ������� ����� ������
    #define JOURNAL_EVENT_DEVICE_ON        18  // ��������� ����������
  
    #define JOURNAL_EVENT_PRINT_BILL_POST1       19  // ������ ���� ���� 1
    #define JOURNAL_EVENT_PRINT_BILL_POST2       20  // ������ ���� ���� 2
    #define JOURNAL_EVENT_PRINT_BILL_POST3       21  // ������ ���� ���� 3
    #define JOURNAL_EVENT_PRINT_BILL_POST4       22  // ������ ���� ���� 4
    #define JOURNAL_EVENT_PRINT_BILL_POST5       23  // ������ ���� ���� 5
    #define JOURNAL_EVENT_PRINT_BILL_POST6       24  // ������ ���� ���� 6
    #define JOURNAL_EVENT_PRINT_BILL_VACUUM1     25  // ������ ���� ������� 1
    #define JOURNAL_EVENT_PRINT_BILL_VACUUM2     26  // ������ ���� ������� 2
  
    #define JOURNAL_EVENT_MONEY_BANK_POST1 27  // ������� ��������� ����� � ����� ���� 1
    #define JOURNAL_EVENT_MONEY_BANK_POST2 28  // ������� ��������� ����� � ����� ���� 2
    #define JOURNAL_EVENT_MONEY_BANK_POST3 29  // ������� ��������� ����� � ����� ���� 3
    #define JOURNAL_EVENT_MONEY_BANK_POST4 30  // ������� ��������� ����� � ����� ���� 4
    #define JOURNAL_EVENT_MONEY_BANK_POST5 31  // ������� ��������� ����� � ����� ���� 5
    #define JOURNAL_EVENT_MONEY_BANK_POST6 32  // ������� ��������� ����� � ����� ���� 6

    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST1       33  // ������ ���� ���� 1
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST2       34  // ������ ���� ���� 2
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST3       35  // ������ ���� ���� 3
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST4       36  // ������ ���� ���� 4
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST5       37  // ������ ���� ���� 5
    #define JOURNAL_EVENT_PRINT_BILL_ONLINE_POST6       38  // ������ ���� ���� 6

    #define JOURNAL_EVENT_PRINT_Z          39  // ������ z-������
    #define JOURNAL_EVENT_PRINT_X          40  // ������ x-������
    #define JOURNAL_EVENT_PRINT_BUF        41  // ������ x-������
    #define JOURNAL_EVENT_CHANGE_MODE      42  // ����� ������
    #define JOURNAL_EVENT_WASHING          43  // ������ �����
    #define JOURNAL_EVENT_PASS_FAIL        44  // �������� ���� ������
    #define JOURNAL_EVENT_EMPTY1           45  // 
    #define JOURNAL_EVENT_EMPTY2           46  // 

    // ������ ����� � ������������
    #define ERROR_FR_CONN                  47

    // ��� ������ �� ���������
    #define ERROR_FR                       48
  
    #define JOURNAL_EVENTS_COUNT             (ERROR_FR+FR_ERROR_NUMBER)  // ����� �������

  // �����
  CPU_INT08U channel;
  
  // ������: ��� ��������� ����� - ������� ������, ��� ������ - ������������ ����������� �������, ���.
  CPU_INT16U data;

}TEventRecord;
#pragma pack(pop)

// ��������� ��� �������� ���������
typedef struct{
  
  // ����� ����� �������� 
  CPU_INT32U  CounterRun;
  // ����� ��������� ����� ������, ���.
  CPU_INT32U  CounterTime;
  // ����� ����� �����   
  CPU_INT32U  CounterMoney;

}TCounters;


// ��������� ��� �������� ������� ���������
// ����� ���� ������ ��� ��� �������
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
