#include "main.h"


void System_COM_Init(uint32_t baudrate, uint8_t mode)
{
	USART_InitTypeDef USART_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	USART_DeInit(USART1);
	USART_StructInit(&USART_InitStruct);
	USART_InitStruct.USART_BaudRate = baudrate;
	USART_Init(USART1, &USART_InitStruct);
	USART_ClearITPendingBit(USART1, USART_IT_TC);
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	// USART_ITConfig(USART1, USART_IT_PE, ENABLE);
	// USART_ITConfig(USART1, USART_IT_FE, ENABLE);
	// USART_ITConfig(USART1, USART_IT_ORE, ENABLE);
	// USART_ITConfig(USART1, USART_IT_NE, ENABLE);

	/* Enable USART */
	USART_Cmd(USART1, ENABLE);

	if(mode == USARTn_Mode_Normal)
	{

	}
	else if(mode == USARTn_Mode_IRQ)
	{
		NVIC_InitTypeDef NVIC_InitStruct;
		NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
		NVIC_InitStruct.NVIC_IRQChannelPriority = 0x03;
		NVIC_Init(&NVIC_InitStruct);
	}
	else if(mode == USARTn_Mode_DMA)
	{
		
	}

}

/* USARTn_IRQn Tx EN */
void USARTn_EN(void)
{
   USART_ITConfig(USART1, USART_IT_TC, ENABLE);
	// DMA_Cmd(DMA1_Channel4, ENABLE);
}

void USARTn_DIS(void)
{
   USART_ITConfig(USART1, USART_IT_TC, DISABLE);
	// DMA_Cmd(DMA1_Channel4, DISABLE);
}

FlagStatus USARTn_GetRxDown(void)
{
   return USART_GetFlagStatus(USART1, USART_FLAG_RXNE);
}

void (*__rx_upload)(uint8_t ch) = USARTn_Rx_SendByte;
uint8_t (*__tx_download)(void) = USARTn_Tx_ReadByte;
uint16_t (*__tx_isempty)(void) = USARTn_TxBufferIsEmpty;

static void USARTn_IRQHandler(void)
{
	int ch = -1;
	USART_TypeDef *UARTn = USART1;
	volatile unsigned short SR = UARTn->ISR;
	if (SR & USART_FLAG_TC)
	{
		if (__tx_isempty())
		{
			USART_ITConfig(UARTn, USART_IT_TC, DISABLE);
		}
		else
		{
			ch = __tx_download();
			USART_SendData(UARTn, (unsigned char)ch);
		}
	}

	else if (SR & USART_FLAG_RXNE)
	{
		if (SR & USART_FLAG_RXNE)
		{
			ch = USART_ReceiveData(UARTn);
			__rx_upload(ch);
		}
	}
	else if(SR & (USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_ORE | USART_FLAG_NE))
	{
		USART_ClearFlag(UARTn, (USART_FLAG_PE | USART_FLAG_FE | USART_FLAG_ORE | USART_FLAG_NE));
	}
}

void USART1_IRQHandler(void)
{
	USARTn_IRQHandler();
}