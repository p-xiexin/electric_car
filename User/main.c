#include "main.h"
#include "stm32f4xx.h"                  // Device header

#include "lcd_key_led.h"
#include "buzzer.h"
#include "delay.h"
#include "encoder.h"
#include "usart.h"
#include "motor.h"
#include "jy901.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#include "start_task.h"
#include "remote_control.h"
#include "user_ui.h"

void BSP_Init(void);
void User_LCD_Start(void);

int main(void)
{
	delay_init(configTICK_RATE_HZ);
	User_LCD_Start();
	delay_ms(2000);
	BSP_Init();
//	
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
	
	
	//encoder_init
	Encoder_All_Init();
	
	//USART2
	USART2_Config();
	//JY901
	JY901_init();
	
	
	//RC
	remote_control_init();
	
	
	
}


void User_LCD_Start(void)
{
	LCD_Init();
	buzzer_init();
	Key_init();
	led_init();
	led_blue_on();
//	led_green_on();
	//motor_init
	Motor_Init();
	Para_Task_Start();
	
}

