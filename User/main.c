#include "main.h"
#include "stm32f4xx.h"                  // Device header

#include "lcd_key_led.h"
#include "buzzer.h"
#include "delay.h"
#include "encoder.h"
#include "usart.h"
#include "motor.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "start_task.h"
#include "remote_control.h"
#include "user_ui.h"

void BSP_Init(void);

int main(void)
{
	BSP_Init();
//	Para_Task_Start();
//	
	
	startTast();
	vTaskStartScheduler();
	
	while(1)
	{
		
	}
}

void BSP_Init(void)
{
	//中断组4
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	//初始化滴答时钟
	delay_init(configTICK_RATE_HZ);
	//motor_init
	Motor_Init();
	//encoder_init
	Encoder_All_Init();
	
  //LED灯初始化
	led_init();
	//LCD
	LCD_Init();
	//蜂鸣器初始化
	buzzer_init();
	//KEY
	Key_init();
	
	//JY901
	USART5_JY901_Config();
	
	remote_control_init();
	//DMA
//	DMA_USART3_init();
//	//uart init
//	USART3_Config();
	//RC
	
	
}

