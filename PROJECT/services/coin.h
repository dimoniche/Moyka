#ifndef _COIN_H_
#define _COIN_H_

#define COIN_IMP_MIN_LEN 2200 // ��/100
#define COIN_IMP_MAX_LEN 9000 // ��/100

// ����� ��������� �������, ����-�����
#define COIN_IMP_SPAN 1000 // ��/100

extern void InitCoin(void);
extern CPU_INT32U GetCoinCount(int index);
extern CPU_INT32U GetResetCoinCount(int index);
extern void CoinDisable(void);
extern void CoinEnable(void);
extern CPU_INT32U GetCashCount(int index);
extern CPU_INT32U GetResetCashCount(int index);
extern void SetCashPulseParam(CPU_INT32U pulse, CPU_INT32U pause, CPU_INT32U post);
extern void SetSignalPulseParam(CPU_INT32U pulse, CPU_INT32U post);

#endif //#ifndef _COIN_H_


