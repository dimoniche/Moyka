#include <includes.h>
#include <stddef.h>
#include "journal.h"
#include "fram.h"
#include "fram_map.h"
#include "time.h"
#include "fr.h"
#include "crc16.h"
#include "mode.h"

static CPU_INT32U  GlobalErrorsFlags[JOURNAL_EVENTS_COUNT / 32 + 1] = {0};
static CPU_INT32U PrevFlags[JOURNAL_EVENTS_COUNT / 32 + 1] = {0};
    
void SetErrorFlag(CPU_INT08U error)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  GlobalErrorsFlags[error/32] |= (1L << (error%32));
  OS_EXIT_CRITICAL();
}

void ClrErrorFlag(CPU_INT08U error)
{
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  GlobalErrorsFlags[error/32] &= ~(1L << (error%32));
  OS_EXIT_CRITICAL();
}

int TstErrorFlag(CPU_INT08U error)
{
  CPU_INT32U temp = 0;
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  OS_ENTER_CRITICAL();
  temp = GlobalErrorsFlags[error/32] & (1L << (error%32));
  OS_EXIT_CRITICAL();
  return temp;
}

int TstCriticalErrors(void)
{
  CPU_INT32U errors = 0;
  #if OS_CRITICAL_METHOD == 3
  OS_CPU_SR  cpu_sr = 0;
  #endif
  CPU_INT32U ignore_fiscal = 0;

  GetData(&DisableFiscalErrorsDesc, &ignore_fiscal, 0, DATA_FLAG_SYSTEM_INDEX);     

  OS_ENTER_CRITICAL();
  
  if (!ignore_fiscal)
  {
    errors |= TstCriticalFiscalError();  
    errors |= TstErrorFlag(ERROR_FR_CONN);  
    /*
    if (!FReportTest())
    { // ФР недоступен для любой печати 
      errors |= 0x1;
    }
    */
  }
  
  OS_EXIT_CRITICAL();
  if (errors) return 1;
  return 0;
}


int TstCriticalValidatorErrors(void)
{
  return 0;
}

void ClrValidatorErrors(void)
{
}

// получить запись из журнала событий
int GetEventRecord(TEventRecord* record, CPU_INT32U index)
{
    if (index >= EVENT_RECORDS_COUNT) return -1;
    ReadArrayFram(offsetof(TFramMap, EventRecords[0])+index*sizeof(TEventRecord), sizeof(TEventRecord), (unsigned char*)record);
    return 0;
}

// запись в журнал записи о событии
void SaveEventRecord(CPU_INT08U channel, CPU_INT08U event, CPU_INT16U data)
{
    TEventRecord record;
    
    // найти самую старую по времени запись и добавить новую после неё
    CPU_INT32U i, ind=0, indm = 0, maxtime = 0;
    for (i = 0; i < EVENT_RECORDS_COUNT; i++)
    {
        ReadArrayFram(offsetof(TFramMap, EventRecords[0])+i*sizeof(TEventRecord), sizeof(TEventRecord), (unsigned char*)&record);
        if ((record.time == 0) || (record.time == 0xffffffff)) {ind = i; break;}
        if (record.time >= maxtime) {maxtime = record.time; indm = i;}
    }
    
    if (i >= EVENT_RECORDS_COUNT) 
    {
        // все записи заполены - записываем на место самой старой
        ind = (indm + 1) % EVENT_RECORDS_COUNT;
    }
    
    record.time = GetTimeSec();
    record.channel = channel;
    record.event = event;
    record.data = data;
    WriteArrayFram(offsetof(TFramMap, EventRecords[0])+ind*sizeof(TEventRecord), sizeof(TEventRecord), (unsigned char*)&record);
}


void ClearEventJournal(void)
{
  SetArrayFram(offsetof(TFramMap, EventRecords), sizeof(TEventRecord)*EVENT_RECORDS_COUNT, 0x00);
}

void GetEventStr(char* str, char event)
{
  switch (event){
    case JOURNAL_EVENT_MONEY_NOTE_POST1:
      sprintf(str, "Вн.купюра пост 1");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST2:
      sprintf(str, "Вн.купюра пост 2");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST3:
      sprintf(str, "Вн.купюра пост 3");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST4:
      sprintf(str, "Вн.купюра пост 4");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST5:
      sprintf(str, "Вн.купюра пост 5");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST6:
      sprintf(str, "Вн.купюра пост 6");
      break;
   
    case JOURNAL_EVENT_MONEY_BANK_POST1:
      sprintf(str, "Эл.платеж пост 1");
      break;
    case JOURNAL_EVENT_MONEY_BANK_POST2:
      sprintf(str, "Эл.платеж пост 2");
      break;
    case JOURNAL_EVENT_MONEY_BANK_POST3:
      sprintf(str, "Эл.платеж пост 3");
      break;
    case JOURNAL_EVENT_MONEY_BANK_POST4:
      sprintf(str, "Эл.платеж пост 4");
      break;
    case JOURNAL_EVENT_MONEY_BANK_POST5:
      sprintf(str, "Эл.платеж пост 5");
      break;
    case JOURNAL_EVENT_MONEY_BANK_POST6:
      sprintf(str, "Эл.платеж пост 6");
      break;

    case JOURNAL_EVENT_MONEY_COIN_POST1:
      sprintf(str, "Вн.монеты пост 1");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST2:
      sprintf(str, "Вн.монеты пост 2");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST3:
      sprintf(str, "Вн.монеты пост 3");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST4:
      sprintf(str, "Вн.монеты пост 4");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST5:
      sprintf(str, "Вн.монеты пост 5");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST6:
      sprintf(str, "Вн.монеты пост 6");
      break;
    case JOURNAL_EVENT_MONEY_COIN_VACUUM1:
      sprintf(str, "Вн.монеты пыл.1");
      break;
    case JOURNAL_EVENT_MONEY_COIN_VACUUM2:
      sprintf(str, "Вн.монеты пыл.2");
      break;

    case JOURNAL_EVENT_START_SESSION:
      sprintf(str, "Печать чека ");
      break;
    case JOURNAL_EVENT_END_SESSION:
      sprintf(str, "Кон.сеанса ");
      break;
    case JOURNAL_EVENT_DEVICE_ON:
      sprintf(str, "Включение");
      break;
      
    case JOURNAL_EVENT_PRINT_BILL_POST1:
      sprintf(str, "Печать чека пост 1");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST2:
      sprintf(str, "Печать чека пост 2");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST3:
      sprintf(str, "Печать чека пост 3");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST4:
      sprintf(str, "Печать чека пост 4");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST5:
      sprintf(str, "Печать чека пост 5");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST6:
      sprintf(str, "Печать чека пост 6");
      break;
    case JOURNAL_EVENT_PRINT_BILL_VACUUM1:
      sprintf(str, "Печать чека пыл.1");
      break;
    case JOURNAL_EVENT_PRINT_BILL_VACUUM2:
      sprintf(str, "Печать чека пыл.2");
      break;
     
    case JOURNAL_EVENT_PRINT_BILL_ONLINE_POST1:
      sprintf(str, "Печ.эл.чека пост 1");
      break;
    case JOURNAL_EVENT_PRINT_BILL_ONLINE_POST2:
      sprintf(str, "Печ.эл.чека пост 2");
      break;
    case JOURNAL_EVENT_PRINT_BILL_ONLINE_POST3:
      sprintf(str, "Печ.эл.чека пост 3");
      break;
    case JOURNAL_EVENT_PRINT_BILL_ONLINE_POST4:
      sprintf(str, "Печ.эл.чека пост 4");
      break;
    case JOURNAL_EVENT_PRINT_BILL_ONLINE_POST5:
      sprintf(str, "Печ.эл.чека пост 5");
      break;
    case JOURNAL_EVENT_PRINT_BILL_ONLINE_POST6:
      sprintf(str, "Печ.эл.чека пост 6");
      break;

    case JOURNAL_EVENT_PRINT_Z:
      sprintf(str, "Печать отчета о закр.см.");
      break;
    case JOURNAL_EVENT_PRINT_X:
      sprintf(str, "Печать X-отчета");
      break;
    case JOURNAL_EVENT_PRINT_BUF:
      sprintf(str, "Печать отч.из буф.");
      break;
    case JOURNAL_EVENT_CHANGE_MODE:
      sprintf(str, "Смена режима");
      break;
    case JOURNAL_EVENT_WASHING:
      sprintf(str, "Мойка");
      break;  
    case JOURNAL_EVENT_PASS_FAIL:
      sprintf(str, "Неверный пароль");
      break;  
    default:
      sprintf(str, "нет");
      break;
  }
}

void GetEventStrEng(char* str, char event)
{
  switch (event){
    case JOURNAL_EVENT_MONEY_NOTE_POST1:
      sprintf(str, " |  Vnesena kupura post 1");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST2:
      sprintf(str, " |  Vnesena kupura post 2");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST3:
      sprintf(str, " |  Vnesena kupura post 3");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST4:
      sprintf(str, " |  Vnesena kupura post 4");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST5:
      sprintf(str, " |  Vnesena kupura post 5");
      break;
    case JOURNAL_EVENT_MONEY_NOTE_POST6:
      sprintf(str, " |  Vnesena kupura post 6");
      break;

    case JOURNAL_EVENT_MONEY_COIN_POST1:
      sprintf(str, " |  Vneseny monety post 1");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST2:
      sprintf(str, " |  Vneseny monety post 2");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST3:
      sprintf(str, " |  Vneseny monety post 3");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST4:
      sprintf(str, " |  Vneseny monety post 4");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST5:
      sprintf(str, " |  Vneseny monety post 5");
      break;
    case JOURNAL_EVENT_MONEY_COIN_POST6:
      sprintf(str, " |  Vneseny monety post 6");
      break;
    case JOURNAL_EVENT_MONEY_COIN_VACUUM1:
      sprintf(str, " |  Vneseny monety vacuu1");
      break;
    case JOURNAL_EVENT_MONEY_COIN_VACUUM2:
      sprintf(str, " |  Vneseny monety vacuu2");
      break;

    case JOURNAL_EVENT_START_SESSION:
      sprintf(str, " |  Print bill ");
      break;
    case JOURNAL_EVENT_END_SESSION:
      sprintf(str, " |  Koneс seansa ");
      break;
    case JOURNAL_EVENT_DEVICE_ON:
      sprintf(str, " |  Vkluchenie ");
      break;
      
    case JOURNAL_EVENT_PRINT_BILL_POST1:
      sprintf(str, " |  Pechat' checka post 1");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST2:
      sprintf(str, " |  Pechat' checka post 2");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST3:
      sprintf(str, " |  Pechat' checka post 3");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST4:
      sprintf(str, " |  Pechat' checka post 4");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST5:
      sprintf(str, " |  Pechat' checka post 5");
      break;
    case JOURNAL_EVENT_PRINT_BILL_POST6:
      sprintf(str, " |  Pechat' checka post 6");
      break;
    case JOURNAL_EVENT_PRINT_BILL_VACUUM1:
      sprintf(str, " |  Pechat' checka vacuu1");
      break;
    case JOURNAL_EVENT_PRINT_BILL_VACUUM2:
      sprintf(str, " |  Pechat' checka vacuu2");
      break;

    case JOURNAL_EVENT_PRINT_Z:
      sprintf(str, " |  Pechat' Z-otcheta ");
      break;
    case JOURNAL_EVENT_PRINT_X:
      sprintf(str, " |  Pechat' X-otcheta ");
      break;
    case JOURNAL_EVENT_PRINT_BUF:
      sprintf(str, " |  Pechat' otcheta iz bufera ");
      break;
    case JOURNAL_EVENT_CHANGE_MODE:
      sprintf(str, " |  Smena rejima ");
      break;
    case JOURNAL_EVENT_WASHING:
      sprintf(str, " |  Moyka ");
      break;  
    case JOURNAL_EVENT_PASS_FAIL:
      sprintf(str, " |  Neverniy parol' ");
      break;
    default:
      sprintf(str, " |  Net sobytiya ");
      break;
  }
}

void PrintEventJournalRecordEng(char* str, TEventRecord *record)
{
  if (record->event)
    {
      TRTC_Data rtc_data;
      
      // напечатаем время 
      Sec2Date(&rtc_data, record->time);
      sprintf(str, "|  ");
      PrintRTCDateTimeString(&str[strlen(str)], &rtc_data);
      // напечатаем событие
      GetEventStrEng(&str[strlen(str)], record->event);
      
      // напечатаем дополнительные поля
      if ((record->event >= JOURNAL_EVENT_MONEY_NOTE_POST1) && (record->event <= JOURNAL_EVENT_MONEY_COIN_VACUUM2))
        {
          sprintf(&str[strlen(str)], "%d rub.", record->data);
        }
      else if ((record->event >= JOURNAL_EVENT_MONEY_BANK_POST1) && (record->event <= JOURNAL_EVENT_MONEY_BANK_POST6))
        {
          sprintf(&str[strlen(str)], "%d rub.", record->data);
        }
      else if (record->event == JOURNAL_EVENT_START_SESSION)
        {
          sprintf(&str[strlen(str)], "");
          PrintSecToHourMinSec(&str[strlen(str)], record->data);
        }
      else if (record->event == JOURNAL_EVENT_END_SESSION)
        {
          sprintf(&str[strlen(str)], "kanal %d ", record->channel+1);
          sprintf(&str[strlen(str)], "");
        }
      else if (record->event == JOURNAL_EVENT_DEVICE_ON)
        {
          sprintf(&str[strlen(str)], "");
        }
      else if ((record->event >= JOURNAL_EVENT_PRINT_BILL_POST1) && (record->event <= JOURNAL_EVENT_PRINT_BILL_VACUUM2))
        {
          sprintf(&str[strlen(str)], " ");
        }
      else if ((record->event >= JOURNAL_EVENT_PRINT_BILL_ONLINE_POST1) && (record->event <= JOURNAL_EVENT_PRINT_BILL_ONLINE_POST1))
        {
          sprintf(&str[strlen(str)], " ");
        }
      else if (record->event == JOURNAL_EVENT_PRINT_Z)
        {
          sprintf(&str[strlen(str)], "");
        }
      else if (record->event == JOURNAL_EVENT_PRINT_X)
        {
          sprintf(&str[strlen(str)], "");
        }
      else if (record->event == JOURNAL_EVENT_PRINT_BUF)
        {
          sprintf(&str[strlen(str)], "");
        }
      else if (record->event == JOURNAL_EVENT_CHANGE_MODE)
      {
          if (record->data == MODE_WORK) sprintf(&str[strlen(str)], "rabota");
          else sprintf(&str[strlen(str)], "nastroika");
      }
      else if (record->event == JOURNAL_EVENT_WASHING)
      {
          sprintf(&str[strlen(str)], "%u post.", record->data + 1);
      }
      else if (record->event == JOURNAL_EVENT_PASS_FAIL)
      {
          sprintf(&str[strlen(str)], "%u", record->data);
      }

      sprintf(&str[strlen(str)], "\r\n");
    }
  else
    { // пустая запись
      sprintf(str, "net zapisi\r\n");
    }
}

void IncCounter(CPU_INT08U ch, CPU_INT32U time, CPU_INT32U money)
{
  CPU_INT32U r, t, m;
  TCountersLong long_ctrs;
  
  // увеличим общие счетчики
  ReadArrayFram(offsetof(TFramMap, Counters.CounterRun), sizeof(CPU_INT32U), (unsigned char*)&r);
  ReadArrayFram(offsetof(TFramMap, Counters.CounterTime), sizeof(CPU_INT32U), (unsigned char*)&t);
  ReadArrayFram(offsetof(TFramMap, Counters.CounterMoney), sizeof(CPU_INT32U), (unsigned char*)&m);
  r++;
  t+=time;
  m+=money;
  WriteArrayFram(offsetof(TFramMap, Counters.CounterRun), sizeof(CPU_INT32U), (unsigned char*)&r);
  WriteArrayFram(offsetof(TFramMap, Counters.CounterTime), sizeof(CPU_INT32U), (unsigned char*)&t);
  WriteArrayFram(offsetof(TFramMap, Counters.CounterMoney), sizeof(CPU_INT32U), (unsigned char*)&m);

  // увеличим длинные счетчики
  ReadArrayFram(offsetof(TFramMap, CountersLong), sizeof(TCountersLong), (unsigned char*)&long_ctrs);

  long_ctrs.CounterRunLong++;
  long_ctrs.CounterTimeLong += time;
  long_ctrs.CounterMoneyLong += money;
  long_ctrs.crc = CRC16((unsigned char*)&long_ctrs, offsetof(TCountersLong, crc));
  WriteArrayFram(offsetof(TFramMap, CountersLong), sizeof(TCountersLong), (unsigned char*)&long_ctrs);
}  

CPU_INT32U GetShortMoney()
{
  CPU_INT32U money;
  ReadArrayFram(offsetof(TFramMap, Counters.CounterMoney), sizeof(CPU_INT32U), (unsigned char*)&money);
  return money;
}

void CheckLongCounters(void)
{
    TCountersLong long_ctrs;
    CPU_INT16U crc;
    ReadArrayFram(offsetof(TFramMap, CountersLong), sizeof(TCountersLong), (unsigned char*)&long_ctrs);
    crc = CRC16((unsigned char*)&long_ctrs, offsetof(TCountersLong, crc));
    if (crc != long_ctrs.crc)
    {
        memset(&long_ctrs, 0, sizeof(TCountersLong));
        long_ctrs.crc = CRC16((unsigned char*)&long_ctrs, offsetof(TCountersLong, crc));
        WriteArrayFram(offsetof(TFramMap, CountersLong), sizeof(TCountersLong), (unsigned char*)&long_ctrs);
    }
}

// сервер ошибок (занесение ошибок в журнал)
void ErrorServer(void)
{
    for (int i = ERROR_FR_CONN; i < JOURNAL_EVENTS_COUNT; i++)
    {
        if (!(PrevFlags[i/32] & (1L<<(i%32)))
            && (TstErrorFlag(i)))
        {
            // запишем в журнал
            SaveEventRecord(0, i, 0);
        }
    }
    
    memcpy(PrevFlags, GlobalErrorsFlags, sizeof(CPU_INT32U) * (JOURNAL_EVENTS_COUNT / 32 + 1));
}

