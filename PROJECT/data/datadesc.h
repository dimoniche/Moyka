#ifndef _DATADESC_H_
#define _DATADESC_H_

#include "data.h"

#define INCAS_SEND_FLAG     0x87654321

#define MAX_PRICE 9999

// 6 постов мойки - 6 купюроприемников и 6 монетоприемников
#define COUNT_POST 6
// плюс два пылесоса
#define COUNT_VACUUM 2

#define DEFAULT_PASSWORD 1111
#define MASTER_PASSWORD 11300045//1234567890L

// структура конфигурации аппаратуры
typedef struct{
  
  CPU_INT32U  EnableCoinAcceptor[COUNT_POST + COUNT_VACUUM];
  CPU_INT32U  EnableValidator[COUNT_POST];
  CPU_INT32U  EnableBank[COUNT_POST];
  
  CPU_INT32U  EnableModem;
  CPU_INT32U  EnableFiscal;
  CPU_INT32U  EnableFiscalDayClear;
  CPU_INT32U  ServiceName;

  CPU_INT32U  CoinPerPulse[COUNT_POST + COUNT_VACUUM]; // цена импульса монетоприемника
  CPU_INT32U  CoinTimeOut[COUNT_POST + COUNT_VACUUM];  // таймаут печати чека монетоприемника

  CPU_INT32U  BillFormat;
  
  CPU_INT32U  DisableFiscalErrors; // отключение реакции на ошибки ФР
  
  CPU_INT32U  EnableEmailErrorSend;
  CPU_INT32U  EnableEmailStatSend;
  CPU_INT32U  EnableEmailJournalSend;
  CPU_INT32U  ClearJournalAfterSend;
  CPU_INT32U  StatSendHourMin;

  CPU_INT32U  CashMode[COUNT_POST];
  CPU_INT32U  CashPerPulse[COUNT_POST]; // цена импульса купюрника

  CPU_INT32U  BankPerPulse[COUNT_POST]; // цена импульса банковского терминала
  
  CPU_INT32U  PrintTimeout;
  CPU_INT32U  PrintTimeoutAfter;

  CPU_INT32U  CoinLevel[COUNT_POST + COUNT_VACUUM];   // уровень сигнала монетника
  CPU_INT32U  CashLevel[COUNT_POST];   // уровень сигнала купюрника
  CPU_INT32U  SignalLevel[COUNT_POST]; // уровень сигнала сигнала печати
  CPU_INT32U  BankLevel[COUNT_POST];   // уровень сигнала купюрника
  
  CPU_INT32U  PrintMode;

  CPU_INT32U  DeviceId;

  CPU_INT32U  CashPerMinute;  // стоимость 1 минуты мойки
  
}TDeviceConfig;


extern CPU_INT32U PeriodIndex;
extern TDataDescStruct const DeviceIDDesc;

extern TDataDescStruct const ServiceNameDesc;
extern TDataDescStruct const PassDesc;
extern TDataDescStruct const PriceDesc;
extern TDataDescStruct const PassCRCDesc;
extern TDataDescStruct const PassTempDesc;
extern CPU_INT32U TempPass;
extern TDataDescStruct const PassTempDesc1;
extern TDataDescStruct const PassTempDesc2;

extern TDataDescStruct const EnableFiscalDesc;
extern TDataDescStruct const EnableCoinDesc;
extern TDataDescStruct const EnableModemDesc;
extern TDataDescStruct const EnableValidatorDesc;
extern TDataDescStruct const EnableBankDesc;
extern TDataDescStruct const CoinPerPulseDesc;
extern TDataDescStruct const EnableFiscalDayClearDesc;

extern TDataDescStruct const InitByDefaultDesc;
extern TDataDescStruct const PrintZReportDesc;
extern TDataDescStruct const PrintXReportDesc;

extern TDataDescStruct const EventJournalIndexDesc; 
extern TDataDescStruct const JournalEventTimeDesc;

extern TDataDescStruct const JournalErrorNumberDesc0;
extern TDataDescStruct const JournalErrorNumberDesc1;
extern TDataDescStruct const ClearJournalCmdDesc;


extern TDataDescStruct const SystemTimeDesc;
extern TDataDescStruct const SystemTimeEditDesc;

extern const TDataDescArrayStruct AllDataArray[];

extern CPU_INT32U ErrorJournalIndex;
extern CPU_INT32U EventJournalIndex;
extern CPU_INT32U last_Index;

extern TDataDescStruct const CounterRunDesc;
extern TDataDescStruct const CounterMoneyDesc;
extern TDataDescStruct const CounterTimeDesc;
extern TDataDescStruct const CounterChannelRunDesc;
extern TDataDescStruct const CounterChannelMoneyDesc;
extern TDataDescStruct const CounterChannelTimeDesc;
extern TDataDescStruct const ClearStatCmdDesc;
extern TDataDescStruct const BillFormatDesc;

extern TDataDescStruct const AcceptedMoneyDesc;
extern TDataDescStruct const AcceptedMoneyDesc1;
extern TDataDescStruct const AcceptedMoneyCRC16Desc;
extern TDataDescStruct const AcceptedBankMoneyDesc;
extern TDataDescStruct const AcceptedBankMoneyDesc1;
extern TDataDescStruct const AcceptedBankMoneyCRC16Desc;

extern TDataDescStruct const DisableFiscalErrorsDesc;

extern TDataDescStruct const StartButtonNameDesc;

extern TDataDescStruct const EnableEmailErrorSendDesc;
extern TDataDescStruct const EnableEmailJournalSendDesc;
extern TDataDescStruct const ClearJournalAfterSendDesc;
extern TDataDescStruct const StatSendPeriodDesc;
extern TDataDescStruct const JournalErrorNumberDescEng;
extern TDataDescStruct const SendTestEmailDesc;
extern TDataDescStruct const ModemStatusDesc;

extern TDataDescStruct const CounterLongRunDesc;
extern TDataDescStruct const CounterLongMoneyDesc;
extern TDataDescStruct const CounterLongTimeDesc;

extern TDataDescStruct const MasterPassTempDesc;

extern TDataDescStruct const CounterChannelRunLongDesc;
extern TDataDescStruct const CounterChannelMoneyLongDesc;
extern TDataDescStruct const CounterChannelTimeLongDesc;
extern TDataDescStruct const ChannelStLongIndexDesc;

extern TDataDescStruct const GatewayDesc;
extern TDataDescStruct const NetMaskDesc;
extern TDataDescStruct const IpAddrDesc;

extern TDataDescStruct const CashModeDesc;
extern TDataDescStruct const CashPerPulseDesc;
extern TDataDescStruct const PrintTimeoutDesc;

extern void OnChangeInitByDefault(void);
extern void OnChangeServiceName(void);

extern TDataDescStruct const CashPulseLenDesc;
extern TDataDescStruct const CashPauseLenDesc;
extern void OnChangeCashPulseLen();
extern void OnChangeSinalPulseLen();
extern void OnChangeLevel();
extern void OnChangeLevelWithoutInit();

extern TDataDescStruct const CoinPulseLenDesc;
extern TDataDescStruct const CoinPauseLenDesc;
extern void OnChangeCoinPulseLen();

extern TDataDescStruct const PrintModeDesc;
extern TDataDescStruct const CashPerMinuteDesc;
extern TDataDescStruct const TaxFormatDesc;
extern TDataDescStruct const SubjSellDesc;
extern TDataDescStruct const CommandV2Desc;
extern TDataDescStruct const TaxSystemDesc;

extern TDataDescStruct const CoinIndexDesc;
extern TDataDescStruct const CashIndexDesc;
extern TDataDescStruct const BankIndexDesc;

extern CPU_INT32U CoinIndex;

extern TDataDescStruct const SignalIndexDesc;
extern TDataDescStruct const EnableSignalDesc;
extern TDataDescStruct const SignalPulseDesc;
extern TDataDescStruct const SignalTimeOutDesc;

extern TDataDescStruct const CashLevelDesc;
extern TDataDescStruct const CoinLevelDesc;
extern TDataDescStruct const SignalPrintLevelDesc;
extern TDataDescStruct const SignalStopMoneyLevelDesc;

extern TDataDescStruct const BankPerPulseDesc;
extern TDataDescStruct const BankPulseLenDesc;
extern TDataDescStruct const BankPauseLenDesc;
extern TDataDescStruct const BankLevelDesc;

extern TDataDescStruct const CoinTimeOutDesc;
extern TDataDescStruct const StatMoneyIndexDesc;

extern void OnChangeBankPulseLen();

#endif //#ifndef _DATADESC_H_
