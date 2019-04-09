#ifndef __USART_H_
#define __USART_H_
#include "stm32f0xx.h"
typedef enum {
	DB_COM,
	MAINCTRL,
	BATTERY,
} COM_IdxDef;

#define COM_Nbr 1
#define Buffer_Size 64

#define USARTn_Mode_Normal	1
#define USARTn_Mode_IRQ		2
#define USARTn_Mode_DMA		3

void USARTn_EN(void);
void USARTn_DIS(void);
FlagStatus USARTn_GetRxDown(void);
void System_COM_Init(uint32_t baudrate, uint8_t mode);

#endif