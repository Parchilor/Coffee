#ifndef __GPIO_H_
#define __GPIO_H_

#include <stdint.h>

#include "stm32f0xx_conf.h"
#define MOTOR_L_PORT	GPIOF
#define MOTOR_L_PIN	GPIO_Pin_0
#define MOTOR_R_PORT	GPIOF
#define MOTOR_R_PIN	GPIO_Pin_1

#define VDET_EN_PORT	GPIOA
#define VDET_EN_PIN	GPIO_Pin_1
#define PWR_HOLD_PORT	GPIOA
#define PWR_HOLD_PIN	GPIO_Pin_5

#define PWR_SW_PORT	GPIOA
#define PWR_SW_PIN	GPIO_Pin_6
#define CHG_DOWN_PORT	GPIOA
#define CHG_DOWN_PIN	GPIO_Pin_9
#define CHG_PORT	GPIOA
#define CHG_PIN	GPIO_Pin_10

#define IO_CTRL(tag) tag##_PORT, tag##_PIN

#define IO_STATE_HI(tag)  (tag##_PORT->ODR |= tag##_PIN)	//GPIO_SetBits(tag##_PORT, tag##_PIN)
#define IO_STATE_LO(tag) (tag##_PORT->ODR &= (~tag##_PIN))
#define IO_STATE_TOGGLE(tag) (tag##_PORT->ODR ^= tag##_PIN)
#define IO_STATE_READ(tag) (!!(tag##_PORT->IDR & tag##_PIN))
#define IO_STATE_OUTPUT_READ(tag) (!!(tag##_PORT->ODR & tag##_PIN))


void System_GPIO_Init(void);

#endif