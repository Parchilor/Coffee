#include "main.h"

static TIM_TypeDef *timer_list[] = {
	TIM1, TIM3, TIM14, TIM16, TIM17
};
static uint32_t timer_rcc_list[] = {
	RCC_APB2Periph_TIM1, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM14, RCC_APB2Periph_TIM16, RCC_APB2Periph_TIM17
};
static uint8_t timer_irqn_list[] = {
	TIM1_BRK_UP_TRG_COM_IRQn, TIM3_IRQn, TIM14_IRQn, TIM16_IRQn, TIM17_IRQn
};
static void (*TIMER_RCC_Init[])(uint32_t, FunctionalState) = {
	RCC_APB2PeriphClockCmd, RCC_APB1PeriphClockCmd, RCC_APB1PeriphClockCmd, RCC_APB2PeriphClockCmd, RCC_APB2PeriphClockCmd
};

static TIM_TypeDef *uplink_list[3][4] = {
	{TIM15, 0, TIM3, TIM17},
	{TIM1, 0, TIM15, TIM14},
	{0, TIM3, TIM16, TIM17}
};

static uint16_t TIM_TS_ITRn[] = {
	TIM_TS_ITR0,
	TIM_TS_ITR1,
	TIM_TS_ITR2,
	TIM_TS_ITR3,
};
// Private Functions
static int8_t TIM_RCC_Init(TIM_TypeDef* TIMx)
{
	int8_t idx;
	for(idx = 0; idx < 5; idx++)
	{
		if(TIMx == timer_list[idx])
		{
			break;
		}
	}
	TIMER_RCC_Init[idx](timer_rcc_list[idx], ENABLE);
	return idx;
}

// Public Functions
void TIM_Nornal_Init(TIM_TypeDef *TIMx, uint16_t TIMpre, uint32_t TIMper)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint8_t idx = TIM_RCC_Init(TIMx);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	TIM_TimeBaseInitStructure.TIM_Period = TIMper;
	TIM_TimeBaseInitStructure.TIM_Prescaler= TIMpre;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(timer_list[idx], &TIM_TimeBaseInitStructure);
	TIM_ClearFlag(timer_list[idx], TIM_IT_Update);
	TIM_ITConfig(timer_list[idx], TIM_IT_Update, ENABLE);
	
	// TIM_Cmd(timer_list[idx], ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=timer_irqn_list[idx];
	NVIC_InitStructure.NVIC_IRQChannelPriority=0x03;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}


static void TIM_Uplink_BaseTIM_Init(TIM_TypeDef* TIMx, uint16_t TIMpre, uint32_t TIMper)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	// NVIC_InitTypeDef NVIC_InitStructure;
	int8_t idx = TIM_RCC_Init(TIMx);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = (TIMpre) % 0xFFFF;
	TIM_TimeBaseInitStruct.TIM_Period = (TIMper) % 0xFFFF;
	TIM_TimeBaseInit(timer_list[idx], &TIM_TimeBaseInitStruct);
	
	TIM_SelectMasterSlaveMode(timer_list[idx], TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger(timer_list[idx], TIM_TRGOSource_Update);
}

void TIM_Uplink_CounterTIM_Init(TIM_TypeDef *cTIMx, uint16_t cTIMpre, uint32_t cTIMper, TIM_TypeDef *bTIMx, uint16_t bTIMpre, uint32_t bTIMper)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	uint8_t pos;

	if(cTIMx == TIM1) for(pos = 0; pos < 4; pos++) if(bTIMx == uplink_list[0][pos]) break;
	else if(cTIMx == TIM3)  for(pos = 0; pos < 4; pos++) if(bTIMx == uplink_list[1][pos]) break;
	else if(cTIMx == TIM15) for(pos = 0; pos < 4; pos++) if(bTIMx == uplink_list[2][pos]) break;

	int8_t idx = TIM_RCC_Init(cTIMx);

	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Prescaler = (cTIMpre) % 0xFFFF;
	TIM_TimeBaseInitStruct.TIM_Period = (cTIMper) % 0xFFFF;
	TIM_TimeBaseInit(timer_list[idx], &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(timer_list[idx], TIM_FLAG_Update);
	TIM_ITConfig(timer_list[idx], TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = timer_irqn_list[idx];  
	NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
	
	TIM_ITRxExternalClockConfig(timer_list[idx],  TIM_TS_ITRn[pos]);
	TIM_Uplink_BaseTIM_Init(bTIMx, bTIMpre, bTIMper);

	TIM_Cmd(cTIMx, ENABLE);
	TIM_Cmd(bTIMx, ENABLE);
}


uint32_t jiffies = 0;
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update)!=RESET)
	{
		jiffies++;
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update)!=RESET)
	{
		jiffies++;
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14, TIM_IT_Update)!=RESET)
	{
		jiffies++;
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
	}
}

void TIM16_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM16, TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM16, TIM_IT_Update);
	}
}

void TIM17_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM17, TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
	}
}