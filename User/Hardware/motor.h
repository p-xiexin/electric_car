#ifndef  __MOTOR_H
#define  __MOTOR_H


#include "stm32f4xx.h"
#include "encoder.h"


#define  DISTANCE_BETWEEN_TWO_WHEEL  0.168                 

/*  DRV8833_M1    PB6  PB7
    DRV8833_M2    PB8  PB9     */


//电机统一数据结构体
typedef struct
{
    uint16_t ecd;           //转子机械角度
    int16_t speed_rpm;      //转子转速
    int16_t given_current;  //实际转矩电流(此处为PWM)
    uint8_t temperate;      //电机温度
    int16_t last_ecd;       //上次转子机械角度
} motor_measure_t;



void Motor_Init(void);

void Set_motor_speed(int motor1_pwm, int motor2_pwm, int motor3_pwm, int motor4_pwm);
void Set_Motor_PI(int targt1, int targt2, int targt3, int targt4);
void Set_Car_Speed(int x_vice, int r_vice);


extern const int *get_Chassis_Motor_Encoder_Point(uint8_t i);
extern const motor_measure_t *get_Chassis_Motor_Measure_Point(uint8_t i);


#endif  /*__MOTOR_H */
