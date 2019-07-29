#include <includes.h>
#include "data.h"
#include "datadesc.h"
#include "menu.h"
#include "menudesc.h"
#include "fram.h"
#include "fram_map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "fiscal.h"
#include "time.h"
#include "CRC16.h"
#include "coin.h"

extern CPU_INT32U modem_status;

/*************************************
  Последнее время отправки email
*************************************/
extern TRangeValueULONG const WorkTimeRange;

/*************************************
  Длина импульса входа монетоприемника, мс
*************************************/
TRangeValueULONG const CoinPulseLenRange = {20, 250};
CPU_INT08U const CoinPulseLenName[] = "Длина имп.,мс";

void OnChangeCoinPulseLen()
{
    CPU_INT32U pulse, pause;
    for(int post = 0; post < COUNT_POST; post++)
    {
      GetData(&CoinPulseLenDesc, &pulse, post, DATA_FLAG_DIRECT_INDEX);
      GetData(&CoinPauseLenDesc, &pause, post, DATA_FLAG_DIRECT_INDEX);
      SetCoinPulseParam(pulse, pause, post);
    }
}

TDataDescStruct const CoinPulseLenDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &CoinIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, coin_pulse_len),            // указатель на переменную или адрес FRAM
  (void*)&CoinPulseLenRange,     // указатель на границы параметра
  OnChangeCoinPulseLen,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  CoinPulseLenName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  50                           
};

/*************************************
  Длина импульса входа купюрника, мс
*************************************/
TRangeValueULONG const CashPulseLenRange = {20, 250};
CPU_INT08U const CashPulseLenName[] = "Длина имп.,мс";

void OnChangeCashPulseLen()
{
    CPU_INT32U pulse, pause;
    for(int post = 0; post < COUNT_POST; post++)
    {
      GetData(&CashPulseLenDesc, &pulse, post, DATA_FLAG_DIRECT_INDEX);
      GetData(&CashPauseLenDesc, &pause, post, DATA_FLAG_DIRECT_INDEX);
      SetCashPulseParam(pulse, pause, post);
    }
}

TDataDescStruct const CashPulseLenDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &CashIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, cash_pulse_len),            // указатель на переменную или адрес FRAM
  (void*)&CashPulseLenRange,     // указатель на границы параметра
  OnChangeCashPulseLen,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  CashPulseLenName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  50                           
};

/*************************************
  Длина импульса входа банковского терминала, мс
*************************************/
TRangeValueULONG const BankPulseLenRange = {20, 250};
CPU_INT08U const BankPulseLenName[] = "Длина имп.,мс";

void OnChangeBankPulseLen()
{
    CPU_INT32U pulse, pause;
    for(int post = 0; post < COUNT_POST; post++)
    {
      GetData(&BankPulseLenDesc, &pulse, post, DATA_FLAG_DIRECT_INDEX);
      GetData(&BankPauseLenDesc, &pause, post, DATA_FLAG_DIRECT_INDEX);
      SetBankPulseParam(pulse, pause, post);
    }
}

TDataDescStruct const BankPulseLenDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &BankIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, bank_pulse_len),            // указатель на переменную или адрес FRAM
  (void*)&BankPulseLenRange,     // указатель на границы параметра
  OnChangeBankPulseLen,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  BankPulseLenName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  50                           
};

/*************************************
  Длина паузы входа монетника, мс
*************************************/
TRangeValueULONG const CoinPauseLenRange = {20, 250};
CPU_INT08U const CoinPauseLenName[] = "Пауза имп.,мс";

TDataDescStruct const CoinPauseLenDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &CoinIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, coin_pause_len),            // указатель на переменную или адрес FRAM
  (void*)&CoinPauseLenRange,     // указатель на границы параметра
  OnChangeCoinPulseLen,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  CoinPauseLenName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  20                           
};
/*************************************
  Длина паузы входа купюрника, мс
*************************************/
TRangeValueULONG const CashPauseLenRange = {20, 250};
CPU_INT08U const CashPauseLenName[] = "Пауза имп.,мс";

TDataDescStruct const CashPauseLenDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &CashIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, cash_pause_len),            // указатель на переменную или адрес FRAM
  (void*)&CashPauseLenRange,     // указатель на границы параметра
  OnChangeCashPulseLen,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  CashPauseLenName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  20                           
};

/*************************************
  Длина паузы входа банковского терминала, мс
*************************************/
TRangeValueULONG const BankPauseLenRange = {20, 250};
CPU_INT08U const BankPauseLenName[] = "Пауза имп.,мс";

TDataDescStruct const BankPauseLenDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &BankIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, bank_pause_len),            // указатель на переменную или адрес FRAM
  (void*)&BankPauseLenRange,     // указатель на границы параметра
  OnChangeBankPulseLen,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  BankPauseLenName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  20                           
};

/*************************************
  Название кнопки СТАРТ (СТАРТ или ПУСК)
*************************************/
TRangeValueULONG const StartButtonNameRange = {0, 1};
CPU_INT08U const StartButtonNameName[] = "Кнопка";
CPU_INT08U const StartButtonNameList_str0[] = "СТАРТ";
CPU_INT08U const StartButtonNameList_str1[] = "ПУСК";
CPU_INT08U const *StartButtonNameList[] = {StartButtonNameList_str0, StartButtonNameList_str1};


TDataDescStruct const StartButtonNameDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, StartButtonName),            // указатель на переменную или адрес FRAM
  (void*)&StartButtonNameRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  StartButtonNameName,      // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  StartButtonNameList,      // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  Включение купюроприемника
*************************************/
TRangeValueULONG const EnableValidatorRange = {0, 1};
CPU_INT08U const EnableValidatorName[] = "Купюропр-к";
CPU_INT08U const OnOffList_str0[] = "выкл.";
CPU_INT08U const OnOffList_str1[] = "вкл.";
CPU_INT08U const *EnableValidatorList[] = {OnOffList_str0, OnOffList_str1};

TDataDescStruct const EnableValidatorDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &CashIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.EnableValidator),            // указатель на переменную или адрес FRAM
  (void*)&EnableValidatorRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  EnableValidatorName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableValidatorList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           // значение по умолчанию
};

/*************************************
  Включение банковского терминала
*************************************/
TRangeValueULONG const EnableBankRange = {0, 1};
CPU_INT08U const EnableBankName[] = "Банк.термин.";

TDataDescStruct const EnableBankDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &BankIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.EnableBank),            // указатель на переменную или адрес FRAM
  (void*)&EnableBankRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  EnableBankName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableValidatorList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           // значение по умолчанию
};

/*************************************
  Включение модема
*************************************/
TRangeValueULONG const EnableModemRange = {0, 1};
CPU_INT08U const EnableModemName[] = "Модем";
CPU_INT08U const *EnableModemList[] = {OnOffList_str0, OnOffList_str1};

void OnchangeEnableModem(void)
{
//    CPU_INT32U en = 0;
//    GetData(&EnableModemDesc, &en, 0, DATA_FLAG_SYSTEM_INDEX);
//    
//    if (en)
//    {
//        if (!IsModemConn())
//        {
//            modem_status = 2;
//        }
//        else if (!IsModemConf())
//        {
//            modem_status = 1;
//        }
//        //PostModemTask(MODEM_TASK_RECONNECT);
//    }
}

TDataDescStruct const EnableModemDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.EnableModem),            // указатель на переменную или адрес FRAM
  (void*)&EnableModemRange,     // указатель на границы параметра
  OnchangeEnableModem,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  EnableModemName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableModemList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Включить оповещение об ошибках по e-mail
*************************************/
TRangeValueULONG const EnableEmailErrorSendRange = {0, 1};
CPU_INT08U const EnableEmailErrorSendName[] = "Опов.об ош.";
CPU_INT08U const *EnableEmailErrorSendList[] = {OnOffList_str0, OnOffList_str1};

TDataDescStruct const EnableEmailErrorSendDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.EnableEmailErrorSend),            // указатель на переменную или адрес FRAM
  (void*)&EnableEmailErrorSendRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  EnableEmailErrorSendName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableEmailErrorSendList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Статус подключения модема
*************************************/
CPU_INT08U const ModemStatusName[] = "Статус";
CPU_INT08U const *ModemStatusList[] = {"Ок", "Ош.конф.", "Нет связи"};

TDataDescStruct const ModemStatusDesc = {
  DATA_DESC_VIEW,          // тип дескриптора
  DATA_TYPE_ULONG,         // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,           // признак массива
  0,                       // размер массива
  0,                       // указатель на десриптор индекса массива
  &modem_status,           // указатель на переменную или адрес FRAM
  NULL,                    // указатель на границы параметра
  NULL,                    // функция по изменению
  0,      // смещение между элементами в массиве
  ModemStatusName, // указатель на строку названия параметра
  DATA_IS_INDEX,           // признак индексного параметра (список строк)
  ModemStatusList,// указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Включить отправку журналов по e-mail
*************************************/
CPU_INT08U const EnableEmailJournalSendName[] = "Отпр.журналы";

TDataDescStruct const EnableEmailJournalSendDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.EnableEmailJournalSend),            // указатель на переменную или адрес FRAM
  (void*)&EnableEmailErrorSendRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  EnableEmailJournalSendName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableEmailErrorSendList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Очищать журнал после отправки по e-mail НЕ ИСПОЛЬЗУЕТСЯ
*************************************/
CPU_INT08U const ClearJournalAfterSendName[] = "Оч.журналы";

TDataDescStruct const ClearJournalAfterSendDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.ClearJournalAfterSend),            // указатель на переменную или адрес FRAM
  (void*)&EnableEmailErrorSendRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  ClearJournalAfterSendName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableEmailErrorSendList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
Время отправки статистики, час : мин
*************************************/
TRangeValueULONG const StatSendHourRange = {0, 60*24 - 1};
CPU_INT08U const StatSendHourName[] = "Tотпр.";

TDataDescStruct const StatSendHourMinDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_HOUR_MIN,    // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.StatSendHourMin),            // указатель на переменную или адрес FRAM
  (void*)&StatSendHourRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  StatSendHourName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  9                           
};

/*************************************
  Индекс при настройке сигналов
*************************************/
TRangeValueULONG const SignalIndexRange = {0, 5};
CPU_INT08U const SignalIndexName[] = "НАСТ.СИГН.ПЕЧ.";
CPU_INT08U const SignalIndex_str0[] = "1";
CPU_INT08U const SignalIndex_str1[] = "2";
CPU_INT08U const SignalIndex_str2[] = "3";
CPU_INT08U const SignalIndex_str3[] = "4";
CPU_INT08U const SignalIndex_str4[] = "5";
CPU_INT08U const SignalIndex_str5[] = "6";
CPU_INT08U const *SignalIndexList[] = {SignalIndex_str0, SignalIndex_str1,SignalIndex_str2,SignalIndex_str3,SignalIndex_str4,SignalIndex_str5};

CPU_INT32U CoinIndex = 0;

void OnChangeSignalIndex(void)
{
}

TDataDescStruct const SignalIndexDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)&CoinIndex,            // указатель на переменную или адрес FRAM
  (void*)&SignalIndexRange,     // указатель на границы параметра
  OnChangeSignalIndex,                     // функция по изменению
  0,       // смещение между элементами в массиве
  SignalIndexName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  SignalIndexList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Включение сигнала печати
*************************************/
TRangeValueULONG const EnableSignalRange = {0, 1};
CPU_INT08U const EnableSignalName[] = "Сигнал ";
CPU_INT08U const *EnableSignalList[] = {OnOffList_str0, OnOffList_str1};

void OnchangeEnableSignal(void)
{
}

TDataDescStruct const EnableSignalDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,// размер массива
  &SignalIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, enableSignalPrint),            // указатель на переменную или адрес FRAM
  (void*)&EnableSignalRange,     // указатель на границы параметра
  OnchangeEnableSignal,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  EnableSignalName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableSignalList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  Длительность импульса сигнала печати
*************************************/
TRangeValueULONG const SignalPulseRange = {1, 60};
CPU_INT08U const SignalPulseName[] = "Длина.имп.,сек.";

void OnChangeSinalPulseLen()
{
    CPU_INT32U pulse;
    for(int post = 0; post < COUNT_POST; post++)
    {
      GetData(&SignalPulseDesc, &pulse, post, DATA_FLAG_DIRECT_INDEX);
      SetSignalPulseParam(pulse, post);
    }
}

TDataDescStruct const SignalPulseDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,  // размер массива
  &SignalIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, signal_pulse_len),            // указатель на переменную или адрес FRAM
  (void*)&SignalPulseRange,     // указатель на границы параметра
  OnChangeSinalPulseLen,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  SignalPulseName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                         // значение по умолчанию
};

/*************************************
  Индекс при настройке монетоприемников
*************************************/
TRangeValueULONG const CoinIndexRange = {0, 7};
CPU_INT08U const CoinIndexName[] = "НАСТ.МОНЕТОПР.";
CPU_INT08U const CoinIndex_str0[] = "1";
CPU_INT08U const CoinIndex_str1[] = "2";
CPU_INT08U const CoinIndex_str2[] = "3";
CPU_INT08U const CoinIndex_str3[] = "4";
CPU_INT08U const CoinIndex_str4[] = "5";
CPU_INT08U const CoinIndex_str5[] = "6";
CPU_INT08U const CoinIndex_str6[] = "П1";
CPU_INT08U const CoinIndex_str7[] = "П2";
CPU_INT08U const *CoinIndexList[] = {CoinIndex_str0, CoinIndex_str1,CoinIndex_str2,CoinIndex_str3,CoinIndex_str4,CoinIndex_str5,CoinIndex_str6,CoinIndex_str7};

void OnChangeCoinIndex(void)
{
}

TDataDescStruct const CoinIndexDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)&CoinIndex,            // указатель на переменную или адрес FRAM
  (void*)&CoinIndexRange,     // указатель на границы параметра
  OnChangeCoinIndex,                     // функция по изменению
  0,       // смещение между элементами в массиве
  CoinIndexName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  CoinIndexList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Включение монетоприемника
*************************************/
TRangeValueULONG const EnableCoinRange = {0, 1};
CPU_INT08U const EnableCoinName[] = "Монетопр-к";
CPU_INT08U const *EnableCoinList[] = {OnOffList_str0, OnOffList_str1};

void OnchangeEnableCoin(void)
{
}

TDataDescStruct const EnableCoinDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST+COUNT_VACUUM,// размер массива
  &CoinIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.EnableCoinAcceptor),            // указатель на переменную или адрес FRAM
  (void*)&EnableCoinRange,     // указатель на границы параметра
  OnchangeEnableCoin,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  EnableCoinName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableCoinList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Цена импульса монетоприемника
*************************************/
TRangeValueULONG const CoinPerPulseRange = {1, 9999};
CPU_INT08U const CoinPerPulseName[] = "Руб./имп.";

TDataDescStruct const CoinPerPulseDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST+COUNT_VACUUM,  // размер массива
  &CoinIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.CoinPerPulse),            // указатель на переменную или адрес FRAM
  (void*)&CoinPerPulseRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  CoinPerPulseName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           // значение по умолчанию
};

/*************************************
  Таймаут печати чека монетоприемника
*************************************/
TRangeValueULONG const CoinTimeOutRange = {1, 999};
CPU_INT08U const CoinTimeOutName[] = "Таймаут";

void OnchangeTimeOutCoin(void)
{
  if(CoinIndex < COUNT_POST)
  {
      
  }
}

TDataDescStruct const CoinTimeOutDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST+COUNT_VACUUM,  // размер массива
  &CoinIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.CoinTimeOut),            // указатель на переменную или адрес FRAM
  (void*)&CoinTimeOutRange,     // указатель на границы параметра
  OnchangeTimeOutCoin,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  CoinTimeOutName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  30                        // значение по умолчанию
};

/*************************************
  Индекс при настройке купюроприемников
*************************************/
TRangeValueULONG const CashIndexRange = {0, 5};
CPU_INT08U const CashIndexName[] = "НАСТР.КУПЮРОПР.";

void OnChangeCashIndex(void)
{
}

TDataDescStruct const CashIndexDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)&CoinIndex,            // указатель на переменную или адрес FRAM
  (void*)&CashIndexRange,     // указатель на границы параметра
  OnChangeCashIndex,                     // функция по изменению
  0,       // смещение между элементами в массиве
  CashIndexName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  CoinIndexList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Индекс при настройке банковского терминала
*************************************/
TRangeValueULONG const BankIndexRange = {0, 5};
CPU_INT08U const BankIndexName[] = "НАСТР.БАНК.ТЕРМ";

void OnChangeBankIndex(void)
{
}

TDataDescStruct const BankIndexDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)&CoinIndex,            // указатель на переменную или адрес FRAM
  (void*)&BankIndexRange,     // указатель на границы параметра
  OnChangeBankIndex,                     // функция по изменению
  0,       // смещение между элементами в массиве
  BankIndexName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  CoinIndexList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Режим купюрника
*************************************/
TRangeValueULONG const CashModeRange = {0, 1};
CPU_INT08U const CashModeName[] = "Режим";
CPU_INT08U const CashMode_str0[] = "CCNet";
CPU_INT08U const CashMode_str1[] = "Pulse";
CPU_INT08U const *CashModeList[] = {CashMode_str0, CashMode_str1};

TDataDescStruct const CashModeDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &CashIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.CashMode),            // указатель на переменную или адрес FRAM
  (void*)&CashModeRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  CashModeName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  CashModeList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  Режим печати чека
*************************************/
TRangeValueULONG const PrintModeRange = {0, 1};
CPU_INT08U const PrintModeName[] = "Реж.печати";
CPU_INT08U const PrintMode_str0[] = "таймаут";
CPU_INT08U const PrintMode_str1[] = "кнопка";
CPU_INT08U const *PrintModeList[] = {PrintMode_str0, PrintMode_str1};

TDataDescStruct const PrintModeDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.PrintMode),            // указатель на переменную или адрес FRAM
  (void*)&PrintModeRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  PrintModeName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  PrintModeList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  Таймаут печати чека
*************************************/
TRangeValueULONG const PrintTimeoutRange = {0, 3600};
CPU_INT08U const PrintTimeoutName[] = "Таймаут печати";

TDataDescStruct const PrintTimeoutDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.PrintTimeout),            // указатель на переменную или адрес FRAM
  (void*)&PrintTimeoutRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,       // смещение между элементами в массиве
  PrintTimeoutName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  5                         // значение по умолчанию
};

/*************************************
  Таймаут отмены печати чека
*************************************/
TRangeValueULONG const PrintTimeoutAfterRange = {0, 60};
CPU_INT08U const PrintTimeoutAfterName[] = "Таймаут обнул.";

TDataDescStruct const PrintTimeoutAfterDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.PrintTimeoutAfter),            // указатель на переменную или адрес FRAM
  (void*)&PrintTimeoutAfterRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,       // смещение между элементами в массиве
  PrintTimeoutAfterName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  60                           // значение по умолчанию
};

/*************************************
  Цена импульса купюрника в импульсном режиме
*************************************/
TRangeValueULONG const CashPerPulseRange = {1, 9999};
CPU_INT08U const CashPerPulseName[] = "Руб./имп.";

TDataDescStruct const CashPerPulseDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &CashIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.CashPerPulse),            // указатель на переменную или адрес FRAM
  (void*)&CashPerPulseRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  CashPerPulseName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  10                           // значение по умолчанию
};

/*************************************
  Цена импульса банковского терминала в импульсном режиме
*************************************/
TRangeValueULONG const BankPerPulseRange = {1, 9999};
CPU_INT08U const BankPerPulseName[] = "Руб./имп.";

TDataDescStruct const BankPerPulseDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &BankIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.BankPerPulse),            // указатель на переменную или адрес FRAM
  (void*)&BankPerPulseRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  BankPerPulseName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  10                           // значение по умолчанию
};

void OnChangeLevel()
{
    CPU_INT32U level1, level2, level3, level4;
    for(int post = 0; post < COUNT_POST + COUNT_VACUUM; post++)
    {
      if(post < COUNT_POST)
      {
        GetData(&CashLevelDesc, &level1, post, DATA_FLAG_DIRECT_INDEX);
        GetData(&BankLevelDesc, &level2, post, DATA_FLAG_DIRECT_INDEX);
        GetData(&SignalStopMoneyLevelDesc, &level3, post, DATA_FLAG_DIRECT_INDEX);
      }
      
      GetData(&CoinLevelDesc, &level4, post, DATA_FLAG_DIRECT_INDEX);

      SetLevelParam(level1, level2, level3, level4, post);
    }
    
    #if OS_CRITICAL_METHOD == 3
    OS_CPU_SR  cpu_sr = 0;
    #endif
    OS_ENTER_CRITICAL();
    InitInputPorts();
    OS_EXIT_CRITICAL();
}

void OnChangeLevelWithoutInit()
{
    CPU_INT32U level1, level2, level3, level4;
    for(int post = 0; post < COUNT_POST + COUNT_VACUUM; post++)
    {
      if(post < COUNT_POST)
      {
        GetData(&CashLevelDesc, &level1, post, DATA_FLAG_DIRECT_INDEX);
        GetData(&BankLevelDesc, &level2, post, DATA_FLAG_DIRECT_INDEX);
        GetData(&SignalStopMoneyLevelDesc, &level3, post, DATA_FLAG_DIRECT_INDEX);
      }
      
      GetData(&CoinLevelDesc, &level4, post, DATA_FLAG_DIRECT_INDEX);

      SetLevelParam(level1, level2, level3, level4, post);
    }
}

/*************************************
  Уровень сигнала купюрника
*************************************/
TRangeValueULONG const LevelRange = {0, 1};
CPU_INT08U const LevelName[] = "Уровень";
CPU_INT08U const Level_str0[] = "LOW";
CPU_INT08U const Level_str1[] = "HIGH";
CPU_INT08U const *LevelList[] = {Level_str0, Level_str1};

TDataDescStruct const CashLevelDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &CashIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.CashLevel),            // указатель на переменную или адрес FRAM
  (void*)&LevelRange,     // указатель на границы параметра
  OnChangeLevel,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  LevelName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  LevelList,                  // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                          // значение по умолчанию
};

/*************************************
  Уровень сигнала банковскогог терминала
*************************************/
TDataDescStruct const BankLevelDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &BankIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.BankLevel),            // указатель на переменную или адрес FRAM
  (void*)&LevelRange,     // указатель на границы параметра
  OnChangeLevel,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  LevelName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  LevelList,                  // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                          // значение по умолчанию
};

/*************************************
  Уровень сигнала монетника
*************************************/
TDataDescStruct const CoinLevelDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST + COUNT_VACUUM,             // размер массива
  &CoinIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.CoinLevel),            // указатель на переменную или адрес FRAM
  (void*)&LevelRange,     // указатель на границы параметра
  OnChangeLevel,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  LevelName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  LevelList,                  // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                          // значение по умолчанию
};

/*************************************
  Уровень сигнала окончания внесения денег
*************************************/
CPU_INT08U const SignalStopLevelName[] = "Старт мойки ";

TDataDescStruct const SignalStopMoneyLevelDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &SignalIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.SignalLevel),            // указатель на переменную или адрес FRAM
  (void*)&LevelRange,     // указатель на границы параметра
  OnChangeLevel,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  SignalStopLevelName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  LevelList,                  // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                          // значение по умолчанию
};

/*************************************
  Уровень сигнала печати чека
*************************************/
CPU_INT08U const SignalPrintLevelName[] = "Печать чека";
CPU_INT08U const *SignalLevelList[] = {Level_str1, Level_str0};

TDataDescStruct const SignalPrintLevelDesc = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,             // размер массива
  &SignalIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.SignalLevel),            // указатель на переменную или адрес FRAM
  (void*)&LevelRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  SignalPrintLevelName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  SignalLevelList,                  // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                          // значение по умолчанию
};

/*************************************
  Включение ФР
*************************************/
TRangeValueULONG const EnableFiscalRange = {0, 1};
CPU_INT08U const EnableFiscalName[] = "ФР";
CPU_INT08U const *EnableFiscalList[] = {OnOffList_str0, OnOffList_str1};

TDataDescStruct const EnableFiscalDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.EnableFiscal),            // указатель на переменную или адрес FRAM
  (void*)&EnableFiscalRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  EnableFiscalName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableFiscalList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  Включение ФР
*************************************/
TRangeValueULONG const DisableFiscalErrorsRange = {0, 1};
CPU_INT08U const DisableFiscalErrorsName[] = "Игнорир.ош.ФР";
CPU_INT08U const DisableFiscalErrorsList_str0[] = "нет";
CPU_INT08U const DisableFiscalErrorsList_str1[] = "да";
CPU_INT08U const *DisableFiscalErrorsList[] = {DisableFiscalErrorsList_str0, DisableFiscalErrorsList_str1};

TDataDescStruct const DisableFiscalErrorsDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.DisableFiscalErrors),            // указатель на переменную или адрес FRAM
  (void*)&DisableFiscalErrorsRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  DisableFiscalErrorsName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  DisableFiscalErrorsList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  Цена 
*************************************/
TRangeValueULONG const PriceRange = {1, 10000};
CPU_INT08U const PriceName[] = "Цена,руб.";

TDataDescStruct const PriceDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,             // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  1,             // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, Price),                // указатель на переменную или адрес FRAM
  (void*)&PriceRange,           // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  PriceName,             // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  100                           
};

/*************************************
  Команда инициализации по умолчанию
*************************************/
CPU_INT32U InitByDefault = 0;

TRangeValueULONG const InitByDefaultRange = {0, 1};
CPU_INT08U const InitByDefaultName[] = "Инициализация";
CPU_INT08U const InitByDefaultList_str0[] = "нет";
CPU_INT08U const InitByDefaultList_str1[] = "да";
CPU_INT08U const *InitByDefaultList[] = {InitByDefaultList_str0, InitByDefaultList_str1};


void OnChangeInitByDefault(void)
{
  int i = 0;
  if (InitByDefault == 0) return;
  while (AllDataArray[i].ptr != NULL)
    {
      InitDescByDefault(AllDataArray[i].ptr);
      i++;
    } 
  InitByDefault = 0;
  
  CPU_INT08U flag[4] = {0xAA, 0x55, 0x81, 0xC3};
  char name[32] = "Услуги автомойки\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
  WriteArrayFram(offsetof(TFramMap, manual_service_flag), 4, (unsigned char*)&flag);
  WriteArrayFram(offsetof(TFramMap, manual_service_name), 32, (unsigned char*)&name);
}


TDataDescStruct const InitByDefaultDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,             // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)&InitByDefault,                // указатель на переменную или адрес FRAM
  (void*)&InitByDefaultRange,           // указатель на границы параметра
  OnChangeInitByDefault,                     // функция по изменению
  0,       // смещение между элементами в массиве
  InitByDefaultName,             // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  InitByDefaultList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};


/*************************************
  Команда на печать Z-отчета
*************************************/
CPU_INT32U PrintZReportCmd = 0;

CPU_INT08U const PrintZReportName[] = "Отч.о закр.см.";
CPU_INT08U const PrintZReportList_str0[] = "нет";
CPU_INT08U const PrintZReportList_str1[] = "печать";
CPU_INT08U const *PrintZReportList[] = {PrintZReportList_str0, PrintZReportList_str1};


void OnChangePrintZReportCmd(void)
{
}

TDataDescStruct const PrintZReportDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,             // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)&PrintZReportCmd,                // указатель на переменную или адрес FRAM
  (void*)&InitByDefaultRange,           // указатель на границы параметра
  OnChangePrintZReportCmd,                     // функция по изменению
  0,       // смещение между элементами в массиве
  PrintZReportName,             // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  PrintZReportList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};


/*************************************
  Команда на печать X-отчета
*************************************/
CPU_INT32U PrintXReportCmd = 0;

CPU_INT08U const PrintXReportName[] = "X-отчет";

void OnChangePrintXReportCmd(void)
{
}

TDataDescStruct const PrintXReportDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,             // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)&PrintXReportCmd,                // указатель на переменную или адрес FRAM
  (void*)&InitByDefaultRange,           // указатель на границы параметра
  OnChangePrintXReportCmd,                     // функция по изменению
  0,       // смещение между элементами в массиве
  PrintXReportName,             // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  PrintZReportList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Ошибка в журнале ошибок
*************************************/
TRangeValueULONG const ErrorNumberRange = {0, JOURNAL_EVENTS_COUNT - 1};
CPU_INT08U const *ErrorNumberList0[JOURNAL_EVENTS_COUNT] = 
                                    {"нет",
                                    
                                        "", "", "", "",
                                        "", "", "", "",
                                        "", "", "", "",
                                        
                                        "", "", "", "",
                                        "", "", "", "",
                                        "", "", "", "",

                                        "", "", "", "",
                                        "", "", "", "", "", "", "",

                                        "", "", "", "",
                                        "", "", "", "",
                                        "", "", "", "",

                                       "ФР:01h-Неизвестная",
                                       "ФР:02h-Неверное",
                                       "ФР:03h-Ошибка ФН",
                                       "ФР:04h-Ошибка КС",
                                       "ФР:05h-Закончен срок",  
                                       "ФР:06h-Архив ФН", 
                                       "ФР:07h-Неверные дата", 
                                       "ФР:08h-Нет ", 
                                       "ФР:09h-Некорр.", 
                                       "ФР:10h-Превышение", 
                                       "ФР:11h-Нет", 
                                       "ФР:12h-Исчерпан",
                                       "ФР:14h-Исчерпан", 
                                       "ФР:15h-Исчерпан", 
                                       "ФР:16h-Смена более",
                                       "ФР:17h-Неверная", 
                                      "ФР:20h-Сообщение", 
                                      "ФР:2Fh-Таймаут ",
                                      "ФР:30h-ФН ", 
                                      "ФР:33h-Некорректные", 
                                      "ФР:34h-Нет", 
                                      "ФР:35h-Некорректный", 
                                      "ФР:36h-Некорректные", 
                                      "ФР:37h-Команда", 
                                      "ФР:38h-Ошибка", 
                                      "ФР:39h-Внутренняя", 
                                      "ФР:3Ah-Переполнение", 
                                      "ФР:3Ch-Смена", 
                                      "ФР:3Dh-Смена", 
                                      "ФР:3Eh-Переполнение", 
                                      "ФР:3Fh-Переполнение", 
                                      "ФР:40h-Переполнение", 
                                      "ФР:41h-Переполнение", 
                                      "ФР:42h-Переполнение", 
                                      "ФР:43h-Переполнение", 
                                      "ФР:44h-Переполнение", 
                                      "ФР:45h-Cумма", 
                                      "ФР:46h-Не хватает", 
                                      "ФР:47h-Переполнение", 
                                      "ФР:48h-Переполнение", 
                                      "ФР:49h-Опер.невозм.", 
                                      "ФР:4Ah-Открыт чек", 
                                      "ФР:4Bh-Буфер чека", 
                                      "ФР:4Ch-Переполнение", 
                                      "ФР:4Dh-Вносимая", 
                                      "ФР:4Eh-Смена", 
                                      "ФР:4Fh-Неверный пароль", 
                                      "ФР:50h-Идет печать", 
                                      "ФР:51h-Переполнение", 
                                      "ФР:52h-Переполнение", 
                                      "ФР:53h-Переполнение", 
                                      "ФР:54h-Переполнение", 
                                      "ФР:55h-Чек закрыт", 
                                      "ФР:56h-Нет док.", 
                                      "ФР:58h-Ожидание", 
                                      "ФР:59h-Документ", 
                                      "ФР:5Bh-Переполнение", 
                                      "ФР:5Ch-Понижено", 
                                      "ФР:5Dh-Таблица", 
                                      "ФР:5Eh-Некорректная",
                                      "ФР:5Fh-Отриц.", 
                                      "ФР:60h-Переполнение",
                                      "ФР:61h-Переполнение",
                                      "ФР:62h-Переполнение",
                                      "ФР:63h-Переполнение",
                                      "ФР:65h-Не хватает",
                                      "ФР:66h-Переполнение",
                                      "ФР:68h-Не хватает",
                                      "ФР:69h-Переполнение",
                                      "ФР:6Ah-Ошибка",
                                      "ФР:6Bh-Нет чековой",
                                      "ФР:6Ch-Нет контр.",
                                      "ФР:6Dh-Не хватает",
                                      "ФР:6Eh-Переполнение",
                                      "ФР:6Fh-Переполнение",
                                      "ФР:71h-Ошибка", 
                                      "ФР:72h-Команда не", 
                                      "ФР:73h-Команда не", 
                                      "ФР:74h-Ошибка ОЗУ", 
                                      "ФР:75h-Ошибка", 
                                      "ФР:77h-Принтер:", 
                                      "ФР:78h-Замена ПО", 
                                      "ФП:7Ah-Поле не", 
                                      "ФР:7Bh-Ошибка", 
                                      "ФР:7Ch-Не совпадает", 
                                      "ФР:7Dh-Неверный", 
                                      "ФР:7Eh-Неверное", 
                                      "ФР:7Fh-Переполнение", 
                                      "ФР:84h-Переполнение", 
                                      "ФР:85h-Переполнение", 
                                      "ФР:86h-Переполнение", 
                                      "ФР:87h-Переполнение", 
                                      "ФР:88h-Переполнение", 
                                      "ФР:89h-Переполнение", 
                                      "ФР:8Ah-Переполнение", 
                                      "ФР:8Bh-Переполнение", 
                                      "ФР:8Ch-Отриц.", 
                                      "ФР:8Dh-Отрицательный", 
                                      "ФР:8Eh-Нулевой итог", 
                                      "ФР:90h-Поле прев.", 
                                      "ФР:91h-Выход за", 
                                      "ФР:92h-Наложение", 
                                      "ФР:93h-Восстановление", 
                                      "ФР:94h-Исчерпан", 
                                      "ФР:C0h-Контроль", 
                                      "ФР:C2h-Превышение", 
                                      "ФР:C4h-Несовпадение", 
                                      "ФР:C7h-Поле не", 
                                      "ФР:С8h-Отсутствуют"

};

TDataDescStruct const JournalErrorNumberDesc0 = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,             // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  EVENT_RECORDS_COUNT,             // размер массива
  &EventJournalIndexDesc,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, EventRecords[0].event),                // указатель на переменную или адрес FRAM
  (void*)&ErrorNumberRange,           // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(TEventRecord),       // смещение между элементами в массиве
  NULL,             // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  (void*)ErrorNumberList0,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

CPU_INT08U const *ErrorNumberList1[JOURNAL_EVENTS_COUNT] = 
                                       {
                                        "",

                                        "", "", "", "",
                                        "", "", "", "",
                                        "", "", "", "",

                                        "", "", "", "",
                                        "", "", "", "",
                                        "", "", "", "",
                                       
                                        "", "", "", "",
                                        "", "", "", "", "", "", "",

                                        "", "", "", "",
                                        "", "", "", "",
                                        "", "", "", "",

                                       "команда",  // ФР:01h
                                       "состояние ФН",                  
                                       "",
                                       "",                                       
                                       "эксплуатации ФН",
                                       "переполнен",
                                       "и/или время",
                                       "запрошенных данных",
                                       "знач.пар.к-ды",
                                       "разм.TLV данных",
                                       "трансп.соед.",
                                       "ресурс КС",
                                       "ресурс хранения",
                                       "ресурс ожидания",
                                       "24 часов",
                                       "разница во времени",
                                        "от ОФД не принято",
                                        "обмена с ФН",
                                        "не отвечает",
                                        "параметры в к-де",
                                        "данных",
                                        "параметр",
                                        "параметры",    // 36h
                                        "не поддерживается",
                                        "в ПЗУ",
                                        "ошибка ПО ККТ",
                                        "нак.по надб.",
                                        "откр.-операция невозм.",
                                        "закр.-операция невозм.",       //3Dh
                                        "накоп.по секциям",
                                        "накоп.по скидкам",
                                        "диапазона скидок",
                                        "диапазона оплаты",
                                        "диапазона оплаты 2",
                                        "диапазона оплаты 3",
                                        "диапазона оплаты 4",
                                        "меньше итога чека",
                                        "наличности в кассе",
                                        "накопления по налогам",
                                        "итога чека",
                                        "в откр.чеке",
                                        "операция невозможна",
                                        "переполнен",
                                        "накоп.по налогам",
                                        "безнал.сумма больше",
                                        "превысила 24 часа",
                                        "",
                                        "пред.команды",
                                        "накоплений в смене",
                                        "накоплений 2 в смене",
                                        "накоплений 3 в смене",
                                        "накоплений 4 в смене",
                                        "операция невозм.",
                                        "для повтора",
                                        "команды продолжения",
                                        "открыт другим оп.",
                                        "диапазона надбавок",
                                        "напряжение 24В",
                                        "не определена",
                                        "операция",
                                        "итог чека",
                                        "при умножении",
                                        "диапазона цены",
                                        "диапазона кол-ва",
                                        "диапазона отдела",
                                        "денег в секции",
                                        "денег в секции",
                                        "денег по налогам",
                                        "денег по налогам",
                                        "питания",
                                        "ленты",
                                        "ленты",
                                        "денег по налогу",
                                        "денег по налогу",
                                        "по выплате в смене",
                                        "отрезчика",
                                        "поддерж.",
                                        "поддерж.",
                                        "",
                                        "питания",
                                        "нет сигнала",
                                        "",
                                        "редактируется",
                                        "оборудования",
                                        "дата",
                                        "формат даты",
                                        "значение в поле длины",
                                        "диапазона итога чека",
                                        "наличности",
                                        "по продажам",
                                        "по покупкам",
                                        "по возвратам продаж",
                                        "по возвратам покупок",
                                        "по внесению в смене",
                                        "по надбавкам в чеке",
                                        "по скидкам в чеке",
                                        "итог надбавки",
                                        "итог скидки в чеке",
                                        "чека",
                                        "размер в настройках",
                                        "границу поля печати",
                                        "полей",
                                        "ОЗУ прошло успешно",
                                        "лимит операций в чеке",
                                        "даты и времени",
                                        "напряжения",
                                        "номеров смен",
                                        "редактируется",
                                        "импульсы тахо."
};

TDataDescStruct const JournalErrorNumberDesc1 = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,             // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  EVENT_RECORDS_COUNT,             // размер массива
  &EventJournalIndexDesc,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, EventRecords[0].event),                // указатель на переменную или адрес FRAM
  (void*)&ErrorNumberRange,           // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(TEventRecord),       // смещение между элементами в массиве
  NULL,             // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  (void*)ErrorNumberList1,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  Строка с ошибкой по-английски
*************************************/
CPU_INT08U const *ErrorNumberListEng[JOURNAL_EVENTS_COUNT] = 
{
  "Net oshibki", "", "", "",
  "", "", "", "",
  "", "", "", "",
  "", "", "", "",
  
  "Oshibka svyazi s modemom",
  "Oshibka svyazi s FR",  
  "Oshibka FR 0x01",
  "Oshibka FR 0x02",
  "Oshibka FR 0x03",
  "Oshibka FR 0x04",
  "Oshibka FR 0x05",
  "Oshibka FR 0x06",
  "Oshibka FR 0x07",
  "Oshibka FR 0x08",
  "Oshibka FR 0x09",
  "Oshibka FR 0x10",
  "Oshibka FR 0x11",
  "Oshibka FR 0x12",
  "Oshibka FR 0x14",
  "Oshibka FR 0x15",
  "Oshibka FR 0x16",
  "Oshibka FR 0x17",
  "Oshibka FR 0x20",
  "Oshibka FR 0x2F",
  "Oshibka FR 0x30",
  "Oshibka FR 0x33",
  "Oshibka FR 0x34",
  "Oshibka FR 0x35",
  "Oshibka FR 0x36",
  "Oshibka FR 0x37",
  "Oshibka FR 0x38",
  "Oshibka FR 0x39",
  "Oshibka FR 0x3A",
  "Oshibka FR 0x3C",
  "Oshibka FR 0x3D",
  "Oshibka FR 0x3E",
  "Oshibka FR 0x3F",
  "Oshibka FR 0x40",
  "Oshibka FR 0x41",
  "Oshibka FR 0x42",
  "Oshibka FR 0x43",
  "Oshibka FR 0x44",
  "Oshibka FR 0x45",
  "Oshibka FR 0x46",
  "Oshibka FR 0x47",
  "Oshibka FR 0x48",
  "Oshibka FR 0x49",
  "Oshibka FR 0x4A",
  "Oshibka FR 0x4B",
  "Oshibka FR 0x4C",
  "Oshibka FR 0x4D",
  "Oshibka FR 0x4E",
  "Oshibka FR 0x4F",
  "Oshibka FR 0x50",
  "Oshibka FR 0x51",
  "Oshibka FR 0x52",
  "Oshibka FR 0x53",
  "Oshibka FR 0x54",
  "Oshibka FR 0x55",
  "Oshibka FR 0x56",
  "Oshibka FR 0x58",
  "Oshibka FR 0x59",
  "Oshibka FR 0x5B",
  "Oshibka FR 0x5C",
  "Oshibka FR 0x5D",
  "Oshibka FR 0x5E",
  "Oshibka FR 0x5F",
  "Oshibka FR 0x60",
  "Oshibka FR 0x61",
  "Oshibka FR 0x62",
  "Oshibka FR 0x63",
  "Oshibka FR 0x65",
  "Oshibka FR 0x66",
  "Oshibka FR 0x68",
  "Oshibka FR 0x69",
  "Oshibka FR 0x6A",
  "Oshibka FR 0x6B",
  "Oshibka FR 0x6C",
  "Oshibka FR 0x6D",
  "Oshibka FR 0x6E",
  "Oshibka FR 0x6F",
  "Oshibka FR 0x71",
  "Oshibka FR 0x72",
  "Oshibka FR 0x73",
  "Oshibka FR 0x74",
  "Oshibka FR 0x75",
  "Oshibka FR 0x77",
  "Oshibka FR 0x78",
  "Oshibka FR 0x7A",
  "Oshibka FR 0x7B",
  "Oshibka FR 0x7C",
  "Oshibka FR 0x7D",
  "Oshibka FR 0x7E",
  "Oshibka FR 0x7F",
  "Oshibka FR 0x84",
  "Oshibka FR 0x85",
  "Oshibka FR 0x86",
  "Oshibka FR 0x87",
  "Oshibka FR 0x88",
  "Oshibka FR 0x89",
  "Oshibka FR 0x8A",
  "Oshibka FR 0x8B",
  "Oshibka FR 0x8C",
  "Oshibka FR 0x8D",
  "Oshibka FR 0x8E",
  "Oshibka FR 0x90",
  "Oshibka FR 0x91",
  "Oshibka FR 0x92",
  "Oshibka FR 0x93",
  "Oshibka FR 0x94",
  "Oshibka FR 0xC0",
  "Oshibka FR 0xC2",
  "Oshibka FR 0xC4",
  "Oshibka FR 0xC7",
  "Oshibka FR 0xC8"
};
  
TDataDescStruct const JournalErrorNumberDescEng = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,             // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  EVENT_RECORDS_COUNT,             // размер массива
  &EventJournalIndexDesc,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, EventRecords[0].event),                // указатель на переменную или адрес FRAM
  (void*)&ErrorNumberRange,           // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(TEventRecord),       // смещение между элементами в массиве
  NULL,             // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  (void*)ErrorNumberList1,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  Индекс при просмотре журнала событий
*************************************/
TRangeValueULONG const EventJournalIndexRange = {0, 0xffffffff};
CPU_INT08U const EventJournalIndexName[] = "Запись #";
CPU_INT32U EventJournalIndex = 0;
CPU_INT32U last_Index = 0;

void OnChangeEventJournalIndex(void)
{
  TEventRecord record;
  
  if (EventJournalIndex == 0xffffffff) EventJournalIndex = EVENT_RECORDS_COUNT - 1;
  else if (EventJournalIndex > EVENT_RECORDS_COUNT - 1) EventJournalIndex = 0;

  GetEventRecord(&record, EventJournalIndex);
  
  if(record.time == 0) 
  {
    EventJournalIndex = last_Index;
    return;
  }
  
  last_Index = EventJournalIndex;
  PrintEventJournalRecord(&record);
}

TDataDescStruct const EventJournalIndexDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)&EventJournalIndex,            // указатель на переменную или адрес FRAM
  (void*)&EventJournalIndexRange,     // указатель на границы параметра
  OnChangeEventJournalIndex,                     // функция по изменению
  0,       // смещение между элементами в массиве
  EventJournalIndexName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  Время события в журнале событий
*************************************/
TDataDescStruct const JournalEventTimeDesc = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_TIME,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  EVENT_RECORDS_COUNT,             // размер массива
  &EventJournalIndexDesc,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, EventRecords[0].time),            // указатель на переменную или адрес FRAM
  NULL,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(TEventRecord),       // смещение между элементами в массиве
  NULL,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Системное время
*************************************/
extern CPU_INT32U SystemTime;

TDataDescStruct const SystemTimeDesc = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_TIME,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)&SystemTime,            // указатель на переменную или адрес FRAM
  NULL,     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,       // смещение между элементами в массиве
  NULL,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};


void OnSetTime(void)
{
  TRTC_Data rtc;
  Sec2Date(&rtc, SystemTime);
  RTC_SetTime(&rtc);
}

TDataDescStruct const SystemTimeEditDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_TIME,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)&SystemTime,            // указатель на переменную или адрес FRAM
  NULL,     // указатель на границы параметра
  OnSetTime,                     // функция по изменению
  0,       // смещение между элементами в массиве
  NULL,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  Команда на очистку журналов
*************************************/
CPU_INT32U ClearJournalCmd = 0;

CPU_INT08U const ClearJournalCmdName[] = "Очистка";
CPU_INT08U const ClearJournalCmdList_str0[] = "нет";
CPU_INT08U const ClearJournalCmdList_str1[] = "да";
CPU_INT08U const *ClearJournalCmdList[] = {ClearJournalCmdList_str0, ClearJournalCmdList_str1};

void OnChangeClearJournalCmd(void)
{
  if (ClearJournalCmd)
    {
      ClearEventJournal();
      ClearJournalCmd = 0;
    }
}

TDataDescStruct const ClearJournalCmdDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,             // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)&ClearJournalCmd,                // указатель на переменную или адрес FRAM
  (void*)&InitByDefaultRange,           // указатель на границы параметра
  OnChangeClearJournalCmd,                     // функция по изменению
  0,       // смещение между элементами в массиве
  ClearJournalCmdName,             // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  ClearJournalCmdList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};


/*************************************
  Общий счетчик числа запусков
*************************************/
CPU_INT08U const CounterRunName[] = "Запуски";

TDataDescStruct const CounterRunDesc = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, Counters.CounterRun),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  CounterRunName,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Общий счетчик денег
*************************************/
CPU_INT08U const CounterMoneyName[] = "Деньги,руб.";

TDataDescStruct const CounterMoneyDesc = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, Counters.CounterMoney),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  CounterMoneyName,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Общий счетчик времени работы
*************************************/
CPU_INT08U const CounterTimeName[] = "Вр.раб.";

TDataDescStruct const CounterTimeDesc = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_TIME_COUNT,     // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, Counters.CounterTime),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  CounterTimeName,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  Общий счетчик числа запусков ДЛИННЫЙ
*************************************/
TDataDescStruct const CounterLongRunDesc = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, CountersLong.CounterRunLong),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  CounterRunName,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Общий счетчик денег ДЛИННЫЙ 
*************************************/
TDataDescStruct const CounterLongMoneyDesc = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, CountersLong.CounterMoneyLong),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  CounterMoneyName,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Общий счетчик времени работы
*************************************/
TDataDescStruct const CounterLongTimeDesc = {
  DATA_DESC_VIEW,           // тип дескриптора
  DATA_TYPE_TIME_COUNT,     // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, CountersLong.CounterTimeLong),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  CounterTimeName,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Команда на очистку статистики 
*************************************/
CPU_INT32U ClearStatCmd = 0;

CPU_INT08U const ClearStatCmdName[] = "Очистка";

void OnChangeClearStatCmd(void)
{
  if (ClearStatCmd)
    {
      ClearStatCmd = 0;
    }
}

TDataDescStruct const ClearStatCmdDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,             // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)&ClearStatCmd,                // указатель на переменную или адрес FRAM
  (void*)&InitByDefaultRange,           // указатель на границы параметра
  OnChangeClearStatCmd,                     // функция по изменению
  0,       // смещение между элементами в массиве
  ClearJournalCmdName,             // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  ClearJournalCmdList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};


/*************************************
  Разрешение автоматического закрытия смен
*************************************/
TRangeValueULONG const EnableFiscalDayClearRange = {0, 1};
CPU_INT08U const EnableFiscalDayClearName[] = "Закр.смены";
CPU_INT08U const *EnableFiscalDayClearList[] = {"Руч.", "Авто", "Буфер"};

TDataDescStruct const EnableFiscalDayClearDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.EnableFiscalDayClear),            // указатель на переменную или адрес FRAM
  (void*)&EnableFiscalDayClearRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  EnableFiscalDayClearName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  EnableFiscalDayClearList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           // значение по умолчанию
};


/*************************************
  Настройка чека
*************************************/
TRangeValueULONG const BillFormatRange = {0, 1};
CPU_INT08U const BillFormatName[] = "Чек:";
CPU_INT08U const BillFormatList_str0[] = "кол-во*цена";
CPU_INT08U const BillFormatList_str1[] = "сумма";
CPU_INT08U const *BillFormatList[] = {BillFormatList_str0, BillFormatList_str1};

TDataDescStruct const BillFormatDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.BillFormat),            // указатель на переменную или адрес FRAM
  (void*)&BillFormatRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  BillFormatName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  BillFormatList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  Система налогообложения
*************************************/
TRangeValueULONG const TaxSystemRange = {0, 5};
CPU_INT08U const TaxSystemName[] = "Система нал.";
CPU_INT08U const TaxSystemList_str0[] = "ОСН";
CPU_INT08U const TaxSystemList_str1[] = "УСН д";
CPU_INT08U const TaxSystemList_str2[] = "УСН д-р";
CPU_INT08U const TaxSystemList_str3[] = "ЕНВД";
CPU_INT08U const TaxSystemList_str4[] = "ЕСН";
CPU_INT08U const TaxSystemList_str5[] = "ПСН";

CPU_INT08U const *TaxSystemList[] = {TaxSystemList_str0, TaxSystemList_str1, TaxSystemList_str2, TaxSystemList_str3, TaxSystemList_str4, TaxSystemList_str5};

TDataDescStruct const TaxSystemDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, TaxSystem),            // указатель на переменную или адрес FRAM
  (void*)&TaxSystemRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  TaxSystemName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  TaxSystemList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Индекс налога
*************************************/
TRangeValueULONG const TaxFormatRange = {1, 6};
CPU_INT08U const TaxFormatName[] = "Налог:";
CPU_INT08U const TaxFormatList_str0[] = "откл.";
CPU_INT08U const TaxFormatList_str1[] = "  1  ";
CPU_INT08U const TaxFormatList_str2[] = "  2  ";
CPU_INT08U const TaxFormatList_str3[] = "  3  ";
CPU_INT08U const TaxFormatList_str4[] = "  4  ";
CPU_INT08U const TaxFormatList_str5[] = "  5  ";
CPU_INT08U const TaxFormatList_str6[] = "  6  ";
CPU_INT08U const *TaxFormatList[] = {TaxFormatList_str0, TaxFormatList_str1, TaxFormatList_str2, TaxFormatList_str3, TaxFormatList_str4, TaxFormatList_str5, TaxFormatList_str6};

TDataDescStruct const TaxFormatDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, TaxFormat),            // указатель на переменную или адрес FRAM
  (void*)&TaxFormatRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  TaxFormatName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  TaxFormatList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  6                           
};

/*************************************
  Предмет расчета
*************************************/
TRangeValueULONG const SubjSellRange = {0, 2};
CPU_INT08U const SubjSellName[] = "Предм.расчета";
CPU_INT08U const SubjSellList_str0[] = "ТОВАР";
CPU_INT08U const SubjSellList_str1[] = "РАБОТА";
CPU_INT08U const SubjSellList_str2[] = "УСЛУГА";
CPU_INT08U const *SubjSellList[] = {SubjSellList_str0, SubjSellList_str1, SubjSellList_str2};

TDataDescStruct const SubjSellDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, SubjSell),            // указатель на переменную или адрес FRAM
  (void*)&SubjSellRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  SubjSellName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  SubjSellList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Тип команд работы с чеком
*************************************/
TRangeValueULONG const CommandV2Range = {0, 1};
CPU_INT08U const CommandV2Name[] = "Тип команд";
CPU_INT08U const CommandV2List_str0[] = "старый";
CPU_INT08U const CommandV2List_str1[] = "V2";
CPU_INT08U const *CommandV2List[] = {CommandV2List_str0, CommandV2List_str1};

TDataDescStruct const CommandV2Desc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, CommandV2),            // указатель на переменную или адрес FRAM
  (void*)&CommandV2Range,     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  CommandV2Name,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  CommandV2List,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  Название услуги
*************************************/
TRangeValueULONG const ServiceNameRange = {0, 1};
CPU_INT08U const ServiceNameName[] = "Товар";
CPU_INT08U const ServiceNameList_str0[] = "Жетон";
CPU_INT08U const ServiceNameList_str1[] = "Услуги мойки";
CPU_INT08U const *ServiceNameList[] = {ServiceNameList_str0, ServiceNameList_str1};

void OnChangeServiceName(void)
{
    CPU_INT08U flag[4] = {0, 0, 0, 0};

    WriteArrayFram(offsetof(TFramMap, manual_service_flag), 4, (unsigned char*)&flag);
}

TDataDescStruct const ServiceNameDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.ServiceName),            // указатель на переменную или адрес FRAM
  (void*)&ServiceNameRange,     // указатель на границы параметра
  OnChangeServiceName,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  ServiceNameName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  ServiceNameList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  1                           // значение по умолчанию
};

/*************************************
  Дескриптор для энергонезависимого сохранения текущих денег
*************************************/
TDataDescStruct const AcceptedMoneyDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST + COUNT_VACUUM,// размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, FRAM_AcceptedMoney),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  NULL,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Дескриптор Crc16 энергонезависимого сохранения текущих денег
*************************************/
TDataDescStruct const AcceptedMoneyCRC16Desc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST+COUNT_VACUUM,  // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, crc_AcceptedMoney),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  NULL,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Дескриптор для энергонезависимого сохранения текущих денег
*************************************/
TDataDescStruct const AcceptedBankMoneyDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,// размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, FRAM_AcceptedBankMoney),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  NULL,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Дескриптор Crc16 энергонезависимого сохранения текущих денег
*************************************/
TDataDescStruct const AcceptedBankMoneyCRC16Desc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_IS_ARRAY,            // признак массива
  COUNT_POST,  // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, crc_AcceptedBankMoney),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  NULL,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Дескриптор пароля
*************************************/
TRangeValueULONG const PassRange = {0, 9999};
CPU_INT08U const PassName[] = "  ПАРОЛb";

void OnChangePass(void)
{
  // обновим CRC пароля  
  CPU_INT32U pass,crc;
  GetData(&PassDesc, &pass, 0, DATA_FLAG_SYSTEM_INDEX);      
  crc = crc16((unsigned char*)&pass, sizeof(CPU_INT32U));
  SetData(&PassCRCDesc, &crc, 0, DATA_FLAG_SYSTEM_INDEX);
}

TDataDescStruct const PassDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, Pass),            // указатель на переменную или адрес FRAM
  (void*)&PassRange,                     // указатель на границы параметра
  OnChangePass,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  (void*)&PassName,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  DEFAULT_PASSWORD                           
};

/*************************************
  Дескриптор CRC пароля
*************************************/
TDataDescStruct const PassCRCDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, crc_Pass),            // указатель на переменную или адрес FRAM
  NULL,                     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  NULL,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Дескриптор временного пароля
*************************************/
CPU_INT32U TempPass = 0;

void OnChangeTempPass(void)
{
  CPU_INT32U pass;
  GetData(&PassDesc, &pass, 0, DATA_FLAG_SYSTEM_INDEX);
    
  if (GetCurrentMenu() == SetPassMenuPanel)
    { // утановка нового пароля
      if (pass != TempPass)
      {
        SaveEventRecord(0, JOURNAL_EVENT_PASS_FAIL, TempPass);
        GoToMenu(ErrorPassPanel);
      }
      else {GoToPreviousMenu(); GoToMenu(SetNewPassMenuPanel);}
    }
  else if (GetCurrentMenu() == ResetSettingsMenuPanel)
    { // сброс настроек
      if (pass != TempPass) 
      {
        SaveEventRecord(0, JOURNAL_EVENT_PASS_FAIL, TempPass);
        GoToMenu(ErrorPassPanel);
      }
      else {InitByDefault = 1; OnChangeInitByDefault(); GoToPreviousMenu(); GoToMenu(SettingsIsReset);}
    }
  else if (GetCurrentMenu() == ClearStatMenu)
    { // очистка статистики
      if (pass != TempPass)
      {
        SaveEventRecord(0, JOURNAL_EVENT_PASS_FAIL, TempPass);
        GoToMenu(ErrorPassPanel);
      }
      else {ClearStatCmd = 1; OnChangeClearStatCmd(); GoToPreviousMenu(); GoToMenu(StatIsReset);}
    }
  else if (GetCurrentMenu() == ClearJournalMenuPanel)
    { // очистка статистики
      if (pass != TempPass) 
      {
        SaveEventRecord(0, JOURNAL_EVENT_PASS_FAIL, TempPass);
        GoToMenu(ErrorPassPanel);
      }
      else {ClearJournalCmd = 1; OnChangeClearJournalCmd(); GoToPreviousMenu(); GoToMenu(JournalIsReset);}
    }
}

TDataDescStruct const PassTempDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)&TempPass,            // указатель на переменную или адрес FRAM
  (void*)&PassRange,                     // указатель на границы параметра
  OnChangeTempPass,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  (void*)&PassName,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};


CPU_INT08U const PassTempName1[] = "    ПАРОЛb";

TDataDescStruct const PassTempDesc1 = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)&TempPass,            // указатель на переменную или адрес FRAM
  (void*)&PassRange,                     // указатель на границы параметра
  OnChangeTempPass,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  (void*)&PassTempName1,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

TRangeValueULONG const MasterPassRange = {0, 99999999};
CPU_INT08U const MasterPassTempName[] = "ПАРОЛb";

void OnChangeMasterPass(void)
{
    CPU_INT32U pass, crc;
        
    if (TempPass == MASTER_PASSWORD)
    {
        TempPass = 0;
        pass = DEFAULT_PASSWORD;
        crc = crc16((unsigned char*)&pass, sizeof(CPU_INT32U));
        SetData(&PassDesc, &pass, 0, DATA_FLAG_SYSTEM_INDEX);
        SetData(&PassCRCDesc, &crc, 0, DATA_FLAG_SYSTEM_INDEX);
        
        GoToPreviousMenu();
        GoToPreviousMenu();
        GoToMenu(SetNewPassMenuPanel);
    }
}

TDataDescStruct const MasterPassTempDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)&TempPass,            // указатель на переменную или адрес FRAM
  (void*)&MasterPassRange,                     // указатель на границы параметра
  OnChangeMasterPass,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  (void*)&MasterPassTempName,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

CPU_INT08U const PassTempName2[] = "   ПАРОЛb";

TDataDescStruct const PassTempDesc2 = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,                        // размер массива
  NULL,                     // указатель на десриптор индекса массива
  (void*)&TempPass,            // указатель на переменную или адрес FRAM
  (void*)&PassRange,                     // указатель на границы параметра
  OnChangeTempPass,                     // функция по изменению
  0,                        // смещение между элементами в массиве
  (void*)&PassTempName2,           // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  Команда послать тестовое сообщение
*************************************/
CPU_INT08U const SendTestEmailName[] = "Отпр.тест";
CPU_INT32U send_test;

void OnChangeSendTestEmail(void)
{
    if (send_test)
    {
        //PostModemTask(MODEM_TASK_SEND_TEST_MSG);
        send_test = 0;
    }
}

TDataDescStruct const SendTestEmailDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_RAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)&send_test,            // указатель на переменную или адрес FRAM
  (void*)&EnableEmailErrorSendRange,     // указатель на границы параметра
  OnChangeSendTestEmail,                     // функция по изменению
  sizeof(CPU_INT32U),       // смещение между элементами в массиве
  SendTestEmailName,       // указатель на строку названия параметра
  DATA_IS_INDEX,            // признак индексного параметра (список строк)
  DisableFiscalErrorsList,                     // указатель на список строк для индексного параметра
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ID устройства 
*************************************/
CPU_INT08U const DeviceIDName[] = "ID уст-ва";
TRangeValueULONG const DeviceIDRange = {0, 9999};

TDataDescStruct const DeviceIDDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_ULONG,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  0,             // размер массива
  0,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, DeviceConfig.DeviceId),            // указатель на переменную или адрес FRAM
  (void*)&DeviceIDRange,     // указатель на границы параметра
  NULL,                     // функция по изменению
  0,       // смещение между элементами в массиве
  DeviceIDName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  NULL,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  IP-адрес свой
*************************************/
void OnChangeIpMask()
{
    CPU_INT32U ip, mask;
    GetData(&IpAddrDesc, &ip, 0, DATA_FLAG_SYSTEM_INDEX);  
    GetData(&NetMaskDesc, &mask, 0, DATA_FLAG_SYSTEM_INDEX);  
    NetIP_CfgAddrThisHost(ip, mask);
}

TRangeValueULONG const DeferredStartRange = {0, 1};
CPU_INT08U const DeferredStart_str0[] = "нет";
CPU_INT08U const DeferredStart_str1[] = "да";
CPU_INT08U const *DeferredStartList[] = {DeferredStart_str0, DeferredStart_str1, NULL};

CPU_INT08U const IpAddrName[] = "IP";

TDataDescStruct const IpAddrDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_IP_ADDR,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  1,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, ip),            // указатель на переменную или адрес FRAM
  (void*)&DeferredStartRange,     // указатель на границы параметра
  OnChangeIpMask,                     // функция по изменению
  0,       // смещение между элементами в массиве
  IpAddrName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  DeferredStartList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0xC0A80064
};

/*************************************
  Меска сети
*************************************/
CPU_INT08U const NetMaskName[] = "MSK";

TDataDescStruct const NetMaskDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_IP_ADDR,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  1,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, netmask),            // указатель на переменную или адрес FRAM
  (void*)&DeferredStartRange,     // указатель на границы параметра
  OnChangeIpMask,                     // функция по изменению
  0,       // смещение между элементами в массиве
  NetMaskName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  DeferredStartList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0xFFFFFF00
};

/*************************************
  Шлюз
*************************************/

void OnChangeGateway()
{
    CPU_INT32U gw;
    GetData(&GatewayDesc, &gw, 0, DATA_FLAG_SYSTEM_INDEX);  
    NetIP_CfgAddrDfltGateway(gw);
}
    
CPU_INT08U const GatewayName[] = "GW";

TDataDescStruct const GatewayDesc = {
  DATA_DESC_EDIT,           // тип дескриптора
  DATA_TYPE_IP_ADDR,          // тип параметра
  DATA_LOC_FRAM,            // расположение параметра
  DATA_NO_ARRAY,            // признак массива
  1,             // размер массива
  NULL,        // указатель на десриптор индекса массива
  (void*)offsetof(TFramMap, gateway),            // указатель на переменную или адрес FRAM
  (void*)&DeferredStartRange,     // указатель на границы параметра
  OnChangeGateway,                     // функция по изменению
  0,       // смещение между элементами в массиве
  GatewayName,       // указатель на строку названия параметра
  DATA_NO_INDEX,            // признак индексного параметра (список строк)
  DeferredStartList,                     // указатель на список строк для индексного параметра
  DATA_INIT_DISABLE,
  0xC0A80001
};

//**************************************************
const TDataDescArrayStruct AllDataArray[] = 
{
    {&EnableValidatorDesc,  "EnableValidatorDesc"},
    {&EnableModemDesc,  "EnableModemDesc"},
    {&EnableFiscalDesc,  "EnableFiscalDesc"},
    {&EnableCoinDesc, "EnableCoinDesc"},
    
    {&PrintZReportDesc, "PrintZReportDesc"},
    {&PrintXReportDesc, "PrintXReportDesc"},
    {&SystemTimeDesc, "SystemTimeDesc"},
    {&SystemTimeEditDesc, "SystemTimeEditDesc"},
    {&CoinPerPulseDesc, "CoinPerPulseDesc"},
    
    {&BillFormatDesc, "BillFormatDesc"},
    
    {&PassDesc, "PassDesc"},
    {&DeviceIDDesc, "DeviceIDDesc"},
    
    {&EnableEmailErrorSendDesc, "EnableEmailErrorSendDesc"},
    {&EnableEmailJournalSendDesc, "EnableEmailJournalSendDesc"},
    {&ClearJournalAfterSendDesc, "ClearJournalAfterSendDesc"},
    {&StatSendHourMinDesc, "StatSendHourMinDesc"},
    {&SendTestEmailDesc, "SendTestEmailDesc"},
    
    {&StartButtonNameDesc, "StartButtonNameDesc"},
    
    {&GatewayDesc, "GatewayDesc"},
    {&NetMaskDesc, "NetMaskDesc"},
    {&IpAddrDesc, "IpAddrDesc"},
    
    {&CashModeDesc, "CashModeDesc"},
    {&CashPerPulseDesc, "CashPerPulseDesc"},

    {&EnableFiscalDayClearDesc, "EnableFiscalDayClearDesc"},
    {&PrintTimeoutDesc, "PrintTimeoutDesc"},

    {&PriceDesc, "PriceDesc"},

    {&ServiceNameDesc, "ServiceNameDesc"},

    {&CashPulseLenDesc, "CashPulseLenDesc"},
    {&CashPauseLenDesc, "CashPauseLenDesc"},
  
    {&CoinPulseLenDesc, "CoinPulseLenDesc"},
    {&CoinPauseLenDesc, "CoinPauseLenDesc"},
  
    {&TaxSystemDesc, "TaxSystemDesc"},
    {&TaxFormatDesc, "TaxFormatDesc"},
    {&SubjSellDesc, "SubjSellDesc"},
    {&CommandV2Desc, "CommandV2Desc"},
    {&ServiceNameDesc, "ServiceNameDesc"},

    {&PrintModeDesc, "PrintModeDesc"},
    {&PrintTimeoutAfterDesc, "PrintTimeoutAfterDesc"},
    {&DisableFiscalErrorsDesc, "DisableFiscalErrorsDesc"},
    {&EnableSignalDesc, "EnableSignalDesc"},
    {&SignalPulseDesc, "SignalPulseDesc"},
    
    {&CashLevelDesc, "CashLevelDesc"},
    {&CoinLevelDesc, "CoinLevelDesc"},
    {&SignalStopMoneyLevelDesc, "SignalStopMoneyLevelDesc"},
    
    {&EnableBankDesc, "EnableBankDesc"},
    {&BankPerPulseDesc, "BankPerPulseDesc"},
    {&BankPulseLenDesc, "BankPulseLenDesc"},
    {&BankPauseLenDesc, "BankPauseLenDesc"},
    {&BankLevelDesc, "BankLevelDesc"},

    {&CoinTimeOutDesc, "CoinTimeOutDesc"},

    {NULL, ""}
};


