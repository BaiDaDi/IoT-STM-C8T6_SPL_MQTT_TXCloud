
#ifndef __LED_H_
#define __LED_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

//Ö¸Ê¾µÆ
enum
{
	LED_R = 1,
	LED_G,
	LED_B,
};

void LED_Init(void);
void LED_Flash(void);
void LED_Open(u8 led);
void LED_Close(u8 led);
void LED_Toggle(u8 led);

#endif
