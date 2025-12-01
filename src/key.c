/***********************************************************************
文件名称：LED.C
功    能：led  IO初始化
编写时间：2013.4.25
编 写 人：
注    意：
***********************************************************************/
#include "stm32f10x.h"
#include "stdio.h"
#include "Register_config.h"
#include "key.h"
#define ROW_PORT GPIOE
#define COL_PORT GPIOE

#define ROW_1 GPIO_Pin_4
#define ROW_2 GPIO_Pin_5
#define ROW_3 GPIO_Pin_6
#define ROW_4 GPIO_Pin_7

#define COL_1 GPIO_Pin_0
#define COL_2 GPIO_Pin_1
#define COL_3 GPIO_Pin_2
#define COL_4 GPIO_Pin_3

void KEY_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7; //行
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(ROW_PORT,&GPIO_InitStructure);
    //
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //列
    	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    	GPIO_Init(GPIOE,&GPIO_InitStructure);

    /*****************寄存器版**********************/
    //	RCC->APB2ENR|=1<<6;    //使能PORTE时钟
    //	GPIOE->CRL=0X33338888;//PE.0 1 2 3  上拉输入  4 5 6 7 推挽输出

    //RCC_APB2ENR |= 1 << 6;  // 使能PORTE时钟
    //GPIOE_CRL = 0X33338888; // PE.0 1 2 3  上拉输入  4 5 6 7 推挽输出
}
int KEY_read(void)
{
    int key_value = -1;
    uint8_t key_1 = 1;
    uint8_t key_2 = 1;
    uint8_t key_3 = 1;
    uint8_t key_4 = 1;

    // ????? (ROW_1=0, ROW_2,3,4=1)
    GPIO_ResetBits(ROW_PORT, ROW_1);
    GPIO_SetBits(ROW_PORT, ROW_2 | ROW_3 | ROW_4);
    
    key_1 = GPIO_ReadInputDataBit(COL_PORT, COL_1);
    key_2 = GPIO_ReadInputDataBit(COL_PORT, COL_2);
    key_3 = GPIO_ReadInputDataBit(COL_PORT, COL_3);
    key_4 = GPIO_ReadInputDataBit(COL_PORT, COL_4);

    if (key_1 == Bit_RESET)
    {
        key_value = 0;
    }
    if (key_2 == Bit_RESET)
    {
        key_value = 1;
    }
    if (key_3 == Bit_RESET)
    {
        key_value = 2;
    }
    if (key_4 == Bit_RESET)
    {
        key_value = 3;
    }

    // ????? (ROW_2=0, ROW_1,3,4=1)
    GPIO_ResetBits(ROW_PORT, ROW_2);
    GPIO_SetBits(ROW_PORT, ROW_1 | ROW_3 | ROW_4);
    
    key_1 = GPIO_ReadInputDataBit(COL_PORT, COL_1);
    key_2 = GPIO_ReadInputDataBit(COL_PORT, COL_2);
    key_3 = GPIO_ReadInputDataBit(COL_PORT, COL_3);
    key_4 = GPIO_ReadInputDataBit(COL_PORT, COL_4);

    if (key_1 == Bit_RESET)
    {
        key_value = 4;
    }
    if (key_2 == Bit_RESET)
    {
        key_value = 5;
    }
    if (key_3 == Bit_RESET)
    {
        key_value = 6;
    }
    if (key_4 == Bit_RESET)
    {
        key_value = 7;
    }

    // ????? (ROW_3=0, ROW_1,2,4=1)
    GPIO_ResetBits(ROW_PORT, ROW_3);
    GPIO_SetBits(ROW_PORT, ROW_1 | ROW_2 | ROW_4);
    
    key_1 = GPIO_ReadInputDataBit(COL_PORT, COL_1);
    key_2 = GPIO_ReadInputDataBit(COL_PORT, COL_2);
    key_3 = GPIO_ReadInputDataBit(COL_PORT, COL_3);
    key_4 = GPIO_ReadInputDataBit(COL_PORT, COL_4);

    if (key_1 == Bit_RESET)
    {
        key_value = 8;
    }
    if (key_2 == Bit_RESET)
    {
        key_value = 9;
    }
    if (key_3 == Bit_RESET)
    {
        key_value = 10;
    }
    if (key_4 == Bit_RESET)
    {
        key_value = 11;
    }

    // ????? (ROW_4=0, ROW_1,2,3=1)
    GPIO_ResetBits(ROW_PORT, ROW_4);
    GPIO_SetBits(ROW_PORT, ROW_1 | ROW_2 | ROW_3);
    
    key_1 = GPIO_ReadInputDataBit(COL_PORT, COL_1);
    key_2 = GPIO_ReadInputDataBit(COL_PORT, COL_2);
    key_3 = GPIO_ReadInputDataBit(COL_PORT, COL_3);
    key_4 = GPIO_ReadInputDataBit(COL_PORT, COL_4);

    if (key_1 == Bit_RESET)
    {
        key_value = 12;
    }
    if (key_2 == Bit_RESET)
    {
        key_value = 13;
    }
    if (key_3 == Bit_RESET)
    {
        key_value = 14;
    }
    if (key_4 == Bit_RESET)
    {
        key_value = 15;
    }

    return key_value;
}

int Key_Scan_10ms(void)
{
    static int LastCode = -1;	
    static int Age = 0;

    int CurrentCode = KEY_read();  // ??????????

    if(CurrentCode == LastCode)
    {
        if(Age < 20) Age++;   // ?????
    }
    else
    {
        Age = 0;              // ????
        LastCode = CurrentCode;
    }

    // ???? 10 ?(100ms)????? ? ?????
    if(Age == 10)
    {
        return LastCode;
    }

    return -1;
}

u8 KEY_Scan()//??????
{
    u8 key_1 = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6);
    if (key_1 == 0)
    {
        delay_ms2(100); // 去抖动
        if (key_1 == 0)
            return 1;
    }
    return 0; // 无按键按下
}
int KEY_GetOneShot(void)
{
    static int stable_key = -1;
    int key = Key_Scan_10ms();

    if(key >= 0 && stable_key == -1)
    {
        stable_key = key;     // ????
        return key;           // ????
    }

    if(key < 0)
    {
        stable_key = -1;      // ?????????
    }

    return -1;
}

int keydown = 0;
void key_test()
{
    KEY_Configuration();
    while (1)
    {
        keydown = KEY_read();
        if (keydown)
        {
            printf("%d\n", keydown);
            keydown = 0;
            delay_ms(2000);
        }
    }
}


