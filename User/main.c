#include "main.h"
#include <string.h>

#define MOTOR_DIR	1

#define SYSTEM_USB_ON		0x02
#define SYSTEM_PWR_ON       0x01
#define SYSTEM_PWR_OFF      0x00

#define KEY_CHECK		1
#define KEY_PWRUP		2
#define KEY_UP			3

uint8_t system_state = SYSTEM_PWR_OFF;

extern uint32_t jiffies;
extern uint8_t motor_state;
extern uint8_t chg_cnt;
extern uint8_t mt_cnt;

uint32_t prev_mask = 0;
/*
 * 按键按下时，IO对应PIN状态为0
 * 按键未按下时，IO对应PIN状态为1
 */
uint8_t key_down = KEY_PWRUP;
uint16_t key_down_cnt = 0;
// uint8_t key_prev_state = 0;

uint16_t motor_cnt = 0;
uint8_t motor_err = 0;
uint8_t motor_prev_state = MT_STATE_IDLE;

int32_t vbat_value = 0;

uint8_t chg_state[2] = {0};

uint8_t test_idx = 1;
uint8_t test_btn_cnt = 1;
void main(void)
{
	RCC_ClocksTypeDef clock;
	System_Init(&clock);
	Motor_STOP();
	LED_LightOff();
	LED_Frame_Send();
	// hal_debug("Freqs: SYSCLK=%dMHz,HCLK=%dMHz,PCLK=%dMHz\r\n", 
	// 			clock.SYSCLK_Frequency / 1000 / 1000,
	// 			clock.HCLK_Frequency / 1000 / 1000,
	// 			clock.PCLK_Frequency / 1000 / 1000
	// 			);
#if 1
	TIM_Nornal_Init(TIM1, TIM_MS_PRE2(clock) / 2 - 1, 10 - 1);
	TIM_Cmd(TIM1, ENABLE);
	/* Check Charging */
	chg_state[0] = IO_STATE_READ(CHG);
	chg_state[1] = IO_STATE_READ(CHG_DOWN);
	// chg_state[0] = 0x00;
	// chg_state[1] = 0x01;
	if(!(chg_state[0] & chg_state[1]))
	{
		IO_STATE_TOGGLE(PWR_HOLD);
		system_state = SYSTEM_USB_ON;
		key_down = 0;
	}
	else
	{
		/* code */
	}
	
	while(1)
	{
		if(prev_mask != jiffies)
		{
			prev_mask = jiffies;
			if(prev_mask % 6 == 0)
			{
				// Flash LEDs
				LED_Frame_Send();
			}
			// End of Flash LEDs
			if(prev_mask % 5 == 0)
			{
				/* Check Charging */
#if 0
				chg_state[0] = IO_STATE_READ(CHG);
				chg_state[1] = IO_STATE_READ(CHG_DOWN);
				// chg_state[0] = 0x00;
				// chg_state[1] = 0x01;
				if(!(chg_state[0] & chg_state[1]))
				{
					system_state = SYSTEM_USB_ON;
				}
#else
				if(system_state != test_idx)
				{
					test_idx = system_state;
					chg_cnt = 0;
					mt_cnt = 0;
				}
#endif
			}
			if(prev_mask % 3 == 0) /* Key Check 15ms per times */
			{
				// Button Control
				if(IO_STATE_READ(PWR_SW) == 1 && key_down == 0)
				{
					key_down = KEY_CHECK;
				}
				if(key_down == KEY_CHECK)
				{
					if(IO_STATE_READ(PWR_SW) == 1)
					{
						key_down_cnt++;
						if((key_down_cnt == 80) && (system_state == SYSTEM_PWR_ON))
						{
							// /* 长按操作 假装关机 */
							// system_state = SYSTEM_PWR_OFF;
							// 设置好标志，关灯关电机
							key_down_cnt = 0;
							key_down = KEY_UP;
							system_state = SYSTEM_PWR_OFF;
							LED_LightOff();
							Motor_STOP();
							IO_STATE_TOGGLE(PWR_HOLD);
						}
					}
					else
					{
						if(key_down_cnt < 80 && (system_state == SYSTEM_PWR_ON | SYSTEM_USB_ON))
						{
#if 1
							// 短按操作
							if(motor_state & MT_STATE_IDLE)
							{
								motor_state = MT_STATE_CW;
							}
							else if(motor_state & (MT_STATE_CW | MT_STATE_CCW))
							{
								motor_state = MT_STATE_CCW;
							}
							else if(motor_state & (MT_STATE_STOP | MT_STATE_DELAY))
							{

							}
#else
							if(test_btn_cnt == 0)
							{
								system_state = SYSTEM_PWR_ON;
							}
							else if(test_btn_cnt == 1)
							{
								motor_state = MT_STATE_CW;
							}
							
							if(++test_btn_cnt > 1)
							{
								test_btn_cnt = 0;
							}
#endif
						}
						key_down_cnt = 0;
						key_down = 0;
					}
				}
				else if(key_down == KEY_PWRUP)
				{
					key_down_cnt++;
					if(system_state == SYSTEM_PWR_ON)
					{
						key_down = KEY_UP;
					}
					else if((key_down_cnt >= 80) && (system_state == SYSTEM_PWR_OFF))
					{
						// 延时开机完成
						IO_STATE_TOGGLE(PWR_HOLD);
						system_state = SYSTEM_PWR_ON;
						key_down = KEY_UP;
						LED_LightOff();
						led_mode[0](test_idx);
					}
				}
				else if(key_down == KEY_UP)
				{
					if(IO_STATE_READ(PWR_SW) == 0)
					{
						key_down_cnt = 0;
						key_down = 0;
					}
				}
			}
			// End of Key

			if((prev_mask % 100 == 0) && (motor_state == MT_STATE_IDLE)) /* LED Control */
			{
				if(system_state == SYSTEM_PWR_ON)
				{
					led_mode[0](test_idx);
				}
				else if(system_state == SYSTEM_USB_ON)
				{
					led_mode[2](test_idx);
				}
			}
			else if((prev_mask % 10 == 0) && (motor_state != MT_STATE_IDLE))
			{
				led_mode[1](test_idx);
			}
			// End of LED Ctrl

			if(motor_state == MT_STATE_IDLE && motor_err == 0)
			{
				
			}
			else if(motor_state == MT_STATE_CW && motor_err == 0)
			{
				Motor_CW(MOTOR_DIR);
				motor_prev_state = MT_STATE_CW;
				motor_state = MT_STATE_DELAY;
			}
			else if(motor_state == MT_STATE_CCW && motor_err == 0)
			{
				Motor_CCW(MOTOR_DIR);
				motor_prev_state = MT_STATE_CCW;
				motor_state = MT_STATE_DELAY;
			}
			else if(motor_state == MT_STATE_STOP && motor_err == 0)
			{
				Motor_STOP();
				motor_prev_state = MT_STATE_IDLE;
				motor_state = MT_STATE_IDLE;
				motor_cnt = 0;
				chg_cnt = 0;
			}
			else if(motor_state == MT_STATE_DELAY && motor_err == 0)
			{
				motor_cnt++;
				if(motor_prev_state == MT_STATE_CW)
				{
					if(motor_cnt++ >= 1000)
					{
						motor_prev_state = MT_STATE_DELAY;
						motor_state = MT_STATE_CCW;
						motor_cnt = 0;
					}
				}
				else if(motor_prev_state == MT_STATE_CCW)
				{
					if(motor_cnt++ >= 50)
					{
						motor_prev_state = MT_STATE_DELAY;
						motor_state = MT_STATE_STOP;
						motor_cnt = 0;
					}
				}
			}
			// End of motor
		}
		// End of 5ms
	}
#else
	TIM_Nornal_Init(TIM1, TIM_MS_PRE2(clock) / 2 - 1, 2 - 1);
	TIM_Cmd(TIM1, ENABLE);
	while(1)
	{
		if(jiffies % 5 == 0 && prev_mask != jiffies)
		{
			prev_mask = jiffies;
			if(prev_mask % 30 == 0)
			{
				// LED Control
				LED_Frame_Send();
			}
			if(prev_mask % 15 == 0)
			{
				// Button Control
				if(IO_STATE_READ(PWR_SW) == 1 && key_down == 0)
				{
					key_down = 1;
				}
				if(key_down == 1)
				{
					if(IO_STATE_READ(PWR_SW) == 1)
					{
						key_down_cnt++;
						if((key_down_cnt == 80) && (system_state == SYSTEM_PWR_ON))
						{
							// /* 长按操作 假装关机 */
							// system_state = SYSTEM_PWR_OFF;
							// 设置好标志，关灯关电机
							motor_flag = 0;
							key_down_cnt = 0;
							key_down = 3;
							system_state = SYSTEM_PWR_OFF;
							LED_LightOff();
							Motor_STOP();
							IO_STATE_TOGGLE(PWR_HOLD);
						}
					}
					else
					{
						if((key_down_cnt < 80) && (system_state == SYSTEM_PWR_ON))
						{
							// 短按操作
							// if(motor_flag == 0)
							// {
							// 	motor_flag = 1;
							// }
							// else
							// {
							// 	// 使用计数参数提前结束当前流程
							// 	motor_flag = 2;
							// 	motor_cnt = 1000;
							// }
						}
						key_down_cnt = 0;
						key_down = 0;
					}
				}
				else if(key_down == 2)
				{
					key_down_cnt++;
					
					if(IO_STATE_READ(PWR_SW) == 0)
					{
						key_down_cnt = 0;
						key_down = 0;
					}
					else if((key_down_cnt >= 80) && (system_state == SYSTEM_PWR_OFF))
					{
						// 延时开机完成
						IO_STATE_TOGGLE(PWR_HOLD);
						system_state = SYSTEM_PWR_ON;
						LED_LightOff();
					}
				}
				else if(key_down == 3)
				{
					if(IO_STATE_READ(PWR_SW) == 0)
					{
						key_down_cnt = 0;
						key_down = 0;
					}
				}
				
			}

			if((prev_mask % 500 == 0) & (system_state == SYSTEM_PWR_ON))
			{
				// Battery Energy LED Control
				// LED_SetLight(0, (0x01 << test_idx++));
				led_mode[2](test_idx++);
				// if(test_idx == 9) test_idx = 1;
			}
			// if(motor_flag == 1 && motor_err == 0)
			// {
			// 	// Motor Contorl
			// 	Motor_CW(MOTOR_DIR);
			// 	motor_cnt++;
			// 	motor_flag = 2;
			// }
			// else if(motor_flag == 2 && motor_err == 0)
			// {
			// 	motor_cnt++;
			// 	if(motor_cnt >= 1000)
			// 	{
			// 		Motor_CCW(MOTOR_DIR);
			// 		motor_cnt = 0;
			// 		motor_flag = 3;
			// 	}
			// }
			// else if(motor_flag == 3)
			// {
			// 	motor_cnt++;
			// 	if(motor_cnt >= 50)
			// 	{
			// 		Motor_STOP();
			// 		motor_flag = 0;
			// 		motor_cnt = 0;
			// 	}
			// }
			// else if(motor_err != 0)
			// {
			// 	Motor_STOP();
			// 	motor_cnt = 0;
			// 	motor_flag = 0;
			// }
		}
	}
#endif
}