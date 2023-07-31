/**
  ****************************RM Warrior 2023****************************
  * @file       motor_task.c/h
  * @brief      对底盘前进速度和旋转速度控制
	*             电机速度PI环：500HZ
	*             
	*             
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

#include "motor_task.h"
#include "main.h"
#include "stdio.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "motor.h"
#include "encoder.h"

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t MotorTaskStack;
#endif

void MotorTask(void *pvParameters)
{
		int X_Vice_target = 10;
		int R_Vice_target = 10;
    while (1)
    {
        Set_Car_Speed(X_Vice_target,R_Vice_target);
		
        vTaskDelay(2);     //2ms
#if INCLUDE_uxTaskGetStackHighWaterMark
        MotorTaskStack = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

