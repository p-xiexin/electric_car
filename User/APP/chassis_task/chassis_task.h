/**
  ****************************(C) COPYRIGHT 2016 DJI****************************
  * @file       chassis.c/h
  * @brief      完成底盘控制任务。
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. 完成
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2016 DJI****************************
  */
#ifndef CHASSIS_TASK_H
#define CHASSIS_TASK_H
#include "main.h"
#include "remote_control.h"
#include "pid.h"
// #include "CAN_Receive.h"
#include "user_lib.h"
#include "pid.h"
#include "motor.h"

/*底盘电机编号
  1     2

  0     3
  
  
  0     1
  
  
  2     3
*/

//任务开始空闲一段时间
#define CHASSIS_TASK_INIT_TIME 300

//前后的遥控器通道号码
#define CHASSIS_X_CHANNEL 0
//旋转的遥控器通道号码
#define CHASSIS_WZ_CHANNEL 1

//遥控器前进摇杆（max 660）转化成车体前进速度（m/s）的比例
#define CHASSIS_VX_RC_SEN 0.006f
//遥控器的yaw遥杆（max 660）转化成车体旋转速度（rad/s）的比例
#define CHASSIS_WZ_RC_SEN 0.007f
//x方向一阶低通滤波参数
#define CHASSIS_ACCEL_X_NUM 0.1666666667f
//遥控器设置速度死区
#define CHASSIS_RC_DEADLINE 10

// #define MOTOR_DISTANCE_TO_CENTER 0.2f

//底盘任务控制间隔 2ms
#define CHASSIS_CONTROL_TIME_MS 2
//底盘任务控制间隔 0.002s
#define CHASSIS_CONTROL_TIME 0.002
//底盘任务控制频率
#define CHASSIS_CONTROL_FREQUENCE 500.0f
//底盘pwm最大值
#define MAX_MOTOR_CAN_CURRENT 6000.0f

//m3508转化成底盘速度(m/s)的比例，做两个宏 是因为可能换电机需要更换比例
// #define M3508_MOTOR_RPM_TO_VECTOR 0.000415809748903494517209f  // 2PI/60/(3591/187) * (0.1524/2)
#define M3508_MOTOR_RPM_TO_VECTOR 0.000831619497806989034418f  // 2PI/60/(20) * (0.068/2)            rpm
#define CHASSIS_MOTOR_RPM_TO_VECTOR_SEN M3508_MOTOR_RPM_TO_VECTOR

//底盘电机最大速度 m/s
#define MAX_WHEEL_SPEED 4.0f
//底盘运动过程最大前进速度 m/s
#define NORMAL_MAX_CHASSIS_SPEED_X 4.0f
//底盘设置旋转速度，设置前后左右轮不同设定速度的比例分权 0为在几何中心，不需要补偿
#define CHASSIS_WZ_SET_SCALE 0.0f


//底盘电机速度环PID
#define M3505_MOTOR_SPEED_PID_KP 48.0f
#define M3505_MOTOR_SPEED_PID_KI 1.6f
#define M3505_MOTOR_SPEED_PID_KD 0.0f
#define M3505_MOTOR_SPEED_PID_MAX_OUT MAX_MOTOR_CAN_CURRENT
#define M3505_MOTOR_SPEED_PID_MAX_IOUT 2000.0f

//底盘旋转跟随PID
#define CHASSIS_FOLLOW_GIMBAL_PID_KP 8.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_KI 0.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_KD 0.15f
#define CHASSIS_FOLLOW_GIMBAL_PID_MAX_OUT 5.0f
#define CHASSIS_FOLLOW_GIMBAL_PID_MAX_IOUT 0.2f

typedef enum
{
  CHASSIS_VECTOR_FOLLOW_GIMBAL_YAW,   //底盘跟随角度控制
  CHASSIS_VECTOR_NO_FOLLOW_YAW,       //底盘跟随速度控制
  CHASSIS_VECTOR_RAW,                 //底盘开环控制
} chassis_mode_e;

typedef struct
{
  const motor_measure_t *chassis_motor_measure;
  // const int *encoder;
  fp32 accel;
  fp32 speed;
  fp32 speed_set;
  int16_t give_current;
} Chassis_Motor_t;

typedef struct
{
  const RC_ctrl_t *chassis_RC;               //底盘使用的遥控器指针
  // const fp32 *chassis_INS_angle;             //获取陀螺仪解算出的欧拉角指针
  chassis_mode_e chassis_mode;               //底盘控制状态机
  chassis_mode_e last_chassis_mode;          //底盘上次控制状态机
  Chassis_Motor_t motor_chassis[4];          //底盘电机数据
  PidTypeDef motor_speed_pid[4];             //底盘电机速度pid
  PidTypeDef chassis_angle_pid;              //底盘跟随角度pid

  first_order_filter_type_t chassis_cmd_slow_set_vx;

  fp32 vx;                         //底盘速度 前进方向 前为正，单位 m/s
  fp32 wz;                         //底盘旋转角速度，逆时针为正 单位 rad/s
  fp32 vx_set;                     //底盘设定速度 前进方向 前为正，单位 m/s
  fp32 wz_set;                     //底盘设定旋转角速度，逆时针为正 单位 rad/s
  fp32 chassis_relative_angle;     //底盘与云台的相对角度，单位 rad/s
  fp32 chassis_relative_angle_set; //设置相对云台控制角度
  fp32 chassis_yaw_set;
  fp32 chassis_yaw;                //陀螺仪的yaw角度

  fp32 vx_max_speed;  //前进方向最大速度 单位m/s
  fp32 vx_min_speed;  //前进方向最小速度 单位m/s
} chassis_move_t;


extern void chassis_task(void *pvParameters);
extern void chassis_rc_to_control_vector(fp32 *vx_set, fp32 *vy_set, chassis_move_t *chassis_move_rc_to_vector);

//底盘初始化，主要是pid初始化
extern void chassis_init(chassis_move_t *chassis_move_init);
//底盘状态机选择，通过遥控器的开关
extern void chassis_set_mode(chassis_move_t *chassis_move_mode);
//底盘数据更新
extern void chassis_feedback_update(chassis_move_t *chassis_move_update);
//底盘状态改变后处理控制量的改变
extern void chassis_mode_change_control_transit(chassis_move_t *chassis_move_transit);
//底盘设置根据遥控器控制量
extern void chassis_set_contorl(chassis_move_t *chassis_move_control);
//底盘PID计算以及运动分解
extern void chassis_control_loop(chassis_move_t *chassis_move_control_loop);

//获取底盘结构体指针
const chassis_move_t *get_chassis_control_point(void);
#endif
