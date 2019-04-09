/**
  ******************************************************************************
  * @file    TIM/TIM_6Steps/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_6Steps
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t step = 1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void __hal_fail(const signed char* file, int line);
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */

	__hal_fail(__FILE__, __LINE__);
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//unsigned long jiffies = 0;

void SysTick_Handler(void)
{
//	jiffies += 10;
}


void call_irq(int irq)
{
	
}

#define DEF_VECTOR(name, id)	\
	void name##_IRQHandler(void)	\
	{	call_irq(id);		}		\

void __hal_fail(const signed char* file, int line)
{
}
DEF_VECTOR(WWDG, 0)
DEF_VECTOR(RTC, 2)
DEF_VECTOR(FLASH, 3)
DEF_VECTOR(RCC, 4)
DEF_VECTOR(EXTI0_1, 5)
DEF_VECTOR(EXTI2_3, 6)
DEF_VECTOR(EXTI4_15, 7)
DEF_VECTOR(DMA1_Channel1, 9)
DEF_VECTOR(DMA1_Channel2_3, 10)
DEF_VECTOR(DMA1_Channel4_5, 11)
DEF_VECTOR(ADC1, 12)
//DEF_VECTOR(TIM1_BRK_UP_TRG_COM, 13)
DEF_VECTOR(TIM1_CC, 14)
//DEF_VECTOR(TIM3, 16)
//DEF_VECTOR(TIM6, 17)
//DEF_VECTOR(TIM14, 19)
DEF_VECTOR(TIM15, 20)
//DEF_VECTOR(TIM16, 21)
//DEF_VECTOR(TIM17, 22)
DEF_VECTOR(I2C1, 23)
DEF_VECTOR(I2C2, 24)
DEF_VECTOR(SPI1, 25)
DEF_VECTOR(SPI2, 26)
//DEF_VECTOR(USART1, 27)
DEF_VECTOR(USART2, 28)
//DEF_VECTOR(USART3_4_5_6, 29)
DEF_VECTOR(USB, 31)

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
