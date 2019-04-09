#ifndef __PRINTF_H_
#define __PRINTF_H_
#include <stdint.h>


uint16_t USARTn_Rx_Head(void);
uint16_t USARTn_Rx_Tail(void);
void USARTn_RxBufferClear(void);
int USARTn_Rx_ReadByte(uint8_t timeout);
void USARTn_Rx_SendByte(uint8_t ch);
uint16_t USARTn_RxBufferIsEmpty(void);
uint16_t USARTn_RxBufferUsed(void);
uint8_t USARTn_RxBufferEOF(void);
// Tx Func
void USARTn_TxBufferClean(void);
uint8_t USARTn_Tx_ReadByte(void);
uint16_t USARTn_TxBufferIsEmpty(void);
// uint8_t wait_tx_space(void);
void USARTn_Tx_SendByte(uint8_t data);
// void USARTn_Tx_Send(uint8_t *msg);
void USARTn_Tx_Send(uint8_t *msg, uint32_t cnt);

int hal_debug(const signed char *fmt, ...);
#endif