#ifndef   __PWM_H
#define   __PWM_H

#include "stm32f4xx.h"
#include "main.h"


#define   TIM8_ENABLE         1
#define   TIM9_ENABLE         1
#define   TIM10_ENABLE        1
#define   TIM11_ENABLE        1

#define   TIM8_CH1_ENABLE     1                           //PI5
#define   TIM8_CH2_ENABLE     1                           //PI6
#define   TIM8_CH3_ENABLE     1                           //PI7
#define   TIM8_CH4_ENABLE     1                           //PC9

#define   TIM9_CH1_ENABLE     1                           //PE5
#define   TIM9_CH2_ENABLE     1                           //PE6

#define   TIM10_CH1_ENABLE    1                           //PB8

#define   TIM11_CH1_ENABLE    1                           //PB9


typedef  enum
{
	T8C1,
	T8C2,
	T8C3,
	T8C4,
	T9C1,
	T9C2,
	T10C1,
	T11C1,
} PWM_Type;

void PWM_init(void);
void Set_PWM_Out(PWM_Type pwm_num, int pwm);
void Set_Sevro_Angle(PWM_Type pwm_num, fp32 angle);


#endif 
