#ifndef __ADC_H
#define __ADC_H
#include "sys.h"

//ADC使用PA1 (ADC通道1)
void My_ADC_Init(void);
u16 ADC_Read(void);
float ADC_GetVoltage(void);

#endif
