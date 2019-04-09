#ifndef __STM32_BOARD_H_
#define __STM32_BOARD_H_

#include "stm32f0xx_conf.h"
#include "stm32f0xx.h"

#include "usart/usart.h"

#include "gpio/gpio.h"

#include "adc/adc.h"

#include "delay/delay.h"

#include "timer/timer.h"

#include "spi/spi.h"



void System_Init(RCC_ClocksTypeDef *clock);
void PWR_EnterSTDBYMode(void);

#endif