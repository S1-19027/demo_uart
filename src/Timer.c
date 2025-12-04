#include "stm32F10x_tim.h"


void tim6_init(uint16_t period, uint16_t prescaler)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;    
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	// void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);
	

	TIM_TimeBaseInitStructure.TIM_Period            = period;
	TIM_TimeBaseInitStructure.TIM_Prescaler         = prescaler;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure);
	
	// ?? TIM6 ? NVIC
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ARRPreloadConfig(TIM6, ENABLE);
	TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);   // ??????,???????
	TIM_Cmd(TIM6, DISABLE);  // ?????
}

// ???????
void tim6_start()
{
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
}
/**
  * @brief  This function handles TIM6 global interrupt request.
  * @param  None
  * @retval : None
  */

int T6_Second = 0;
int T6_1msConunt = 0,T6_10msCount=0  ;
int count;
void time6_handle()
{
		T6_1msConunt ++ ;
	

				TIM1->CCR1+=30;
				TIM1->CCR2+=50;		
	
	
		if (T6_1msConunt >=10)
		{
				T6_10msCount ++;
				T6_1msConunt-=10;
		
				
		}
	
		if (T6_10msCount > 100 )
		{
			T6_10msCount -= 100;
			T6_Second ++ ;
		}
}


void TIM6_IRQHandler(void)
{
    if(TIM6->SR & TIM_SR_UIF) // 
    {
        TIM6->SR = ~TIM_SR_UIF;
				time6_handle();
    }

    NVIC_ClearPendingIRQ(TIM6_IRQn);
}


void TIM4_Capture_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
 

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
 

    TIM_TimeBaseStructure.TIM_Period = 1000; // 1ms周期，1ms溢出一次
    TIM_TimeBaseStructure.TIM_Prescaler = 71; // 72分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // TIM4_CH2
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling; // 引脚初值为高电平，所以第一次触发用下降沿TIM4_CCER |=(1<<1); 		
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // ??????
    TIM_ICInitStructure.TIM_ICFilter = 0x0; // ?????
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
 
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 

    TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_CC1, ENABLE);

    TIM_Cmd(TIM4, ENABLE);
}
 
int KeyTimeStart,KeyTimeEnd,KeyCatureFlag = 0 ;
int KeyCatureTimeOver=0;
float keyPulsWidth;
void TIM4_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
    {
        if ( KeyCatureFlag == 0) 
				{
					KeyCatureFlag = 1;
					KeyCatureTimeOver = 0;
					KeyTimeStart = TIM4->CCR1 ;
					TIM4->CCER&=~(1<<1); 				//CC1P=0 设置为上升沿捕获
				}
				else
				{
					KeyCatureFlag = 0 ;
					KeyTimeEnd = TIM4->CCR1 ;
					TIM4->CCER |=(1<<1); 				//CC1P=0 设置为下降沿捕获

					keyPulsWidth = KeyCatureTimeOver+ (float)(KeyTimeEnd - KeyTimeStart)/1000;

				}
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
    }
    if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        if ( KeyCatureFlag == 1 ) 
					KeyCatureTimeOver ++ ; //在捕获中发生计数器溢出，溢出次数+1
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
}

#include "stm32f10x.h"
 
void PwmOutputInit(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // 使能TIM1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE); // 使能GPIOE时钟
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);
	
	
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
 
    // 配置PE9为复用功能
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
 
    // 初始化TIM1
		TIM_DeInit(TIM1);
    TIM_TimeBaseStructure.TIM_Period = 65535; // 周期
    TIM_TimeBaseStructure.TIM_Prescaler = 0; // 预分频器
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
 
    // 初始化TIM1 PWM模式
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 1000; // 初始占空比50%,  TIMx->CCR1 = 500;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    //TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM1 ch1输出比较预装载
		TIM_OCInitStructure.TIM_Pulse = 5535; // 初始占空比
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    //TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable); // 使能TIM1 ch1输出比较预装载\		
		
    TIM_ARRPreloadConfig(TIM1, ENABLE); // 使能TIM1在ARR上使能预装载寄存器
		
    TIM_Cmd(TIM1, ENABLE); // 使能TIM1		
    TIM_CtrlPWMOutputs(TIM1, ENABLE); // 使能TIM1的PWM输出 

		
}
 
