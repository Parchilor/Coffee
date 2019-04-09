#ifndef __LED_H_
#define __LED_H_

extern void (*led_mode[])(uint8_t);
#define LED_SetMode(idx)	led_mode[idx]()

#endif