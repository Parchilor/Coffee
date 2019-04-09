#include "main.h"

uint8_t fac_us = 0;
uint16_t fac_ms = 0;

void delay_init(uint8_t SYSCLK)
{
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;// 设置SysTick的分频为8分频
	fac_us = SYSCLK / 8;
	fac_ms = (uint16_t)fac_us * 1000;
}

void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD = nus * fac_us;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (0x01 << 16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0X00;
}
/*
 * nms <= 0xffffff*8*1000/SYSCLK
 * 48M 时为 2796
 */
void delay_xms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD = (uint32_t)nms * fac_ms;
	SysTick->VAL = 0x00;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	do{
		temp = SysTick->CTRL;
	}while((temp & 0x01) && !(temp & (0x01 << 16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL = 0X00;
}

void delay_ms(uint16_t nms)
{	 	 
	uint8_t repeat = nms / 540;
	
	uint16_t remain = nms % 540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 