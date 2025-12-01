#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"

void LED_Init(void);
void Update_Number(uint8_t num);
void Display_Clear(void);
void Refresh_Display();
#endif