#ifndef __MOTOR_H_
#define __MOTOR_H_

#include <stdint.h>

#define MT_STATE_DELAY	0x08
#define MT_STATE_STOP	0x04
#define MT_STATE_CW		0x02
#define MT_STATE_CCW	0x01
#define MT_STATE_IDLE	0x00

void Motor_CW(uint8_t dir);
void Motor_CCW(uint8_t dir);
void Motor_STOP(void);
#endif 