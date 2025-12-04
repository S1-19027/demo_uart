//systick.c
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_dma.h"

int TimeSecond = 0;
int Time1msConunt = 0  ;

volatile uint32_t ms_tick = 0;
volatile uint8_t  scan_flag = 0;

void time_handle()
{
		Time1msConunt ++ ;
		if ( Time1msConunt >= 1000)
		{ 
			Time1msConunt -= 1000;
			TimeSecond ++ ;
		}
}

volatile int time1ms ;
void SysTick_Handler(void)
{
  /* Update the LocalTime by adding SYSTEMTICK_PERIOD_MS each SysTick interrupt */
	time_handle();
     // ? 2ms ??????(??????????)
    static uint8_t cnt = 0;
    cnt++;
    if (cnt >= 1)
    {
        cnt = 0;
        scan_flag = 1;
    }    // ? 2ms ??????(??????????)
   
}

void systick_init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
	
    /* SystTick configuration: an interrupt every 1ms */
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
    /* Configure Systick clock source as HCLK */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
}