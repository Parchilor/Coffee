#ifndef __SPI_H_
#define __SPI_H_

#include <stdint.h>

void System_SPIx_Init(void);
void Test_SPIx_SendData(uint8_t *Data, uint8_t cnt);
#endif