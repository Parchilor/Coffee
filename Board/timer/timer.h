#ifndef __TIMER_H_
#define __TIMER_H_
#include "stm32f0xx_conf.h"

#define TIM_MS_PRE2(clk) (clk.PCLK_Frequency / 1000)

void TIM_Nornal_Init(TIM_TypeDef *TIMx, uint16_t TIMpre, uint32_t TIMper);
void TIM_Uplink_CounterTIM_Init(TIM_TypeDef *cTIMx, uint16_t cTIMpre, uint32_t cTIMper, TIM_TypeDef *bTIMx, uint16_t bTIMpre, uint32_t bTIMper);
#endif