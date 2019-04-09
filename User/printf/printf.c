#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

#include "main.h"

#define Tx_Buffer_Size Buffer_Size
#define Rx_Buffer_Size Buffer_Size

//#define COM_Nbr 1
#define DB_COM 0

uint8_t __tx_buffer[Tx_Buffer_Size];
uint8_t __rx_buffer[Rx_Buffer_Size];
unsigned short __tx_buffer_head = 0, __tx_buffer_tail = 0;
unsigned short __rx_buffer_head = 0, __rx_buffer_tail = 0;

// TxD Functions
static void __tx_buffer_putc(uint8_t data)
{
	hal_flags flags;

	save_and_cli(flags);
	__tx_buffer[__tx_buffer_head++] = data;
	__tx_buffer_head %= Tx_Buffer_Size;
	restore_flags(flags);
}

static uint32_t __tx_buffer_puts(const uint8_t *msg, uint32_t cnt)
{
	uint8_t *p = (uint8_t *)msg;
	while(cnt--)
	{
		__tx_buffer_putc((uint8_t)*p);
		p++;
	}
	return p - msg;
}

static uint8_t __tx_buffer_getc(void)
{
	uint8_t data;
	hal_flags flags;

	save_and_cli(flags);
	data = __tx_buffer[__tx_buffer_tail++];
	__tx_buffer_tail %= Tx_Buffer_Size;
	restore_flags(flags);
	return data;
}

static uint16_t __tx_buffer_empty(void)
{
	return (__tx_buffer_tail == __tx_buffer_head);
}

static void __tx_buffer_clear(void)
{
	__tx_buffer_tail = __tx_buffer_head = 0;
}

// RxD Functions
static void __rx_buffer_putc(uint8_t data)
{
	hal_flags flags;

	save_and_cli(flags);
	__rx_buffer[__rx_buffer_head++] = data;
	__rx_buffer_head %= Rx_Buffer_Size;
	restore_flags(flags);
}

static uint8_t __rx_buffer_getc(void)
{
	uint8_t data;
	hal_flags flags;

	save_and_cli(flags);
	data = __rx_buffer[__rx_buffer_tail++];
	__rx_buffer_tail %= Rx_Buffer_Size;
	restore_flags(flags);
	// hal_debug("%02x", data);
	
	return data;
}

static uint16_t __rx_buffer_empty(void)
{
	return (__rx_buffer_tail == __rx_buffer_head);
}

//static uint16_t __rx_buffer_space(void)
//{
//	return Rx_Buffer_Size - __rx_buffer_used();
//}

static void __rx_buffer_clear(void)
{
	// __rx_buffer_tail = __rx_buffer_head = 0;
	__rx_buffer_head = __rx_buffer_tail;
}

// Rx Func
uint16_t USARTn_Rx_Head(void)
{
	return __rx_buffer_head;
}

uint16_t USARTn_Rx_Tail(void)
{
	return __rx_buffer_tail;
}

int USARTn_Rx_ReadByte(uint8_t timeout)
{
	timeout *= 1000;
	do
	{
		if (!__rx_buffer_empty())
		{
			return __rx_buffer_getc();
		}
	} while (timeout--);
	return -1;
}

void USARTn_Rx_SendByte(uint8_t ch)
{
	__rx_buffer_putc(ch);
}

uint16_t USARTn_RxBufferIsEmpty(void)
{
	return __rx_buffer_empty();
}

// uint8_t UARTn_Search_7f()
// {
// 	int tail = __rx_buffer_tail;
// 	while(tail != __rx_buffer_head)
// 	{
// 		if(__rx_buffer[tail] == 0x7f)
// 		{
// 			return 1;
// 		}
// 		tail += 1;
// 		tail %= Rx_Buffer_Size;
// 	}
// 	return 0;
// }

uint8_t USARTn_RxBufferEOF(void)
{
	uint16_t tmp = __rx_buffer_head - 1;
	return __rx_buffer[tmp % Rx_Buffer_Size];
}

uint16_t USARTn_RxRecvCnt(void)
{
	return 0;
}

// Tx Func
uint8_t USARTn_Tx_ReadByte(void)
{
	if (__tx_buffer_head == __tx_buffer_tail)
		return 0;
	return __tx_buffer_getc();
}

uint16_t USARTn_TxBufferIsEmpty(void)
{
	return __tx_buffer_empty();
}

void USARTn_RxBufferClear(void)
{
	__rx_buffer_clear();
}

uint8_t wait_tx_space(void)
{
	// __wait_tx_space();
	while (__tx_buffer_head != __tx_buffer_tail)
		;
	return 0;
}

void USARTn_Tx_Send(uint8_t *msg, uint32_t cnt)
{
	USARTn_DIS();
	__tx_buffer_puts(msg, cnt);
	USARTn_EN();
	wait_tx_space();
}

int hal_debug(const signed char *fmt, ...)
{
	va_list args;
	int len;
	signed char buff[Buffer_Size] = "";

	va_start(args, fmt);
	len = vsnprintf((char *)buff, sizeof(buff) - 1, (char const *)fmt, args);
	va_end(args);

	buff[len] = 0;
	USARTn_Tx_Send((uint8_t *)buff, len);
	return len;
}