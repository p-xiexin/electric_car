/**
  ****************************RM Warrior 2023****************************
  * @file       start_task.c/h
  * @brief      一个普通的心跳程序，如果程序没有问题，蓝灯以1Hz的频率闪烁
  *             同时也用来发送相关数值到上位机调参。
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2023/2/         pxx              ......
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************RM Warrior 2023****************************
  */

#include "User_Task.h"
#include "main.h"
#include <stdio.h>

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"

#include "usart.h"
#include "buzzer.h"
#include "jy901.h"

#include "chassis_task.h"

extern int encoder[4];
extern int angle_Z_data;
extern int gyro_Z_data;

const chassis_move_t* local_chassis_move;
const fp32 * angle_Z;
const fp32 * gyro_Z;

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t UserTaskStack;
#endif

void UserTask(void *pvParameters)
{
    static uint8_t Tcount = 0;
	
	
    angle_Z = get_angle_point();
    gyro_Z = get_gyro_point();
	local_chassis_move = get_chassis_control_point();
	
    while (1)
    {
        Tcount++;
        if(Tcount >= 50)
        {
			led_green_toggle();	
			Tcount = 0;
        }

		// Test_Send_User(encoder[0], encoder[1], encoder[2], encoder[3], (int)(Angle_Z*10),(int)(Gyro_Z*10), local_chassis_move->motor_chassis[0].speed*1000,local_chassis_move->motor_chassis[1].speed*1000);
		// printf("%d  %d\n",local_chassis_move->chassis_RC->ch[0],local_chassis_move->chassis_RC->ch[1]);
		// printf("%f %f %f\n",local_chassis_move->wz_set,*gyro_Z, *angle_Z);

//        printf("%.2f, %.2f, %.2f, %.2f\n", local_chassis_move->chassis_yaw * 57.3f, local_chassis_move->chassis_yaw_set * 57.3f, local_chassis_move->wz * 10, local_chassis_move->wz_set * 10);
		printf("%.2f, %.2f\n", local_chassis_move->chassis_yaw * 57.3f, local_chassis_move->chassis_yaw_set * 57.3f);
		
        vTaskDelay(10);     //ms
#if INCLUDE_uxTaskGetStackHighWaterMark
        UserTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}


/**
  * @brief          蜂鸣器报警
  * @param[in]      num:响声次数
  * @retval         none
  */
void buzzer_warn(uint8_t num)
{
   static uint8_t show_num = 0;
   static uint8_t stop_num = 100;
   if(show_num == 0 && stop_num == 0)
   {
       show_num = num;
       stop_num = 100;
   }
   else if(show_num == 0)
   {
       stop_num--;
       buzzer_off();
   }
   else
   {
       static uint8_t tick = 0;
       tick++;
       if(tick < 50)
       {
           buzzer_off();
       }
       else if(tick < 100)
       {
           buzzer_on();
       }
       else
       {
           tick = 0;
           show_num--;
       }
   }
}





