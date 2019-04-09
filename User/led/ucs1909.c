#include "main.h"
#include <string.h>

/* R1	G1	B1	R2	G2	B2	R3	G3	B3*/
/* 数值为亮度 */
//static uint8_t led_test_frame[] = {{0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}, {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}, {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}, {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}, {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}, {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}, {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}, {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}, {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE}};

// uint16_t led_frame_mask = 0;
uint8_t led_bright = 0x22;

uint8_t LED_Frame[CHIP_Nbr][LED_Nbr_Per_CHIP][Color_Nbr][Bit_Per_Color][Byte_Per_Bit] = {
	{/* Chip 1*/
		{/* LED1 RGB*/
			{// LED_Frame[0][0][0][0]
				{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, 
			}, 
			{
				{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, 
			}, 
			{
				{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, 
			}, 
		},
		{/* LED2 RGB*/
			{
				{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, 
			}, 
			{
				{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, 
			}, 
			{
				{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, 
			}, 
		},
		{/* LED3 RGB*/
			{
				{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, 
			}, 
			{
				{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, 
			}, 
			{
				{0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, {0xFF, 0x00, 0x00}, 
			}, 
		},
	},
};

extern uint8_t led_frame[];

uint8_t LED_Level[2][Byte_Per_Bit] = {
	{0xFF, 0x00, 0x00},
	{0xFF, 0xFE, 0x00},
};

void LED_Frame_Send(void)
{
// 	memcpy(led_frame, LED_Frame, LED_Frame_Size);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel3, LED_Frame_Size);
	DMA_Cmd(DMA1_Channel3, ENABLE);
}

/**
  * @brief  Set LED per Color On per channel on chip
  * @param  CHIPx: chip serial Number.
  * @param  color_mask: light on mask.
  * @param  bright: led bright.
  * @retval Set A Chip LED On or Off.
  */
void LED_SetColor(uint8_t CHIPx, uint8_t LEDx, uint8_t R, uint8_t G, uint8_t B)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		if(0x80 == (R & 0x80)) memcpy(LED_Frame[CHIPx][LEDx][0][i], &LED_Level[1], Byte_Per_Bit); else memcpy(LED_Frame[CHIPx][LEDx][0][i], &LED_Level[0], Byte_Per_Bit);
		if(0x80 == (G & 0x80)) memcpy(LED_Frame[CHIPx][LEDx][1][i], &LED_Level[1], Byte_Per_Bit); else memcpy(LED_Frame[CHIPx][LEDx][1][i], &LED_Level[0], Byte_Per_Bit);
		if(0x80 == (B & 0x80)) memcpy(LED_Frame[CHIPx][LEDx][2][i], &LED_Level[1], Byte_Per_Bit); else memcpy(LED_Frame[CHIPx][LEDx][2][i], &LED_Level[0], Byte_Per_Bit);
		R <<= 1;
		G <<= 1;
		B <<= 1;
	}
}

void LED_SetOneColor(uint8_t CHIPx, uint8_t LEDx, uint8_t Color, uint8_t bright)
{
	uint8_t i;
	for(i = 0; i < 8; i++)
	{
		if(bright & 0x80) 
		{
			memcpy(LED_Frame[CHIPx][LEDx][Color][i], &LED_Level[1], Byte_Per_Bit);
		}
		else
		{
			memcpy(LED_Frame[CHIPx][LEDx][Color][i], &LED_Level[0], Byte_Per_Bit);
		}
		bright <<= 1;
	}
}

void LED_SetLight(uint8_t CHIPx, uint8_t mask)
{
	// R
	if(mask & (0x01 << 0)) LED_SetOneColor(CHIPx, 0, LED_R, led_bright); else LED_SetOneColor(CHIPx, 0, LED_R, 0x00);
	if(mask & (0x01 << 1)) LED_SetOneColor(CHIPx, 0, LED_G, led_bright); else LED_SetOneColor(CHIPx, 0, LED_G, 0x00);
	if(mask & (0x01 << 2)) LED_SetOneColor(CHIPx, 0, LED_B, led_bright); else LED_SetOneColor(CHIPx, 0, LED_B, 0x00);
	// G
	if(mask & (0x01 << 3)) LED_SetOneColor(CHIPx, 1, LED_R, led_bright); else LED_SetOneColor(CHIPx, 1, LED_R, 0x00);
	if(mask & (0x01 << 4)) LED_SetOneColor(CHIPx, 1, LED_G, led_bright); else LED_SetOneColor(CHIPx, 1, LED_G, 0x00);
	if(mask & (0x01 << 5)) LED_SetOneColor(CHIPx, 1, LED_B, led_bright); else LED_SetOneColor(CHIPx, 1, LED_B, 0x00);
	// B
	if(mask & (0x01 << 6)) LED_SetOneColor(CHIPx, 2, LED_R, led_bright); else LED_SetOneColor(CHIPx, 2, LED_R, 0x00);
	if(mask & (0x01 << 7)) LED_SetOneColor(CHIPx, 2, LED_G, led_bright); else LED_SetOneColor(CHIPx, 2, LED_G, 0x00);
	// if(mask & (0x01 << 8)) LED_SetOneColor(CHIPx, 2, LED_B, 0x00); else LED_SetOneColor(CHIPx, 2, LED_B, 0x00);
	LED_SetOneColor(CHIPx, 2, LED_B, 0x00);
}

void LED_LightOff(void)
{
	uint8_t i;
	for(i = 0; i < 3; i++)
	{
		LED_SetColor(0, i, 0x00, 0x00, 0x00);
	}
}

void LED_LightOn(void)
{
	uint8_t i;
	for(i = 0; i < 3; i++)      
	{
		LED_SetColor(0, i, led_bright, led_bright, led_bright);
	}
}
void LED_CleanColor(void)
{
	memset(led_frame, 0, LED_Frame_Size);
}