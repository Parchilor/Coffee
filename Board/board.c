#include "main.h"

void System_ClockInit(uint8_t idx)
{
	if(idx == 0)
	{
		__IO uint32_t StartUpCounter = 0, HSEStatus = 0;

		RCC_HSICmd(ENABLE);

		/* Wait till HSE is ready and if Time out is reached exit */
		HSEStatus = RCC_WaitForHSEStartUp();

		/* Enable Prefetch Buffer */
		FLASH_SetLatency(FLASH_Latency_1);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1); 

		/* PCLK = HCLK */
		RCC_PCLKConfig(RCC_HCLK_Div1);

		/*  PLL configuration:  = HSE *  6 = 48 MHz */
		RCC_PREDIV1Config(RCC_PREDIV1_Div1);
		RCC_PLLConfig(RCC_PLLSource_HSE, RCC_PLLMul_6);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* PLL as system clock source */
		//RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div2);
	}
	else if(idx == 1)
	{
		RCC_HSICmd(ENABLE);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
		// RCC_PREDIV1Config(RCC_PREDIV1_Div10);
		RCC_PLLConfig(RCC_PLLSource_HSI, RCC_PLLMul_10);
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//		RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div2);
	}
}

void System_Init(RCC_ClocksTypeDef *clock)
{
#ifdef USE_SRAM_DEBUG
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x00);
#else
	// NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
#endif
    
	System_ClockInit(1);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOF, ENABLE);
	RCC_GetClocksFreq(clock);
	delay_init(clock->SYSCLK_Frequency / 1000 / 1000);
	System_GPIO_Init();
	System_SPIx_Init();
	System_ADCInit();
	// System_COM_Init(9600, USARTn_Mode_IRQ);
}