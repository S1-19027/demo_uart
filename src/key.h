#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

void KEY_Configuration(void);
int KEY_read(void);
uint8_t KEY_Scan(void);
void key_test(void);
int KEY_GetOneShot(void);
int Key_Scan_10ms(void);
int KEY_GetOneShot2(void);
int KEY_read_column(uint8_t col);
#endif

