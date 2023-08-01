#include "motor.h"



int motor_stop_flag=1;
int encoder[4];



/*******************************************************************************
* Function Name :Motor_Init_TIM1
* Description   :初始化IR2104PWM接口定时器TIM1 10kHz
* GPIO_PIN      :PA8 PA9 PA10 PA11
* Input         :None
* Return        :None
*******************************************************************************/
static void Motor_Init_TIM1(void)        
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;                             
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   //使能定时器1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA的时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;          //复用推挽输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;//PA8 PA9 PA10 PA11
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;          //复用推挽输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//PA8 PA9 PA10 PA11
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	
/*******************************************************************************
*  Time_BASE  =  ((1+TIM_Prescaler)/180M)*(1+TIM_Period)
*  Time       =  ((1+1)/180M)*(1+8999)  =  0.1ms
*******************************************************************************/
	
	TIM_TimeBaseInitStruct.TIM_Prescaler  = 1;          //设定预分频器
	TIM_TimeBaseInitStruct.TIM_Period = 8999;              //设定计数器自动重装值 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //设置时钟分割
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);       //初始化定时器
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);                       //初始化输出比较参数
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);   //CH1使能预装载寄存器
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);                //使能TIM4在ARR上的预装载寄存器
	TIM_Cmd(TIM1, ENABLE);                              //使能定时器4
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}





/*******************************************************************************
* Function Name :Motor_Init_TIM8
* Description   :初始化IR2104PWM接口定时器TIM8 10kHz
* GPIO_PIN      :PI5 PI6 PI7 PC9
* Input         :None
* Return        :None
*******************************************************************************/
static void Motor_Init_TIM8(void)        
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;                             
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);   //使能定时器1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);  //使能GPIOA的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //使能GPIOA的时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;          //复用推挽输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;//PI5 PI6 PI7 
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOI, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOI, GPIO_PinSource7, GPIO_AF_TIM8);
	
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;          //复用推挽输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;          //复用推挽输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;//PA8 PA9 PA10 PA11
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOI, &GPIO_InitStruct);
	
	
	
/*******************************************************************************
*  Time_BASE  =  ((1+TIM_Prescaler)/180M)*(1+TIM_Period)
*  Time       =  ((1+1)/180M)*(1+8999)  =  0.1ms
*******************************************************************************/
	
	TIM_TimeBaseInitStruct.TIM_Prescaler  = 1;          //设定预分频器
	TIM_TimeBaseInitStruct.TIM_Period = 8999;              //设定计数器自动重装值 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //设置时钟分割
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStruct);       //初始化定时器
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC1Init(TIM8, &TIM_OCInitStruct);                       //初始化输出比较参数
	TIM_OC2Init(TIM8, &TIM_OCInitStruct);
	TIM_OC3Init(TIM8, &TIM_OCInitStruct);
	TIM_OC4Init(TIM8, &TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);   //CH1使能预装载寄存器
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM8, ENABLE);                //使能TIM4在ARR上的预装载寄存器
	TIM_Cmd(TIM8, ENABLE);                              //使能定时器4
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
}


static void motor_init_new_way(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;                             
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   //使能定时器1时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA的时钟
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;          //复用推挽输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;//PA8 PA9 PA10 PA11
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
/*******************************************************************************
*  Time_BASE  =  ((1+TIM_Prescaler)/180M)*(1+TIM_Period)
*  Time       =  ((1+1)/180M)*(1+8999)  =  0.1ms
*******************************************************************************/
	
	TIM_TimeBaseInitStruct.TIM_Prescaler  = 1;          //设定预分频器
	TIM_TimeBaseInitStruct.TIM_Period = 8999;              //设定计数器自动重装值 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;         //设置时钟分割
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);       //初始化定时器
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;             //选择PWM1模式
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;     //设置输出极性
	TIM_OC1Init(TIM1, &TIM_OCInitStruct);                       //初始化输出比较参数
	TIM_OC2Init(TIM1, &TIM_OCInitStruct);
	TIM_OC3Init(TIM1, &TIM_OCInitStruct);
	TIM_OC4Init(TIM1, &TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);   //CH1使能预装载寄存器
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM1, ENABLE);                //使能TIM4在ARR上的预装载寄存器
	TIM_Cmd(TIM1, ENABLE);                              //使能定时器4
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}




/*******************************************************************************
* Function Name :Motor_Init
* Description   :初始化DRV8833
* Input         :None
* Return        :None
*******************************************************************************/
void Motor_Init(void)
{
//	Motor_Init_TIM1();
//	Motor_Init_TIM8();
	motor_init_new_way();
}







int pwm_max = 6000;
int motor1_deadhigh = 880 - 100;
int motor1_deadlow = -900 + 100;
int motor2_deadhigh = 900 - 100;
int motor2_deadlow = -880 + 100;
int motor3_deadhigh = 790 - 100;
int motor3_deadlow = -930 + 100;
int motor4_deadhigh = 640 - 100;
int motor4_deadlow = -740 + 100;
//int motor1_deadhigh = 800;
//int motor1_deadlow = -800;
//int motor2_deadhigh = 800;
//int motor2_deadlow = -800;
//int motor3_deadhigh = 800;
//int motor3_deadlow = -800;
//int motor4_deadhigh = 800;
//int motor4_deadlow = -800;


/*******************************************************************************
* Function Name :Set_PWM_Cycle
* Description   :Set the 
* Input         :None
* Return        :None
*******************************************************************************/
void Set_motor_speed(int motor1_pwm, int motor2_pwm, int motor3_pwm, int motor4_pwm)
{
	if(motor1_pwm>6000)
		motor1_pwm=6000;
	if(motor2_pwm>6000)
		motor2_pwm=6000;
	if(motor3_pwm>6000)
		motor3_pwm=6000;
	if(motor4_pwm>6000)
		motor4_pwm=6000;
	
	if(motor1_pwm<-6000)
		motor1_pwm=-6000;
	if(motor2_pwm<-6000)
		motor2_pwm=-6000;
	if(motor3_pwm<-6000)
		motor3_pwm=-6000;
	if(motor4_pwm<-6000)
		motor4_pwm=-6000;
	
	if(motor1_pwm>0&&motor1_pwm<motor1_deadhigh)
		motor1_pwm+=motor1_deadhigh;
	if(motor2_pwm>0&&motor2_pwm<motor2_deadhigh)
		motor2_pwm+=motor2_deadhigh;
	if(motor3_pwm>0&&motor3_pwm<motor3_deadhigh)
		motor3_pwm+=motor3_deadhigh;
	if(motor4_pwm>0&&motor4_pwm<motor4_deadhigh)
		motor4_pwm+=motor4_deadhigh;
	
	if(motor1_pwm<0&&motor1_pwm>motor1_deadlow)
		motor1_pwm+=motor1_deadlow;
	if(motor2_pwm<0&&motor2_pwm>motor2_deadlow)
		motor2_pwm+=motor2_deadlow;
	if(motor3_pwm<0&&motor3_pwm>motor3_deadlow)
		motor3_pwm+=motor3_deadlow;
	if(motor4_pwm<0&&motor4_pwm>motor4_deadlow)
		motor4_pwm+=motor4_deadlow;
	
	
//	if(motor1_pwm > 0)//M1
//	{
//		GPIO_SetBits(GPIOA, GPIO_Pin_10);
//		TIM_SetCompare2(TIM1, (motor1_pwm));
//	}
//	else
//	{
//		GPIO_ResetBits(GPIOA, GPIO_Pin_10);
//		TIM_SetCompare2(TIM1, -(motor1_pwm));
//	}
//	
//	
//	
//	
//	if(motor2_pwm > 0)//M2
//	{
//		GPIO_ResetBits(GPIOI, GPIO_Pin_6);
//		TIM_SetCompare1(TIM1, motor2_pwm);
//	}
//	else
//	{
//		GPIO_SetBits(GPIOI, GPIO_Pin_6);
//		TIM_SetCompare1(TIM1, -(motor2_pwm));
//	}
//	
//	
//	
//	if(motor3_pwm > 0)//M3
//	{
//		GPIO_SetBits(GPIOA, GPIO_Pin_11);
//		TIM_SetCompare4(TIM8, motor3_pwm);
//	
//	}
//	else
//	{
//		GPIO_ResetBits(GPIOA, GPIO_Pin_11);
//		TIM_SetCompare4(TIM8, -motor3_pwm);
//	}
//	
//	
//	
//	
//	if(motor4_pwm > 0)//M4
//	{
//		GPIO_ResetBits(GPIOI, GPIO_Pin_5);
//		TIM_SetCompare3(TIM8, motor4_pwm);
//	}
//	else
//	{
//		GPIO_SetBits(GPIOI, GPIO_Pin_5);
//		TIM_SetCompare3(TIM8, -(motor4_pwm));
//	}
	
	
	if(motor1_pwm > 0)//M1
	{
		GPIO_SetBits(GPIOG, GPIO_Pin_11);
		TIM_SetCompare2(TIM1, (motor1_pwm));
	}
	else
	{
		GPIO_ResetBits(GPIOG, GPIO_Pin_11);
		TIM_SetCompare2(TIM1, -(motor1_pwm));
	}
	
	
	
	
	if(motor2_pwm > 0)//M2
	{
		GPIO_ResetBits(GPIOG, GPIO_Pin_2);
		TIM_SetCompare4(TIM1, motor2_pwm);
	}
	else
	{
		GPIO_SetBits(GPIOG, GPIO_Pin_2);
		TIM_SetCompare4(TIM1, -(motor2_pwm));
	}
	
	
	
	if(motor3_pwm > 0)//M3
	{
		GPIO_SetBits(GPIOG, GPIO_Pin_12);
		TIM_SetCompare3(TIM1, motor3_pwm);
	
	}
	else
	{
		GPIO_ResetBits(GPIOG, GPIO_Pin_12);
		TIM_SetCompare3(TIM1, -motor3_pwm);
	}
	
	
	
	
	if(motor4_pwm > 0)//M4
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		TIM_SetCompare1(TIM1, motor4_pwm);
	}
	else
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
		TIM_SetCompare1(TIM1, -(motor4_pwm));
	}
	
}



//***************************************


void Set_Motor_PI(int targt1, int targt2, int targt3, int targt4)
{

}





void Set_Car_Speed(int x_vice, int r_vice)
{
	int motor1, motor2, motor3, motor4;
	int motor_l, motor_r;
//	//mai ke na mu 
//	motor1 = x_pwm+y_pwm+r_pwm;
//	motor2 = x_pwm-y_pwm-r_pwm;
//	motor3 = x_pwm-y_pwm+r_pwm;
//	motor4 = x_pwm+y_pwm-r_pwm;
	
	//si lun cha su
	//https://blog.csdn.net/qq_52785580/article/details/125995617
	motor_l = x_vice - (DISTANCE_BETWEEN_TWO_WHEEL/2)*r_vice;
	motor_r = x_vice + (DISTANCE_BETWEEN_TWO_WHEEL/2)*r_vice;
	
	motor1 = motor_l;
	motor3 = motor_l;
	motor2 = motor_r;
	motor4 = motor_r;
	

	
	Read_All_Encoder();
	if(motor_stop_flag)
		Set_Motor_PI(motor1, motor2, motor3, motor4);
	else
		Set_motor_speed(0,0,0,0);
}

//返回底盘电机变量地址，通过指针方式获取原始数据
const int *get_Chassis_Motor_Encoder_Point(uint8_t i)
{
    return &encoder[(i & 0x03)];
}


