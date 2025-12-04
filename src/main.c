/**
  ******************************************************************************
  * @file InputCaptureMode/main.c 
  * @author   MCD Application Team
  * @version  V3.0.0
  * @date     04/27/2009
  * @brief    Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */ 

/** @addtogroup InputCaptureMode
  * @{
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "LED.h"
#include "key.h"
#include "sys.h"
#include "EXTI.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);

//#define _Exp_LED_  //ok
#define _Exp_KEY_	//ok
//#define _EXP_BEEP_	//ok
//#define _EXP_UART_	//ok
//#define _EXP_LCD_		//OK
//#define _EXP_SPI_  //OK
//#define _EXP_AIAO_  //AI OK， AO not supported by 103VB
//#define _EXP_COUNTER_  /OK
//#define _EXP_IIC_		 //OK
//#define _EXP_PWM_		 //正极输出OK，负极没有。另外无法满足同时三个通道，remap有问题
//#define _EXP_7SEG_		 //OK

void BoardInit()
{
  /* System Clocks Configuration */
	SystemInit();

}
// main.c ????:
extern volatile int key_flag;
extern volatile uint8_t column_trigger;
extern volatile uint8_t scan_flag ;

int FlashID;
u8 SPIFlashStatus;
u8 FlashBuf[256] ;

int LastCode = -1;   // ?????????
static int Age = 0;         // ??????
int stable_key = -1; // ???????
int key=-1;
int main(void)
{
	//int i ;
	BoardInit();
	//for (i = 0 ; i < 256 ; i++) FlashBuf[i] = i;
//	
	 
//LED_Configuration();
LED_Init();
  
  //KEY_Configuration(); // ?????????

//指示灯--------
//	LED_Configuration();//指示灯配置，本例无用
	
	
//------------定时器测试------------
	systick_init();	
	//tim6_init(1000,72-1);
	//tim6_start();
	//PwmOutputInit(); //	TIM1_CH1N作为PWM输出
	//PWM_Configuration(1000,71);
	//PWM_test();
	
//------------PB6按键------------	
//	Count_KEY_Init();  	//配置引脚为输入
	//EXTI_test(); 		//外部中断测试
	 EXITInit();
	//TIM4_Capture_Init();//输入脉宽测试

//------------SPI flash测试------------	
	//ENC25Q80_SPI2_Init();
//	FlashID = SPI_FLASH_ReadDeviceID();	
	
	//SPI2_WriteDisable();
//	SPIFlashStatus = SPI2_ReadStatus();
	//SPI2_WriteEnable();
	//SPIFlashStatus = SPI2_ReadStatus();
	
	//EraseSector(0);
//	ProgramPage(0,FlashBuf) ;
	//ReadPage(0,FlashBuf) ;
while(1) {
    if(key_flag) {
        int CurrentCode = KEY_read_column(column_trigger);
        
        // ? ????????
        if(CurrentCode == -1) {
            // ????,????
            Age = 0;
            LastCode = -1;
            // ???key_flag,???????
        } 
        // ? ????
        else if(CurrentCode != -1) {
            if(CurrentCode == LastCode) {
                if(Age < 5) Age++;
            } else {
                
                LastCode = CurrentCode;
            }
            
            // ????
            if(Age >= 2) {
                key = CurrentCode;
                Age = 0;  // ? ??Age=10,??????
                key_flag = 0;  // ????,????
                column_trigger = 0xFF;  // ?????
                // ? ????LastCode,????????
            }
        }
    }
    
    // ????
    if(key != -1) {
        Update_Number(key);
        key = -1;
    }
    
    // ??
    if(scan_flag > 0) {
			Refresh_Display();
       scan_flag = 0;
     
    }
		//Refresh_Display();
		//delay_us2(10);
}
}


/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
