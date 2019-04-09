#ifndef __LED_FRAME_H_
#define __LED_FRAME_H_

#define CHIP_Nbr	1
#define RST_FILL_Nbr	11
#define LED_Nbr_Per_CHIP	3
#define Color_Nbr	3
#define Bit_Per_Color	8
#define Byte_Per_Bit	3

#define LED_R   0
#define LED_G   1
#define LED_B   2

extern uint8_t led_bright;

// #define LED_Frame_Size	((CHIP_Nbr + RST_FILL_Nbr) * LED_Nbr_Per_CHIP * Color_Nbr * Bit_Per_Color * Byte_Per_Bit)
#define LED_Frame_Size	((CHIP_Nbr) * LED_Nbr_Per_CHIP * Color_Nbr * Bit_Per_Color * Byte_Per_Bit)
#define LED_Set_Bright(brt) (led_bright = brt)
#define LED_Bright()    led_bright

void LED_Frame_Send(void);
void LED_SetColor(uint8_t CHIPx, uint8_t LEDx, uint8_t R, uint8_t G, uint8_t B);
void LED_SetLight(uint8_t CHIPx, uint8_t mask);
void LED_LightOff(void);
void LED_LightOn(void);

#endif