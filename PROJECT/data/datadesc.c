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
#include "app_serv.h"

extern CPU_INT32U modem_status;

/*************************************
  ��������� ����� �������� email
*************************************/
extern TRangeValueULONG const WorkTimeRange;

/*************************************
  ����� �������� ����� ���������������, ��
*************************************/
TRangeValueULONG const CoinPulseLenRange = {20, 250};
CPU_INT08U const CoinPulseLenName[] = "����� ���.,��";

void OnChangeCoinPulseLen()
{
    CPU_INT32U pulse, pause;
    for(int post = 0; post < COUNT_POST + COUNT_VACUUM; post++)
    {
      GetData(&CoinPulseLenDesc, &pulse, post, DATA_FLAG_DIRECT_INDEX);
      GetData(&CoinPauseLenDesc, &pause, post, DATA_FLAG_DIRECT_INDEX);
      SetCoinPulseParam(pulse, pause, post);
    }
}

TDataDescStruct const CoinPulseLenDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST + COUNT_VACUUM,             // ������ �������
  &CoinIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, coin_pulse_len),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CoinPulseLenRange,     // ��������� �� ������� ���������
  OnChangeCoinPulseLen,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  CoinPulseLenName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  50                           
};

/*************************************
  ����� �������� ����� ���������, ��
*************************************/
TRangeValueULONG const CashPulseLenRange = {20, 250};
CPU_INT08U const CashPulseLenName[] = "����� ���.,��";

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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &CashIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, cash_pulse_len),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CashPulseLenRange,     // ��������� �� ������� ���������
  OnChangeCashPulseLen,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  CashPulseLenName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  50                           
};

/*************************************
  ����� �������� ����� ����������� ���������, ��
*************************************/
TRangeValueULONG const BankPulseLenRange = {20, 250};
CPU_INT08U const BankPulseLenName[] = "����� ���.,��";

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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &BankIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, bank_pulse_len),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&BankPulseLenRange,     // ��������� �� ������� ���������
  OnChangeBankPulseLen,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  BankPulseLenName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  50                           
};

/*************************************
  ����� ����� ����� ���������, ��
*************************************/
TRangeValueULONG const CoinPauseLenRange = {20, 250};
CPU_INT08U const CoinPauseLenName[] = "����� ���.,��";

TDataDescStruct const CoinPauseLenDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST + COUNT_VACUUM,             // ������ �������
  &CoinIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, coin_pause_len),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CoinPauseLenRange,     // ��������� �� ������� ���������
  OnChangeCoinPulseLen,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  CoinPauseLenName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  20                           
};
/*************************************
  ����� ����� ����� ���������, ��
*************************************/
TRangeValueULONG const CashPauseLenRange = {20, 250};
CPU_INT08U const CashPauseLenName[] = "����� ���.,��";

TDataDescStruct const CashPauseLenDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &CashIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, cash_pause_len),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CashPauseLenRange,     // ��������� �� ������� ���������
  OnChangeCashPulseLen,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  CashPauseLenName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  20                           
};

/*************************************
  ����� ����� ����� ����������� ���������, ��
*************************************/
TRangeValueULONG const BankPauseLenRange = {20, 250};
CPU_INT08U const BankPauseLenName[] = "����� ���.,��";

TDataDescStruct const BankPauseLenDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &BankIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, bank_pause_len),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&BankPauseLenRange,     // ��������� �� ������� ���������
  OnChangeBankPulseLen,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  BankPauseLenName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  20                           
};

/*************************************
  �������� ������ ����� (����� ��� ����)
*************************************/
TRangeValueULONG const StartButtonNameRange = {0, 1};
CPU_INT08U const StartButtonNameName[] = "������";
CPU_INT08U const StartButtonNameList_str0[] = "�����";
CPU_INT08U const StartButtonNameList_str1[] = "����";
CPU_INT08U const *StartButtonNameList[] = {StartButtonNameList_str0, StartButtonNameList_str1};


TDataDescStruct const StartButtonNameDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, StartButtonName),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&StartButtonNameRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  StartButtonNameName,      // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  StartButtonNameList,      // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  ��������� ���������������
*************************************/
TRangeValueULONG const EnableValidatorRange = {0, 1};
CPU_INT08U const EnableValidatorName[] = "��������-�";
CPU_INT08U const OnOffList_str0[] = "����.";
CPU_INT08U const OnOffList_str1[] = "���.";
CPU_INT08U const *EnableValidatorList[] = {OnOffList_str0, OnOffList_str1};

TDataDescStruct const EnableValidatorDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &CashIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.EnableValidator),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableValidatorRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  EnableValidatorName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableValidatorList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           // �������� �� ���������
};

/*************************************
  ��������� ����������� ���������
*************************************/
TRangeValueULONG const EnableBankRange = {0, 1};
CPU_INT08U const EnableBankName[] = "����.������.";

TDataDescStruct const EnableBankDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &BankIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.EnableBank),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableBankRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  EnableBankName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableValidatorList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           // �������� �� ���������
};

/*************************************
  ��������� ������
*************************************/
TRangeValueULONG const EnableModemRange = {0, 1};
CPU_INT08U const EnableModemName[] = "�����";
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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.EnableModem),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableModemRange,     // ��������� �� ������� ���������
  OnchangeEnableModem,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  EnableModemName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableModemList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  �������� ���������� �� ������� �� e-mail
*************************************/
TRangeValueULONG const EnableEmailErrorSendRange = {0, 1};
CPU_INT08U const EnableEmailErrorSendName[] = "����.�� ��.";
CPU_INT08U const *EnableEmailErrorSendList[] = {OnOffList_str0, OnOffList_str1};

TDataDescStruct const EnableEmailErrorSendDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.EnableEmailErrorSend),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableEmailErrorSendRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  EnableEmailErrorSendName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableEmailErrorSendList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ������ ����������� ������
*************************************/
CPU_INT08U const ModemStatusName[] = "������";
CPU_INT08U const *ModemStatusList[] = {"��", "��.����.", "��� �����"};

TDataDescStruct const ModemStatusDesc = {
  DATA_DESC_VIEW,          // ��� �����������
  DATA_TYPE_ULONG,         // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,           // ������� �������
  0,                       // ������ �������
  0,                       // ��������� �� ��������� ������� �������
  &modem_status,           // ��������� �� ���������� ��� ����� FRAM
  NULL,                    // ��������� �� ������� ���������
  NULL,                    // ������� �� ���������
  0,      // �������� ����� ���������� � �������
  ModemStatusName, // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,           // ������� ���������� ��������� (������ �����)
  ModemStatusList,// ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  �������� �������� �������� �� e-mail
*************************************/
CPU_INT08U const EnableEmailJournalSendName[] = "����.�������";

TDataDescStruct const EnableEmailJournalSendDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.EnableEmailJournalSend),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableEmailErrorSendRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  EnableEmailJournalSendName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableEmailErrorSendList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ������� ������ ����� �������� �� e-mail �� ������������
*************************************/
CPU_INT08U const ClearJournalAfterSendName[] = "��.�������";

TDataDescStruct const ClearJournalAfterSendDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.ClearJournalAfterSend),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableEmailErrorSendRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  ClearJournalAfterSendName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableEmailErrorSendList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
����� �������� ����������, ��� : ���
*************************************/
TRangeValueULONG const StatSendHourRange = {0, 60*24 - 1};
CPU_INT08U const StatSendHourName[] = "T����.";

TDataDescStruct const StatSendHourMinDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_HOUR_MIN,    // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.StatSendHourMin),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&StatSendHourRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  StatSendHourName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  9                           
};

/*************************************
  ������ ��� ��������� ��������
*************************************/
TRangeValueULONG const SignalIndexRange = {0, 5};
CPU_INT08U const SignalIndexName[] = "����.����.���.";
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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)&CoinIndex,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&SignalIndexRange,     // ��������� �� ������� ���������
  OnChangeSignalIndex,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  SignalIndexName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  SignalIndexList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ��������� ������� ������
*************************************/
TRangeValueULONG const EnableSignalRange = {0, 1};
CPU_INT08U const EnableSignalName[] = "������ ";
CPU_INT08U const *EnableSignalList[] = {OnOffList_str0, OnOffList_str1};

void OnchangeEnableSignal(void)
{
}

TDataDescStruct const EnableSignalDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,// ������ �������
  &SignalIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, enableSignalPrint),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableSignalRange,     // ��������� �� ������� ���������
  OnchangeEnableSignal,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  EnableSignalName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableSignalList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ������������ �������� ������� ������
*************************************/
TRangeValueULONG const SignalPulseRange = {1, 60};
CPU_INT08U const SignalPulseName[] = "�����.���.,���.";

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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,  // ������ �������
  &SignalIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, signal_pulse_len),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&SignalPulseRange,     // ��������� �� ������� ���������
  OnChangeSinalPulseLen,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  SignalPulseName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  1                         // �������� �� ���������
};

/*************************************
  ������� ������� ������ �����
*************************************/
TRangeValueULONG const SignalTimeOutRange = {0, 300};
CPU_INT08U const SignalTimeOutName[] = "�������,���.";

TDataDescStruct const SignalTimeOutDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  0,                        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, signal_print_timeout_len),        // ��������� �� ���������� ��� ����� FRAM
  (void*)&SignalTimeOutRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  SignalTimeOutName,        // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                         // �������� �� ���������
};

/*************************************
  ������ ��� ��������� ����������������
*************************************/
TRangeValueULONG const CoinIndexRange = {0, 7};
CPU_INT08U const CoinIndexName[] = "����.��������.";
CPU_INT08U const CoinIndex_str0[] = "1";
CPU_INT08U const CoinIndex_str1[] = "2";
CPU_INT08U const CoinIndex_str2[] = "3";
CPU_INT08U const CoinIndex_str3[] = "4";
CPU_INT08U const CoinIndex_str4[] = "5";
CPU_INT08U const CoinIndex_str5[] = "6";
CPU_INT08U const CoinIndex_str6[] = "�1";
CPU_INT08U const CoinIndex_str7[] = "�2";
CPU_INT08U const *CoinIndexList[] = {CoinIndex_str0, CoinIndex_str1,CoinIndex_str2,CoinIndex_str3,CoinIndex_str4,CoinIndex_str5,CoinIndex_str6,CoinIndex_str7};

void OnChangeCoinIndex(void)
{
}

TDataDescStruct const CoinIndexDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)&CoinIndex,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CoinIndexRange,     // ��������� �� ������� ���������
  OnChangeCoinIndex,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  CoinIndexName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  CoinIndexList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ��������� ���������������
*************************************/
TRangeValueULONG const EnableCoinRange = {0, 1};
CPU_INT08U const EnableCoinName[] = "��������-�";
CPU_INT08U const *EnableCoinList[] = {OnOffList_str0, OnOffList_str1};

void OnchangeEnableCoin(void)
{
}

TDataDescStruct const EnableCoinDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST+COUNT_VACUUM,// ������ �������
  &CoinIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.EnableCoinAcceptor),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableCoinRange,     // ��������� �� ������� ���������
  OnchangeEnableCoin,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  EnableCoinName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableCoinList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ���� �������� ���������������
*************************************/
TRangeValueULONG const CoinPerPulseRange = {1, 9999};
CPU_INT08U const CoinPerPulseName[] = "���./���.";

TDataDescStruct const CoinPerPulseDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST+COUNT_VACUUM,  // ������ �������
  &CoinIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.CoinPerPulse),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CoinPerPulseRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  CoinPerPulseName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  10                           // �������� �� ���������
};

/*************************************
  ������� ������ ���� ���������������
*************************************/
TRangeValueULONG const CoinTimeOutRange = {1, 999};
CPU_INT08U const CoinTimeOutName[] = "�������";

void OnchangeTimeOutCoin(void)
{
  if(CoinIndex < COUNT_POST)
  {
      
  }
}

TDataDescStruct const CoinTimeOutDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST+COUNT_VACUUM,  // ������ �������
  &CoinIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.CoinTimeOut),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CoinTimeOutRange,     // ��������� �� ������� ���������
  OnchangeTimeOutCoin,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  CoinTimeOutName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  30                        // �������� �� ���������
};

/*************************************
  ������ ��� ��������� ����������������
*************************************/
TRangeValueULONG const CashIndexRange = {0, 5};
CPU_INT08U const CashIndexName[] = "�����.��������.";

void OnChangeCashIndex(void)
{
}

TDataDescStruct const CashIndexDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)&CoinIndex,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CashIndexRange,     // ��������� �� ������� ���������
  OnChangeCashIndex,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  CashIndexName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  CoinIndexList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ������ ��� ��������� ����������� ���������
*************************************/
TRangeValueULONG const BankIndexRange = {0, 5};
CPU_INT08U const BankIndexName[] = "�����.����.����";

void OnChangeBankIndex(void)
{
}

TDataDescStruct const BankIndexDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)&CoinIndex,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&BankIndexRange,     // ��������� �� ������� ���������
  OnChangeBankIndex,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  BankIndexName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  CoinIndexList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ����� ���������
*************************************/
TRangeValueULONG const CashModeRange = {0, 1};
CPU_INT08U const CashModeName[] = "�����";
CPU_INT08U const CashMode_str0[] = "CCNet";
CPU_INT08U const CashMode_str1[] = "Pulse";
CPU_INT08U const *CashModeList[] = {CashMode_str0, CashMode_str1};

TDataDescStruct const CashModeDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &CashIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.CashMode),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CashModeRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  CashModeName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  CashModeList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  ����� ������ ����
*************************************/
TRangeValueULONG const PrintModeRange = {0, 1};
CPU_INT08U const PrintModeName[] = "���.������";
CPU_INT08U const PrintMode_str0[] = "�������";
CPU_INT08U const PrintMode_str1[] = "������";
CPU_INT08U const *PrintModeList[] = {PrintMode_str0, PrintMode_str1};

TDataDescStruct const PrintModeDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.PrintMode),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&PrintModeRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  PrintModeName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  PrintModeList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  ������� ������ ����
*************************************/
TRangeValueULONG const PrintTimeoutRange = {0, 3600};
CPU_INT08U const PrintTimeoutName[] = "������� ������";

TDataDescStruct const PrintTimeoutDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.PrintTimeout),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&PrintTimeoutRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  PrintTimeoutName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  5                         // �������� �� ���������
};

/*************************************
  ��������� ������ ����� ��� ���� ����
*************************************/
TRangeValueULONG const CashPerMinuteRange = {0, 600};
CPU_INT08U const CashPerMinuteName[] = "���./���.";

TDataDescStruct const CashPerMinuteDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.CashPerMinute),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CashPerMinuteRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  CashPerMinuteName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  10                          // �������� �� ���������
};

/*************************************
  ���� �������� ��������� � ���������� ������
*************************************/
TRangeValueULONG const CashPerPulseRange = {1, 9999};
CPU_INT08U const CashPerPulseName[] = "���./���.";

TDataDescStruct const CashPerPulseDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &CashIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.CashPerPulse),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CashPerPulseRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  CashPerPulseName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  10                           // �������� �� ���������
};

/*************************************
  ���� �������� ����������� ��������� � ���������� ������
*************************************/
TRangeValueULONG const BankPerPulseRange = {1, 9999};
CPU_INT08U const BankPerPulseName[] = "���./���.";

TDataDescStruct const BankPerPulseDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &BankIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.BankPerPulse),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&BankPerPulseRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  BankPerPulseName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  10                           // �������� �� ���������
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
  ������� ������� ���������
*************************************/
TRangeValueULONG const LevelRange = {0, 1};
CPU_INT08U const LevelName[] = "�������";
CPU_INT08U const Level_str0[] = "LOW";
CPU_INT08U const Level_str1[] = "HIGH";
CPU_INT08U const *LevelList[] = {Level_str0, Level_str1};

TDataDescStruct const CashLevelDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &CashIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.CashLevel),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&LevelRange,     // ��������� �� ������� ���������
  OnChangeLevel,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  LevelName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  LevelList,                  // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                          // �������� �� ���������
};

/*************************************
  ������� ������� ������������ ���������
*************************************/
TDataDescStruct const BankLevelDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &BankIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.BankLevel),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&LevelRange,     // ��������� �� ������� ���������
  OnChangeLevel,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  LevelName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  LevelList,                  // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                          // �������� �� ���������
};

/*************************************
  ������� ������� ���������
*************************************/
TDataDescStruct const CoinLevelDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST + COUNT_VACUUM,             // ������ �������
  &CoinIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.CoinLevel),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&LevelRange,     // ��������� �� ������� ���������
  OnChangeLevel,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  LevelName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  LevelList,                  // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                          // �������� �� ���������
};

/*************************************
  ������� ������� ��������� �������� �����
*************************************/
CPU_INT08U const SignalStopLevelName[] = "����� ����� ";

TDataDescStruct const SignalStopMoneyLevelDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &SignalIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.SignalLevel),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&LevelRange,     // ��������� �� ������� ���������
  OnChangeLevel,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  SignalStopLevelName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  LevelList,                  // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  1                          // �������� �� ���������
};

/*************************************
  ������� ������� ������ ����
*************************************/
CPU_INT08U const SignalPrintLevelName[] = "������ ����";
CPU_INT08U const *SignalLevelList[] = {Level_str1, Level_str0};

TDataDescStruct const SignalPrintLevelDesc = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,             // ������ �������
  &SignalIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.SignalLevel),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&LevelRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  SignalPrintLevelName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  SignalLevelList,                  // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  1                          // �������� �� ���������
};

/*************************************
  ��������� ��
*************************************/
TRangeValueULONG const EnableFiscalRange = {0, 1};
CPU_INT08U const EnableFiscalName[] = "��";
CPU_INT08U const *EnableFiscalList[] = {OnOffList_str0, OnOffList_str1};

TDataDescStruct const EnableFiscalDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.EnableFiscal),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableFiscalRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  EnableFiscalName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableFiscalList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ��������� ��
*************************************/
TRangeValueULONG const DisableFiscalErrorsRange = {0, 1};
CPU_INT08U const DisableFiscalErrorsName[] = "�������.��.��";
CPU_INT08U const DisableFiscalErrorsList_str0[] = "���";
CPU_INT08U const DisableFiscalErrorsList_str1[] = "��";
CPU_INT08U const *DisableFiscalErrorsList[] = {DisableFiscalErrorsList_str0, DisableFiscalErrorsList_str1};

TDataDescStruct const DisableFiscalErrorsDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.DisableFiscalErrors),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&DisableFiscalErrorsRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  DisableFiscalErrorsName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  DisableFiscalErrorsList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  ���� 
*************************************/
TRangeValueULONG const PriceRange = {1, 10000};
CPU_INT08U const PriceName[] = "����,���.";

TDataDescStruct const PriceDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,             // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  1,             // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, Price),                // ��������� �� ���������� ��� ����� FRAM
  (void*)&PriceRange,           // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  PriceName,             // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  100                           
};

/*************************************
  ������� ������������� �� ���������
*************************************/
CPU_INT32U InitByDefault = 0;

TRangeValueULONG const InitByDefaultRange = {0, 1};
CPU_INT08U const InitByDefaultName[] = "�������������";
CPU_INT08U const InitByDefaultList_str0[] = "���";
CPU_INT08U const InitByDefaultList_str1[] = "��";
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
  char name[32] = "������ ���������\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
  WriteArrayFram(offsetof(TFramMap, manual_service_flag), 4, (unsigned char*)&flag);
  WriteArrayFram(offsetof(TFramMap, manual_service_name), 32, (unsigned char*)&name);
  
  // ������� �������� ������
  for(int i = 0; i < COUNT_POST; i++)
  {
    ClearAcceptedBankMoney(i);
  }
  
  for(int i = 0; i < COUNT_POST + COUNT_VACUUM; i++)
  {
    ClearAcceptedMoney(i);
  }
}


TDataDescStruct const InitByDefaultDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,             // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)&InitByDefault,                // ��������� �� ���������� ��� ����� FRAM
  (void*)&InitByDefaultRange,           // ��������� �� ������� ���������
  OnChangeInitByDefault,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  InitByDefaultName,             // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  InitByDefaultList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};


/*************************************
  ������� �� ������ Z-������
*************************************/
CPU_INT32U PrintZReportCmd = 0;

CPU_INT08U const PrintZReportName[] = "���.� ����.��.";
CPU_INT08U const PrintZReportList_str0[] = "���";
CPU_INT08U const PrintZReportList_str1[] = "������";
CPU_INT08U const *PrintZReportList[] = {PrintZReportList_str0, PrintZReportList_str1};


void OnChangePrintZReportCmd(void)
{
}

TDataDescStruct const PrintZReportDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,             // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)&PrintZReportCmd,                // ��������� �� ���������� ��� ����� FRAM
  (void*)&InitByDefaultRange,           // ��������� �� ������� ���������
  OnChangePrintZReportCmd,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  PrintZReportName,             // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  PrintZReportList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};


/*************************************
  ������� �� ������ X-������
*************************************/
CPU_INT32U PrintXReportCmd = 0;

CPU_INT08U const PrintXReportName[] = "X-�����";

void OnChangePrintXReportCmd(void)
{
}

TDataDescStruct const PrintXReportDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,             // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)&PrintXReportCmd,                // ��������� �� ���������� ��� ����� FRAM
  (void*)&InitByDefaultRange,           // ��������� �� ������� ���������
  OnChangePrintXReportCmd,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  PrintXReportName,             // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  PrintZReportList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ������ � ������� ������
*************************************/
TRangeValueULONG const ErrorNumberRange = {0, JOURNAL_EVENTS_COUNT - 1};
CPU_INT08U const *ErrorNumberList0[JOURNAL_EVENTS_COUNT] = 
                                    {"���",
                                    
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
                                        "", "", "", "��:���������",

                                       "��:01h-�����������",
                                       "��:02h-��������",
                                       "��:03h-������ ��",
                                       "��:04h-������ ��",
                                       "��:05h-�������� ����",  
                                       "��:06h-����� ��", 
                                       "��:07h-�������� ����", 
                                       "��:08h-��� ", 
                                       "��:09h-������.", 
                                       "��:10h-����������", 
                                       "��:11h-���", 
                                       "��:12h-��������",
                                       "��:14h-��������", 
                                       "��:15h-��������", 
                                       "��:16h-����� �����",
                                       "��:17h-��������", 
                                      "��:20h-���������", 
                                      "��:2Fh-������� ",
                                      "��:30h-�� ", 
                                      "��:33h-������������", 
                                      "��:34h-���", 
                                      "��:35h-������������", 
                                      "��:36h-������������", 
                                      "��:37h-�������", 
                                      "��:38h-������", 
                                      "��:39h-����������", 
                                      "��:3Ah-������������", 
                                      "��:3Ch-�����", 
                                      "��:3Dh-�����", 
                                      "��:3Eh-������������", 
                                      "��:3Fh-������������", 
                                      "��:40h-������������", 
                                      "��:41h-������������", 
                                      "��:42h-������������", 
                                      "��:43h-������������", 
                                      "��:44h-������������", 
                                      "��:45h-C����", 
                                      "��:46h-�� �������", 
                                      "��:47h-������������", 
                                      "��:48h-������������", 
                                      "��:49h-����.������.", 
                                      "��:4Ah-������ ���", 
                                      "��:4Bh-����� ����", 
                                      "��:4Ch-������������", 
                                      "��:4Dh-��������", 
                                      "��:4Eh-�����", 
                                      "��:4Fh-�������� ������", 
                                      "��:50h-���� ������", 
                                      "��:51h-������������", 
                                      "��:52h-������������", 
                                      "��:53h-������������", 
                                      "��:54h-������������", 
                                      "��:55h-��� ������", 
                                      "��:56h-��� ���.", 
                                      "��:58h-��������", 
                                      "��:59h-��������", 
                                      "��:5Bh-������������", 
                                      "��:5Ch-��������", 
                                      "��:5Dh-�������", 
                                      "��:5Eh-������������",
                                      "��:5Fh-�����.", 
                                      "��:60h-������������",
                                      "��:61h-������������",
                                      "��:62h-������������",
                                      "��:63h-������������",
                                      "��:65h-�� �������",
                                      "��:66h-������������",
                                      "��:68h-�� �������",
                                      "��:69h-������������",
                                      "��:6Ah-������",
                                      "��:6Bh-��� �������",
                                      "��:6Ch-��� �����.",
                                      "��:6Dh-�� �������",
                                      "��:6Eh-������������",
                                      "��:6Fh-������������",
                                      "��:71h-������", 
                                      "��:72h-������� ��", 
                                      "��:73h-������� ��", 
                                      "��:74h-������ ���", 
                                      "��:75h-������", 
                                      "��:77h-�������:", 
                                      "��:78h-������ ��", 
                                      "��:7Ah-���� ��", 
                                      "��:7Bh-������", 
                                      "��:7Ch-�� ���������", 
                                      "��:7Dh-��������", 
                                      "��:7Eh-��������", 
                                      "��:7Fh-������������", 
                                      "��:84h-������������", 
                                      "��:85h-������������", 
                                      "��:86h-������������", 
                                      "��:87h-������������", 
                                      "��:88h-������������", 
                                      "��:89h-������������", 
                                      "��:8Ah-������������", 
                                      "��:8Bh-������������", 
                                      "��:8Ch-�����.", 
                                      "��:8Dh-�������������", 
                                      "��:8Eh-������� ����", 
                                      "��:90h-���� ����.", 
                                      "��:91h-����� ��", 
                                      "��:92h-���������", 
                                      "��:93h-��������������", 
                                      "��:94h-��������", 
                                      "��:C0h-��������", 
                                      "��:C2h-����������", 
                                      "��:C4h-������������", 
                                      "��:C7h-���� ��", 
                                      "��:�8h-�����������"

};

TDataDescStruct const JournalErrorNumberDesc0 = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,             // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  EVENT_RECORDS_COUNT,             // ������ �������
  &EventJournalIndexDesc,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, EventRecords[0].event),                // ��������� �� ���������� ��� ����� FRAM
  (void*)&ErrorNumberRange,           // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(TEventRecord),       // �������� ����� ���������� � �������
  NULL,             // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  (void*)ErrorNumberList0,                     // ��������� �� ������ ����� ��� ���������� ���������
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
                                        "", "", "", "�������",

                                       "�������",  // ��:01h
                                       "��������� ��",                  
                                       "",
                                       "",                                       
                                       "������������ ��",
                                       "����������",
                                       "�/��� �����",
                                       "����������� ������",
                                       "����.���.�-��",
                                       "����.TLV ������",
                                       "������.����.",
                                       "������ ��",
                                       "������ ��������",
                                       "������ ��������",
                                       "24 �����",
                                       "������� �� �������",
                                        "�� ��� �� �������",
                                        "������ � ��",
                                        "�� ��������",
                                        "��������� � �-��",
                                        "������",
                                        "��������",
                                        "���������",    // 36h
                                        "�� ��������������",
                                        "� ���",
                                        "������ �� ���",
                                        "���.�� ����.",
                                        "����.-�������� ������.",
                                        "����.-�������� ������.",       //3Dh
                                        "�����.�� �������",
                                        "�����.�� �������",
                                        "��������� ������",
                                        "��������� ������",
                                        "��������� ������ 2",
                                        "��������� ������ 3",
                                        "��������� ������ 4",
                                        "������ ����� ����",
                                        "���������� � �����",
                                        "���������� �� �������",
                                        "����� ����",
                                        "� ����.����",
                                        "�������� ����������",
                                        "����������",
                                        "�����.�� �������",
                                        "������.����� ������",
                                        "��������� 24 ����",
                                        "",
                                        "����.�������",
                                        "���������� � �����",
                                        "���������� 2 � �����",
                                        "���������� 3 � �����",
                                        "���������� 4 � �����",
                                        "�������� ������.",
                                        "��� �������",
                                        "������� �����������",
                                        "������ ������ ��.",
                                        "��������� ��������",
                                        "���������� 24�",
                                        "�� ����������",
                                        "��������",
                                        "���� ����",
                                        "��� ���������",
                                        "��������� ����",
                                        "��������� ���-��",
                                        "��������� ������",
                                        "����� � ������",
                                        "����� � ������",
                                        "����� �� �������",
                                        "����� �� �������",
                                        "�������",
                                        "�����",
                                        "�����",
                                        "����� �� ������",
                                        "����� �� ������",
                                        "�� ������� � �����",
                                        "���������",
                                        "�������.",
                                        "�������.",
                                        "",
                                        "�������",
                                        "��� �������",
                                        "",
                                        "�������������",
                                        "������������",
                                        "����",
                                        "������ ����",
                                        "�������� � ���� �����",
                                        "��������� ����� ����",
                                        "����������",
                                        "�� ��������",
                                        "�� ��������",
                                        "�� ��������� ������",
                                        "�� ��������� �������",
                                        "�� �������� � �����",
                                        "�� ��������� � ����",
                                        "�� ������� � ����",
                                        "���� ��������",
                                        "���� ������ � ����",
                                        "����",
                                        "������ � ����������",
                                        "������� ���� ������",
                                        "�����",
                                        "��� ������ �������",
                                        "����� �������� � ����",
                                        "���� � �������",
                                        "����������",
                                        "������� ����",
                                        "�������������",
                                        "�������� ����."
};

TDataDescStruct const JournalErrorNumberDesc1 = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,             // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  EVENT_RECORDS_COUNT,             // ������ �������
  &EventJournalIndexDesc,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, EventRecords[0].event),                // ��������� �� ���������� ��� ����� FRAM
  (void*)&ErrorNumberRange,           // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(TEventRecord),       // �������� ����� ���������� � �������
  NULL,             // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  (void*)ErrorNumberList1,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  ������ � ������� ��-���������
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
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,             // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  EVENT_RECORDS_COUNT,             // ������ �������
  &EventJournalIndexDesc,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, EventRecords[0].event),                // ��������� �� ���������� ��� ����� FRAM
  (void*)&ErrorNumberRange,           // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(TEventRecord),       // �������� ����� ���������� � �������
  NULL,             // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  (void*)ErrorNumberList1,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  ������ ��� ��������� ������� �������
*************************************/
TRangeValueULONG const EventJournalIndexRange = {0, 0xffffffff};
CPU_INT08U const EventJournalIndexName[] = "������ #";
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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)&EventJournalIndex,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EventJournalIndexRange,     // ��������� �� ������� ���������
  OnChangeEventJournalIndex,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  EventJournalIndexName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ����� ������� � ������� �������
*************************************/
TDataDescStruct const JournalEventTimeDesc = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_TIME,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  EVENT_RECORDS_COUNT,             // ������ �������
  &EventJournalIndexDesc,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, EventRecords[0].time),            // ��������� �� ���������� ��� ����� FRAM
  NULL,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(TEventRecord),       // �������� ����� ���������� � �������
  NULL,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ��������� �����
*************************************/
extern CPU_INT32U SystemTime;

TDataDescStruct const SystemTimeDesc = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_TIME,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)&SystemTime,            // ��������� �� ���������� ��� ����� FRAM
  NULL,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  NULL,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_TIME,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)&SystemTime,            // ��������� �� ���������� ��� ����� FRAM
  NULL,     // ��������� �� ������� ���������
  OnSetTime,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  NULL,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  ������� �� ������� ��������
*************************************/
CPU_INT32U ClearJournalCmd = 0;

CPU_INT08U const ClearJournalCmdName[] = "�������";
CPU_INT08U const ClearJournalCmdList_str0[] = "���";
CPU_INT08U const ClearJournalCmdList_str1[] = "��";
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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,             // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)&ClearJournalCmd,                // ��������� �� ���������� ��� ����� FRAM
  (void*)&InitByDefaultRange,           // ��������� �� ������� ���������
  OnChangeClearJournalCmd,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  ClearJournalCmdName,             // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  ClearJournalCmdList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};


/*************************************
  ����� ������� ����� ��������
*************************************/
CPU_INT08U const CounterRunName[] = "�������";

TDataDescStruct const CounterRunDesc = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, Counters.CounterRun),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  CounterRunName,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ����� ������� �����
*************************************/
CPU_INT08U const CounterMoneyName[] = "������,���.";

TDataDescStruct const CounterMoneyDesc = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, Counters.CounterMoney),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  CounterMoneyName,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ����� ������� ������� ������
*************************************/
CPU_INT08U const CounterTimeName[] = "��.���.";

TDataDescStruct const CounterTimeDesc = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_TIME_COUNT,     // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, Counters.CounterTime),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  CounterTimeName,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};


/*************************************
  ����� ������� ����� �������� �������
*************************************/
TDataDescStruct const CounterLongRunDesc = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, CountersLong.CounterRunLong),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  CounterRunName,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ����� ������� ����� ������� 
*************************************/
TDataDescStruct const CounterLongMoneyDesc = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, CountersLong.CounterMoneyLong),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  CounterMoneyName,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ����� ������� ������� ������
*************************************/
TDataDescStruct const CounterLongTimeDesc = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_TIME_COUNT,     // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, CountersLong.CounterTimeLong),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  CounterTimeName,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ������� �� ������� ���������� 
*************************************/
CPU_INT32U ClearStatCmd = 0;

CPU_INT08U const ClearStatCmdName[] = "�������";

void OnChangeClearStatCmd(void)
{
  if (ClearStatCmd)
    {
      ClearStatCmd = 0;
    }
}

TDataDescStruct const ClearStatCmdDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,             // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)&ClearStatCmd,                // ��������� �� ���������� ��� ����� FRAM
  (void*)&InitByDefaultRange,           // ��������� �� ������� ���������
  OnChangeClearStatCmd,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  ClearJournalCmdName,             // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  ClearJournalCmdList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};


/*************************************
  ���������� ��������������� �������� ����
*************************************/
TRangeValueULONG const EnableFiscalDayClearRange = {0, 1};
CPU_INT08U const EnableFiscalDayClearName[] = "����.�����";
CPU_INT08U const *EnableFiscalDayClearList[] = {"���.", "����", "�����"};

TDataDescStruct const EnableFiscalDayClearDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.EnableFiscalDayClear),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableFiscalDayClearRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  EnableFiscalDayClearName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  EnableFiscalDayClearList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           // �������� �� ���������
};


/*************************************
  ��������� ����
*************************************/
TRangeValueULONG const BillFormatRange = {0, 1};
CPU_INT08U const BillFormatName[] = "���:";
CPU_INT08U const BillFormatList_str0[] = "���-��*����";
CPU_INT08U const BillFormatList_str1[] = "�����";
CPU_INT08U const *BillFormatList[] = {BillFormatList_str0, BillFormatList_str1};

TDataDescStruct const BillFormatDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.BillFormat),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&BillFormatRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  BillFormatName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  BillFormatList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  ������� ���������������
*************************************/
TRangeValueULONG const TaxSystemRange = {0, 5};
CPU_INT08U const TaxSystemName[] = "������� ���.";
CPU_INT08U const TaxSystemList_str0[] = "���";
CPU_INT08U const TaxSystemList_str1[] = "��� �";
CPU_INT08U const TaxSystemList_str2[] = "��� �-�";
CPU_INT08U const TaxSystemList_str3[] = "����";
CPU_INT08U const TaxSystemList_str4[] = "���";
CPU_INT08U const TaxSystemList_str5[] = "���";

CPU_INT08U const *TaxSystemList[] = {TaxSystemList_str0, TaxSystemList_str1, TaxSystemList_str2, TaxSystemList_str3, TaxSystemList_str4, TaxSystemList_str5};

TDataDescStruct const TaxSystemDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, TaxSystem),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&TaxSystemRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  TaxSystemName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  TaxSystemList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ������ ������
*************************************/
TRangeValueULONG const TaxFormatRange = {1, 6};
CPU_INT08U const TaxFormatName[] = "�����:";
CPU_INT08U const TaxFormatList_str0[] = "����.";
CPU_INT08U const TaxFormatList_str1[] = "  1  ";
CPU_INT08U const TaxFormatList_str2[] = "  2  ";
CPU_INT08U const TaxFormatList_str3[] = "  3  ";
CPU_INT08U const TaxFormatList_str4[] = "  4  ";
CPU_INT08U const TaxFormatList_str5[] = "  5  ";
CPU_INT08U const TaxFormatList_str6[] = "  6  ";
CPU_INT08U const *TaxFormatList[] = {TaxFormatList_str0, TaxFormatList_str1, TaxFormatList_str2, TaxFormatList_str3, TaxFormatList_str4, TaxFormatList_str5, TaxFormatList_str6};

TDataDescStruct const TaxFormatDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, TaxFormat),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&TaxFormatRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  TaxFormatName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  TaxFormatList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  6                           
};

/*************************************
  ������� �������
*************************************/
TRangeValueULONG const SubjSellRange = {0, 2};
CPU_INT08U const SubjSellName[] = "�����.�������";
CPU_INT08U const SubjSellList_str0[] = "�����";
CPU_INT08U const SubjSellList_str1[] = "������";
CPU_INT08U const SubjSellList_str2[] = "������";
CPU_INT08U const *SubjSellList[] = {SubjSellList_str0, SubjSellList_str1, SubjSellList_str2};

TDataDescStruct const SubjSellDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, SubjSell),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&SubjSellRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  SubjSellName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  SubjSellList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ��� ������ ������ � �����
*************************************/
TRangeValueULONG const CommandV2Range = {0, 1};
CPU_INT08U const CommandV2Name[] = "��� ������";
CPU_INT08U const CommandV2List_str0[] = "������";
CPU_INT08U const CommandV2List_str1[] = "V2";
CPU_INT08U const *CommandV2List[] = {CommandV2List_str0, CommandV2List_str1};

TDataDescStruct const CommandV2Desc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, CommandV2),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&CommandV2Range,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  CommandV2Name,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  CommandV2List,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  1                           
};

/*************************************
  �������� ������
*************************************/
TRangeValueULONG const ServiceNameRange = {0, 1};
CPU_INT08U const ServiceNameName[] = "�����";
CPU_INT08U const ServiceNameList_str0[] = "�����";
CPU_INT08U const ServiceNameList_str1[] = "������ �����";
CPU_INT08U const *ServiceNameList[] = {ServiceNameList_str0, ServiceNameList_str1};

void OnChangeServiceName(void)
{
    CPU_INT08U flag[4] = {0, 0, 0, 0};

    WriteArrayFram(offsetof(TFramMap, manual_service_flag), 4, (unsigned char*)&flag);
}

TDataDescStruct const ServiceNameDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.ServiceName),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&ServiceNameRange,     // ��������� �� ������� ���������
  OnChangeServiceName,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  ServiceNameName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  ServiceNameList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  1                           // �������� �� ���������
};

/*************************************
  ���������� ��� ������������������ ���������� ������� �����
*************************************/

TDataDescStruct const AcceptedMoneyDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST + COUNT_VACUUM,// ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, FRAM_AcceptedMoney),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  NULL,                     // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

CPU_INT08U const AcceptedMoneyName[] = "���/���.���.";

TDataDescStruct const AcceptedMoneyDesc1 = {
  DATA_DESC_VIEW,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST + COUNT_VACUUM,// ������ �������
  &StatMoneyIndexDesc,      // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, FRAM_AcceptedMoney),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  (void*)&AcceptedMoneyName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ���������� Crc16 ������������������ ���������� ������� �����
*************************************/
TDataDescStruct const AcceptedMoneyCRC16Desc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST+COUNT_VACUUM,  // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, crc_AcceptedMoney),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  NULL,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ���������� ��� ������������������ ���������� ������� �����
*************************************/

TDataDescStruct const AcceptedBankMoneyDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,               // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, FRAM_AcceptedBankMoney),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  NULL,                     // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

CPU_INT08U const AcceptedBankMoneyName[] = "���� ���.";

TDataDescStruct const AcceptedBankMoneyDesc1 = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,               // ������ �������
  &StatMoneyIndexDesc,      // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, FRAM_AcceptedBankMoney),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  (void*)&AcceptedBankMoneyName,   // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ���������� Crc16 ������������������ ���������� ������� �����
*************************************/
TDataDescStruct const AcceptedBankMoneyCRC16Desc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_IS_ARRAY,            // ������� �������
  COUNT_POST,  // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, crc_AcceptedBankMoney),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  NULL,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ���������� ������
*************************************/
TRangeValueULONG const PassRange = {0, 9999};
CPU_INT08U const PassName[] = "  �����b";

void OnChangePass(void)
{
  // ������� CRC ������  
  CPU_INT32U pass,crc;
  GetData(&PassDesc, &pass, 0, DATA_FLAG_SYSTEM_INDEX);      
  crc = crc16((unsigned char*)&pass, sizeof(CPU_INT32U));
  SetData(&PassCRCDesc, &crc, 0, DATA_FLAG_SYSTEM_INDEX);
}

TDataDescStruct const PassDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, Pass),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&PassRange,                     // ��������� �� ������� ���������
  OnChangePass,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  (void*)&PassName,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  DEFAULT_PASSWORD                           
};

/*************************************
  ���������� CRC ������
*************************************/
TDataDescStruct const PassCRCDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, crc_Pass),            // ��������� �� ���������� ��� ����� FRAM
  NULL,                     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  NULL,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ���������� ���������� ������
*************************************/
CPU_INT32U TempPass = 0;

void OnChangeTempPass(void)
{
  CPU_INT32U pass;
  GetData(&PassDesc, &pass, 0, DATA_FLAG_SYSTEM_INDEX);
    
  if (GetCurrentMenu() == SetPassMenuPanel)
    { // �������� ������ ������
      if (pass != TempPass)
      {
        SaveEventRecord(0, JOURNAL_EVENT_PASS_FAIL, TempPass);
        GoToMenu(ErrorPassPanel);
      }
      else {GoToPreviousMenu(); GoToMenu(SetNewPassMenuPanel);}
    }
  else if (GetCurrentMenu() == ResetSettingsMenuPanel)
    { // ����� ��������
      if (pass != TempPass) 
      {
        SaveEventRecord(0, JOURNAL_EVENT_PASS_FAIL, TempPass);
        GoToMenu(ErrorPassPanel);
      }
      else {InitByDefault = 1; OnChangeInitByDefault(); GoToPreviousMenu(); GoToMenu(SettingsIsReset);}
    }
  else if (GetCurrentMenu() == ClearStatMenu)
    { // ������� ����������
      if (pass != TempPass)
      {
        SaveEventRecord(0, JOURNAL_EVENT_PASS_FAIL, TempPass);
        GoToMenu(ErrorPassPanel);
      }
      else {ClearStatCmd = 1; OnChangeClearStatCmd(); GoToPreviousMenu(); GoToMenu(StatIsReset);}
    }
  else if (GetCurrentMenu() == ClearJournalMenuPanel)
    { // ������� ����������
      if (pass != TempPass) 
      {
        SaveEventRecord(0, JOURNAL_EVENT_PASS_FAIL, TempPass);
        GoToMenu(ErrorPassPanel);
      }
      else {ClearJournalCmd = 1; OnChangeClearJournalCmd(); GoToPreviousMenu(); GoToMenu(JournalIsReset);}
    }
}

TDataDescStruct const PassTempDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)&TempPass,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&PassRange,                     // ��������� �� ������� ���������
  OnChangeTempPass,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  (void*)&PassName,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};


CPU_INT08U const PassTempName1[] = "    �����b";

TDataDescStruct const PassTempDesc1 = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)&TempPass,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&PassRange,                     // ��������� �� ������� ���������
  OnChangeTempPass,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  (void*)&PassTempName1,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

TRangeValueULONG const MasterPassRange = {0, 99999999};
CPU_INT08U const MasterPassTempName[] = "�����b";

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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)&TempPass,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&MasterPassRange,                     // ��������� �� ������� ���������
  OnChangeMasterPass,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  (void*)&MasterPassTempName,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

CPU_INT08U const PassTempName2[] = "   �����b";

TDataDescStruct const PassTempDesc2 = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,                        // ������ �������
  NULL,                     // ��������� �� ��������� ������� �������
  (void*)&TempPass,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&PassRange,                     // ��������� �� ������� ���������
  OnChangeTempPass,                     // ������� �� ���������
  0,                        // �������� ����� ���������� � �������
  (void*)&PassTempName2,           // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  ������� ������� �������� ���������
*************************************/
CPU_INT08U const SendTestEmailName[] = "����.����";
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
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)&send_test,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&EnableEmailErrorSendRange,     // ��������� �� ������� ���������
  OnChangeSendTestEmail,                     // ������� �� ���������
  sizeof(CPU_INT32U),       // �������� ����� ���������� � �������
  SendTestEmailName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  DisableFiscalErrorsList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
};

/*************************************
  ID ���������� 
*************************************/
CPU_INT08U const DeviceIDName[] = "ID ���-��";
TRangeValueULONG const DeviceIDRange = {0, 9999};

TDataDescStruct const DeviceIDDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  0,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, DeviceConfig.DeviceId),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&DeviceIDRange,     // ��������� �� ������� ���������
  NULL,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  DeviceIDName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  NULL,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0                           
};

/*************************************
  IP-����� ����
*************************************/
void OnChangeIpMask()
{
    CPU_INT32U ip, mask;
    GetData(&IpAddrDesc, &ip, 0, DATA_FLAG_SYSTEM_INDEX);  
    GetData(&NetMaskDesc, &mask, 0, DATA_FLAG_SYSTEM_INDEX);  
    NetIP_CfgAddrThisHost(ip, mask);
}

TRangeValueULONG const DeferredStartRange = {0, 1};
CPU_INT08U const DeferredStart_str0[] = "���";
CPU_INT08U const DeferredStart_str1[] = "��";
CPU_INT08U const *DeferredStartList[] = {DeferredStart_str0, DeferredStart_str1, NULL};

CPU_INT08U const IpAddrName[] = "IP";

TDataDescStruct const IpAddrDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_IP_ADDR,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  1,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, ip),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&DeferredStartRange,     // ��������� �� ������� ���������
  OnChangeIpMask,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  IpAddrName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  DeferredStartList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0xC0A80064
};

/*************************************
  ����� ����
*************************************/
CPU_INT08U const NetMaskName[] = "MSK";

TDataDescStruct const NetMaskDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_IP_ADDR,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  1,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, netmask),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&DeferredStartRange,     // ��������� �� ������� ���������
  OnChangeIpMask,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  NetMaskName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  DeferredStartList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0xFFFFFF00
};

/*************************************
  ����
*************************************/

void OnChangeGateway()
{
    CPU_INT32U gw;
    GetData(&GatewayDesc, &gw, 0, DATA_FLAG_SYSTEM_INDEX);  
    NetIP_CfgAddrDfltGateway(gw);
}
    
CPU_INT08U const GatewayName[] = "GW";

TDataDescStruct const GatewayDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_IP_ADDR,          // ��� ���������
  DATA_LOC_FRAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  1,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)offsetof(TFramMap, gateway),            // ��������� �� ���������� ��� ����� FRAM
  (void*)&DeferredStartRange,     // ��������� �� ������� ���������
  OnChangeGateway,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  GatewayName,       // ��������� �� ������ �������� ���������
  DATA_NO_INDEX,            // ������� ���������� ��������� (������ �����)
  DeferredStartList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_DISABLE,
  0xC0A80001
};

/*************************************
  ������ ��� ��������� �������� ���������� �����
*************************************/
CPU_INT08U const StatIndexName[] = "����� �� �����";

TDataDescStruct const StatMoneyIndexDesc = {
  DATA_DESC_EDIT,           // ��� �����������
  DATA_TYPE_ULONG,          // ��� ���������
  DATA_LOC_RAM,            // ������������ ���������
  DATA_NO_ARRAY,            // ������� �������
  0,             // ������ �������
  NULL,        // ��������� �� ��������� ������� �������
  (void*)&CoinIndex,            // ��������� �� ���������� ��� ����� FRAM
  (void*)&BankIndexRange,     // ��������� �� ������� ���������
  OnChangeBankIndex,                     // ������� �� ���������
  0,       // �������� ����� ���������� � �������
  StatIndexName,       // ��������� �� ������ �������� ���������
  DATA_IS_INDEX,            // ������� ���������� ��������� (������ �����)
  CoinIndexList,                     // ��������� �� ������ ����� ��� ���������� ���������
  DATA_INIT_ENABLE,
  0                           
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
    {&DisableFiscalErrorsDesc, "DisableFiscalErrorsDesc"},
    {&EnableSignalDesc, "EnableSignalDesc"},
    {&SignalPulseDesc, "SignalPulseDesc"},
    {&SignalTimeOutDesc, "SignalTimeOutDesc"},
    {&CashPerMinuteDesc, "CashPerMinuteDesc"},
    
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


