#ifndef JY901_H
#define JY901_H

#include "stm32f4xx.h"
#include "main.h"


#define  JY901_NVIC    2


#define 	JY901_START_BYTE 	0x55
#define 	JY901_RX_BUF_NUM 	22u             //11*2
#define 	JY901_FRAME_LENGTH  11u


#define  	ANGLE_TO_RAD        0.01745329251943295f

void JY901_init(void);
const fp32 *get_angle_point(void);
const fp32 *get_gyro_point(void);


#endif
