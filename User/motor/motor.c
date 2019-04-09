
#include "main.h"
/*
 * CW: 正转
 * CCW: 反转
 */

uint8_t mt_cnt = 0;

uint8_t motor_state = MT_STATE_IDLE;
void Motor_STOP(void)
{
#ifdef __DEBUG
	hal_debug("STOP\r\n");
#endif
	// motor_state = MT_STATE_STOP;
	// LED_LightOff();
	mt_cnt = 0;
	IO_STATE_LO(MOTOR_L);
	IO_STATE_LO(MOTOR_R);
}

void Motor_CW(uint8_t dir)
{
#ifdef __DEBUG
	hal_debug("CW\r\n");
#endif
	// motor_state = MT_STATE_CW;
	IO_STATE_LO(MOTOR_L);
	IO_STATE_LO(MOTOR_R);
	delay_ms(100);
	if(dir)
	{
		IO_STATE_LO(MOTOR_R);
		IO_STATE_HI(MOTOR_L);
	}
	else
	{
		IO_STATE_LO(MOTOR_L);
		IO_STATE_HI(MOTOR_R);
	}
}

void Motor_CCW(uint8_t dir)
{
#ifdef __DEBUG
	hal_debug("CCW\r\n");
#endif
	// motor_state = MT_STATE_CCW;
	IO_STATE_LO(MOTOR_L);
	IO_STATE_LO(MOTOR_R);
	delay_ms(100);
	if(dir)
	{
		IO_STATE_LO(MOTOR_L);
		IO_STATE_HI(MOTOR_R);
	}
	else
	{
		IO_STATE_LO(MOTOR_R);
		IO_STATE_HI(MOTOR_L);
	}
}
