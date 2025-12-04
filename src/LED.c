#include "LED.h"
#include "SysTick.h"
// ?????????? (a-g, dp)
// 0=?, 1=?(???????????)
// ????:dp,g,f,e,d,c,b,a
const uint8_t SEGMENT_CODE[] = {
    0xC0, // 0 - a,b,c,d,e,f,g
    0xF9, // 1 - b,c
    0xA4, // 2 - a,b,d,e,g
    0xB0, // 3 - a,b,c,d,g
    0x99, // 4 - b,c,f,g
    0x92, // 5 - a,c,d,f,g
    0x82, // 6 - a,c,d,e,f,g
    0xF8, // 7 - a,b,c
    0x80, // 8 - a,b,c,d,e,f,g
    0x90, // 9 - a,b,c,d,f,g
    0x88, // A - a,b,c,e,f,g
    0x83, // b - c,d,e,f,g
    0xC6, // C - a,d,e,f
    0xA1, // d - b,c,d,e,g
    0x86, // E - a,d,e,f,g
    0x8E  // F - a,e,f,g
};

void LED_Init(void)
{
    RCC->APB2ENR |= 3 << 4; // ?? PORTC ? PORTD ??

    // ??PC10-13?????(???????)
    GPIOC->CRH &= 0xFF0000FF;
    GPIOC->CRH |= 0x00333300; // PC.10 11 12 13 ????

    // ??PD0-7?????(???????)
    GPIOD->CRL &= 0X00000000;

    GPIOD->CRL |= 0X33333333; // PD.0 1 2 3 4 5 6 7 ????

    // ???????:??????(??)
    GPIOD->BSRR = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                  GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;

    // ????:??????(???????)
    GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
}

// ?????,???????4???(0-15??0-F)
uint8_t display_buffer[4] = {16,16,16,16}; // ???????0,?????

void Update_Number(uint8_t num)
{    

    // ??????????(????)
    for (int i = 0; i < 3; i++)
    {
        display_buffer[i] = display_buffer[i + 1];
    }
    display_buffer[3] = num;

}

void Refresh_Display(void)
{
    for (int i = 3; i >= 0; i--)
    {
        // ???????
        GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;

        // ????????
        switch (i)
        {
        case 0:
            GPIOC->BRR = GPIO_Pin_10;
            break; // ??
        case 1:
            GPIOC->BRR = GPIO_Pin_11;
            break;
        case 2:
            GPIOC->BRR = GPIO_Pin_12;
            break;
        case 3:
            GPIOC->BRR = GPIO_Pin_13;
            break; // ??
        }

        // ???????
        if (display_buffer[i] < 16)
            GPIOD->ODR = (GPIOD->ODR & 0xFF00) | SEGMENT_CODE[display_buffer[i]];
        else
            GPIOD->ODR = (GPIOD->ODR & 0xFF00) | 0xFF; // ???

        delay_ms(5); // ????,??????
    }
}

void Display_Clear(void)
{
    // ???????
    for (int i = 0; i < 4; i++)
    {
        display_buffer[i] = 0;
    }

    // ???????
    GPIOC->BSRR = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIOD->ODR = (GPIOD->ODR & 0xFF00) | 0xFF;
}