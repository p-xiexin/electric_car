#include "encoder.h"

//       motor.h
extern int encoder[4];
static motor_measure_t motor_chassis[4];

/*******************************************************************************
* Function Name :Encoder1_Init_TIM4
* Description   :初始化编码器1使用的定时器4
* GPIO_PIN      :PB6  PB7
* Input         :None
* Return        :None
*******************************************************************************/
static void Encoder1_Init_TIM4(void)
{
	//NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef        TIM_ICInitStructure;  
	GPIO_InitTypeDef         GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器5的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能PB端口时钟
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置 PB6 PB7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//浮空输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//根据设定参数初始化GPIOB
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = 65535; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//边沿计数模式 
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);  //初始化定时器
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3

	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(TIM4, &TIM_ICInitStructure);//根据 TIM_ICInitStruct 的参数初始化外设	TIMx
 
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);//使能定时器中断
	TIM_SetCounter(TIM4,0);

	TIM_Cmd(TIM4, ENABLE); //使能定时器
}

/*******************************************************************************
* Function Name :Encoder2_Init_TIM3
* Description   :初始化编码器2使用的定时器3
* GPIO_PIN      :PC6    PA7
* Input         :None
* Return        :None
*******************************************************************************/
static void Encoder2_Init_TIM3(void)
{
	//NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PA端口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能PC端口时钟

	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	//端口配置PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//浮空输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//根据设定参数初始化GPIOC
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	//端口配置PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//浮空输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化GPIOA
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = 65535; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//边沿计数模式 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  //初始化定时器
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 0;  //设置滤波器长度
	TIM_ICInit(TIM3, &TIM_ICInitStructure);//根据 TIM_ICInitStruct 的参数初始化外设	TIMx
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能定时器中断
  TIM_SetCounter(TIM3,0);
	TIM_Cmd(TIM3, ENABLE); //使能定时器
}



/*******************************************************************************
* Function Name :Encoder3_Init_TIM5
* Description   :初始化编码器1使用的定时器5
* GPIO_PIN      :PH10  PH11
* Input         :None
* Return        :None
*******************************************************************************/
static void Encoder3_Init_TIM5(void)
{
	//NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef        TIM_ICInitStructure;  
	GPIO_InitTypeDef         GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);//使能定时器5的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);//使能PH端口时钟
	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;	//端口配置 PH10 PH11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//浮空输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOH, &GPIO_InitStructure);					//根据设定参数初始化GPIOH
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource10, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOH, GPIO_PinSource11, GPIO_AF_TIM5);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = 65535; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//边沿计数模式 
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);  //初始化定时器
	
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3

	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 10;  //设置滤波器长度
	TIM_ICInit(TIM5, &TIM_ICInitStructure);//根据 TIM_ICInitStruct 的参数初始化外设	TIMx
 
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);//使能定时器中断
	TIM_SetCounter(TIM5,0);

	TIM_Cmd(TIM5, ENABLE); //使能定时器
}


/*******************************************************************************
* Function Name :Encoder4_Init_TIM2
* Description   :初始化编码器4使用的定时器2
* GPIO_PIN      :PA15    PA1
* Input         :None
* Return        :None
*******************************************************************************/
static void Encoder4_Init_TIM2(void)
{
	//NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能定时器3的时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能PA端口时钟

	
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_15;	//端口配置PC6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	//浮空输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//根据设定参数初始化GPIOC
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = 65535; //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//边沿计数模式 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);  //初始化定时器
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	
	TIM_ICStructInit(&TIM_ICInitStructure); //把TIM_ICInitStruct 中的每一个参数按缺省值填入
	TIM_ICInitStructure.TIM_ICFilter = 0;  //设置滤波器长度
	TIM_ICInit(TIM2, &TIM_ICInitStructure);//根据 TIM_ICInitStruct 的参数初始化外设	TIMx
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//使能定时器中断
  TIM_SetCounter(TIM2,0);
	TIM_Cmd(TIM2, ENABLE); //使能定时器
}




/*******************************************************************************
* Function Name :Encoder_All_Init
* Description   :初始化所有编码器
* Input         :None
* Return        :None
*******************************************************************************/
void Encoder_All_Init(void)
{
	Encoder1_Init_TIM4();
	Encoder2_Init_TIM3();
	Encoder3_Init_TIM5();
	Encoder4_Init_TIM2();
}


/*******************************************************************************
* Function Name :Read_Encoder
* Description   :读取编码器数值
* Input         :编码器序号 Encoder_TypeDef Encoder1-2
* Return        :读数 int
*******************************************************************************/
extern int Read_Encoder(Encoder_TypeDef Encoder_num)
{
	int encoder_value;
	switch (Encoder_num)
	{
		case Encoder1: 
			encoder_value = 65535 - (TIM4 -> CNT);
			TIM_SetCounter(TIM4, 65535);
			break;
		case Encoder2: 
			encoder_value = (TIM3 -> CNT);
			TIM_SetCounter(TIM3, 65535);
			break;
		case Encoder3: 
			encoder_value = (TIM5 -> CNT);
			TIM_SetCounter(TIM5, 65535);
			break;
		case Encoder4: 
			encoder_value = 65535 - (TIM2 -> CNT);
			TIM_SetCounter(TIM2, 65535);
			break;
		default:
			encoder_value = 0;
	}
	if (encoder_value <= 65535/2)
		return encoder_value;
	else
		return -(65535-encoder_value);
}

void Read_All_Encoder(void)
{
	int i;
	encoder[0] = Read_Encoder(Encoder1);
	encoder[1] = Read_Encoder(Encoder2);
	encoder[2] = Read_Encoder(Encoder3);
	encoder[3] = Read_Encoder(Encoder4);
	motor_chassis[0].speed_rpm = GMR_ENCODER_TO_MOTOR_RPM*encoder[2];
	motor_chassis[1].speed_rpm = GMR_ENCODER_TO_MOTOR_RPM*encoder[0];
	motor_chassis[2].speed_rpm = GMR_ENCODER_TO_MOTOR_RPM*encoder[1];
	motor_chassis[3].speed_rpm = GMR_ENCODER_TO_MOTOR_RPM*encoder[3];
}

//返回底盘电机变量地址，通过指针方式获取原始数据
const motor_measure_t *get_Chassis_Motor_Measure_Point(uint8_t i)
{
    return &motor_chassis[(i & 0x03)];
}

