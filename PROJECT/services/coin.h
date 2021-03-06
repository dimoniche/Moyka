#ifndef _COIN_H_
#define _COIN_H_

#define COIN_IMP_MIN_LEN 22 // ��/1
#define COIN_IMP_MAX_LEN 90 // ��/1

// ����� ��������� �������, ����-�����
#define COIN_IMP_SPAN 10 // ��/1

extern void InitCoin(void);
extern CPU_INT32U GetCoinCount(int index);
extern CPU_INT32U GetResetCoinCount(int index);
extern void CoinDisable(void);
extern void CoinEnable(void);
extern void InitInputPorts();

extern CPU_INT32U GetCashCount(int index);
extern CPU_INT32U GetResetCashCount(int index);

extern CPU_INT32U GetbankCount(int index);
extern CPU_INT32U GetResetbankCount(int index);

extern void SetCoinPulseParam(CPU_INT32U pulse, CPU_INT32U pause, CPU_INT32U post);
extern void SetCashPulseParam(CPU_INT32U pulse, CPU_INT32U pause, CPU_INT32U post);
extern void SetBankPulseParam(CPU_INT32U pulse, CPU_INT32U pause, CPU_INT32U post);
extern void SetSignalPulseParam(CPU_INT32U pulse, CPU_INT32U post);
extern void SetLevelParam(CPU_INT32U level1, CPU_INT32U level2, CPU_INT32U level3, CPU_INT32U level4, CPU_INT32U post);

#define BIT(bit)          (1UL << (bit))

#define SETBIT(Val,bit)   ((Val) |= BIT(bit))
#define CLRBIT(Val,bit)   ((Val) &= ~BIT(bit))
#define XORBIT(Val,bit)   ((Val) ^= BIT(bit))
#define TSTBIT(Val,bit)   ((Val) & BIT(bit))

#endif //#ifndef _COIN_H_


