#include "pwm.h"

void PWM_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;                             
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOB, ENABLE);  //开启引脚时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;          //引脚复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	
/*******************************************************************************
*  Time_BASE  =  ((1+TIM_Prescaler)/90M)*(1+TIM_Period)                 //APB2-90M              APB1-45M
*  Time       =  ((1+99)/90M)*(1+8999)  =  10ms
*******************************************************************************/
	
	TIM_TimeBaseInitStruct.TIM_Prescaler  = 89;          //预分频器装填为99
	TIM_TimeBaseInitStruct.TIM_Period = 9999;              //重装值设为8999
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //时钟分割
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //PWM模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	
#if TIM8_ENABLE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);   //开启定时器八的时钟
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);
#endif
	
#if TIM9_ENABLE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);   //开启定时器八的时钟
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseInitStruct);
#endif
	
#if TIM10_ENABLE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);   //开启定时器八的时钟
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseInitStruct);
#endif
	
#if TIM11_ENABLE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);   //开启定时器八的时钟
	TIM_TimeBaseInit(TIM11, &TIM_TimeBaseInitStruct);
#endif
	
	
	
#if TIM8_CH1_ENABLE
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOI, &GPIO_InitStruct);                
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource5,  GPIO_AF_TIM8);    //连接引脚复用
	TIM_OC1Init(TIM8, &TIM_OCInitStruct);                       //将TIM1通道按结构体初始化
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);   //使能通道预装载寄存器
	TIM_ARRPreloadConfig(TIM8, ENABLE);                 //
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_Cmd(TIM8, ENABLE);                              //使能定时器
#endif
	
#if TIM8_CH2_ENABLE
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOI, &GPIO_InitStruct);                
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource6,  GPIO_AF_TIM8);    //连接引脚复用
	TIM_OC2Init(TIM8, &TIM_OCInitStruct);                       //将TIM1通道按结构体初始化
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);   //使能通道预装载寄存器
	TIM_ARRPreloadConfig(TIM8, ENABLE);                 //
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_Cmd(TIM8, ENABLE);                              //使能定时器
#endif

#if TIM8_CH3_ENABLE
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOI, &GPIO_InitStruct);                
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource7,  GPIO_AF_TIM8);    //连接引脚复用
	TIM_OC3Init(TIM8, &TIM_OCInitStruct);                       //将TIM1通道按结构体初始化
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);   //使能通道预装载寄存器
	TIM_ARRPreloadConfig(TIM8, ENABLE);                 //
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_Cmd(TIM8, ENABLE);          
#endif

#if TIM8_CH4_ENABLE
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStruct);                
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,  GPIO_AF_TIM8);    //连接引脚复用
	TIM_OC4Init(TIM8, &TIM_OCInitStruct);                       //将TIM1通道按结构体初始化
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);   //使能通道预装载寄存器
	TIM_ARRPreloadConfig(TIM8, ENABLE);                 //
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	TIM_Cmd(TIM8, ENABLE);     
#endif

#if TIM9_CH1_ENABLE
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOE, &GPIO_InitStruct);                
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource5,  GPIO_AF_TIM9);    //连接引脚复用
	TIM_OC1Init(TIM9, &TIM_OCInitStruct);                       //将TIM1通道按结构体初始化
	TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);   //使能通道预装载寄存器
	TIM_ARRPreloadConfig(TIM9, ENABLE);                 //
	TIM_Cmd(TIM9, ENABLE); 
#endif

#if TIM9_CH2_ENABLE
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStruct);                
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource6,  GPIO_AF_TIM9);    //连接引脚复用
	TIM_OC2Init(TIM9, &TIM_OCInitStruct);                       //将TIM1通道按结构体初始化
	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);   //使能通道预装载寄存器
	TIM_ARRPreloadConfig(TIM9, ENABLE);                 //
	TIM_Cmd(TIM9, ENABLE);          
#endif

#if TIM10_CH1_ENABLE
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOB, &GPIO_InitStruct);                
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8,  GPIO_AF_TIM10);    //连接引脚复用
	TIM_OC1Init(TIM10, &TIM_OCInitStruct);                       //将TIM1通道按结构体初始化
	TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);   //使能通道预装载寄存器
	TIM_ARRPreloadConfig(TIM10, ENABLE);                 //
	TIM_Cmd(TIM10, ENABLE);  
#endif

#if TIM11_CH1_ENABLE
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOB, &GPIO_InitStruct);                
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,  GPIO_AF_TIM11);    //连接引脚复用
	TIM_OC1Init(TIM11, &TIM_OCInitStruct);                       //将TIM1通道按结构体初始化
	TIM_OC1PreloadConfig(TIM11, TIM_OCPreload_Enable);   //使能通道预装载寄存器
	TIM_ARRPreloadConfig(TIM11, ENABLE);                 //
	TIM_Cmd(TIM11, ENABLE);  
#endif
	


}


void Set_PWM_Out(PWM_Type pwm_num, int pwm)
{
	switch(pwm_num) {
		case T8C1:
			TIM_SetCompare1(TIM8, pwm);
		break;
		
		case T8C2:
			TIM_SetCompare2(TIM8, pwm);
		break;
		
		case T8C3:
			TIM_SetCompare3(TIM8, pwm);
		break;
		
		case T8C4:
			TIM_SetCompare4(TIM8, pwm);
		break;
		
		case T9C1:
			TIM_SetCompare1(TIM9, pwm);
		break;
		
		case T9C2:
			TIM_SetCompare2(TIM9, pwm);
		break;
		
		case T10C1:
			TIM_SetCompare1(TIM10, pwm);
		break;
		
		case T11C1:
			TIM_SetCompare1(TIM11, pwm);
		break;
	}
	
}


void Set_Sevro_Angle(PWM_Type pwm_num, fp32 angle)
{
	int pwm;
	pwm = (int)(angle*100/9 + 1500);
	Set_PWM_Out(pwm_num, pwm);
}
