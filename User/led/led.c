#include "main.h"
#include "led.h"

uint8_t chg_cnt = 0;
extern uint8_t mt_cnt;
static uint8_t Get_VolLevel(void)
{
	int32_t value = ADC_ReadValue(1);
 	int8_t level = -1;
	if((value >= 2909) && (value < 3021))
	{
		level = 0x01;
	}
	else if((value >= 3021) && (value < 3132))
	{
		level = 0x02;
	}
	else if((value >= 3132) && (value < 3356))
	{
		level = 0x03;
	}
	else if((value >= 3356) && (value < 3579))
	{
		level = 0x04;
	}
	else if((value >= 3579) && (value < 3803))
	{
		level = 0x05;
	}
	else if((value >= 3803) && (value < 3914))
	{
		level = 0x06;
	}
	else if((value >= 3914) && (value < 4025))
	{
		level = 0x07;
	}
	else if((value >= 4025))
	{
		level = 0x08;
	}
	else
	{
		/* Err */
	}
	return level;
}

void LED_Mode_Energy(uint8_t Reserve)	
{
	uint8_t i, temp = 0;
	int8_t level = Get_VolLevel();
	if(level != -1)
	{
		for(i = 0; i < level; i++)
		{
			temp |= (0x01 << i);
		}
		LED_SetLight(0, temp);
	}
}

void LED_Mode_Charging(uint8_t Reserve)
{
	uint8_t i, temp = 0;
	int8_t level = 0;
	if(chg_cnt == 0)
	{
		level = Get_VolLevel();
		if(level != -1)
		{
			chg_cnt = 8 - level;
		}
	}
	else
	{
		chg_cnt--;
		level = 8 - chg_cnt;
	}
	if(level != -1)
	{
		for(i = 0; i < level; i++)
		{
			temp |= (0x01 << i);
		}
		LED_SetLight(0, temp);
	}
}

static void LED_Mode_Motor(uint8_t Reserve)
{
	LED_SetLight(0, (0x01 << mt_cnt++));
	if(mt_cnt > 8)
	{
		mt_cnt = 0;
	}
}

void (*led_mode[])(uint8_t ) = {
	LED_Mode_Energy,
	LED_Mode_Motor,
	LED_Mode_Charging,
};