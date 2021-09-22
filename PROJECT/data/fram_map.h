#include "cpu.h"
#include "datadesc.h"
#include "journal.h"


typedef struct
{

#ifdef BOARD_CENTRAL_CFG
    
  CPU_INT32U SerialNum;

  TDeviceConfig DeviceConfig;

  CPU_INT32U FRAM_AcceptedMoney[COUNT_POST + COUNT_VACUUM];
  CPU_INT32U crc_AcceptedMoney[COUNT_POST + COUNT_VACUUM];

  CPU_INT32U FRAM_AcceptedBankMoney[COUNT_POST];
  CPU_INT32U crc_AcceptedBankMoney[COUNT_POST];
  
  CPU_INT32U  enableSignalPrint[COUNT_POST];
  CPU_INT32U  signal_pulse_len[COUNT_POST];

  CPU_INT32U  coin_pulse_len[COUNT_POST + COUNT_VACUUM];
  CPU_INT32U  coin_pause_len[COUNT_POST + COUNT_VACUUM];

  CPU_INT32U  cash_pulse_len[COUNT_POST];
  CPU_INT32U  cash_pause_len[COUNT_POST];

  CPU_INT32U  bank_pulse_len[COUNT_POST];
  CPU_INT32U  bank_pause_len[COUNT_POST];

  CPU_INT08U manual_service_flag[4];
  char manual_service_name[32];

  CPU_INT32U  TaxFormat;
  CPU_INT32U  SubjSell;
  CPU_INT32U  CommandV2;
  CPU_INT32U  TaxSystem;

  CPU_INT32U Pass;
  CPU_INT32U crc_Pass;

  // журнал событий+ошибок
  TEventRecord EventRecords[EVENT_RECORDS_COUNT];

  CPU_INT32U Price;

  // счетчики
  TCounters Counters;
  // длинные счетчики с CRC16 
  TCountersLong CountersLong;

  CPU_INT32U StartButtonName;
  
  // таймаут начала мойки без внешнего сигнала - общий на все каналы
  CPU_INT32U  signal_print_timeout_len;
#endif
  
  CPU_INT08U  mac_addr[6];
  CPU_INT32U  ip;
  CPU_INT32U  netmask;
  CPU_INT32U  gateway;

  CPU_INT16U  port;
  
}TFramMap;

