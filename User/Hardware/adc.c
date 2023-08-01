/**
  ******************************************************************************
  * @file    bsp_bsp_adc.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   adc驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "adc.h"

double OUT[2048];
double BASE;
double BASE_sum;
__IO uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL]={0};
//double ADC1_TURE[NUM_OF_CYCLE]={0};
//double ADC2_TURE[NUM_OF_CYCLE]={0};
//double ADC1_OUT[NUM_OF_CYCLE]={0};
//double ADC2_OUT[NUM_OF_CYCLE]={0};

static void Rheostat_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/*=====================通道10======================*/	
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK1,ENABLE);		
	// 配置 IO
  GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //不上拉不下拉	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT1, &GPIO_InitStructure);

	/*=====================通道11======================*/
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK2,ENABLE);		
	// 配置 IO
  GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //不上拉不下拉	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT2, &GPIO_InitStructure);	

	/*=====================通道12=======================*/
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK3,ENABLE);		
	// 配置 IO
  GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //不上拉不下拉	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT3, &GPIO_InitStructure);
	
	/*=====================通道13=======================*/
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_GPIO_CLK4,ENABLE);		
	// 配置 IO
  GPIO_InitStructure.GPIO_Pin = RHEOSTAT_ADC_GPIO_PIN4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //不上拉不下拉	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(RHEOSTAT_ADC_GPIO_PORT4, &GPIO_InitStructure);

}

static void Rheostat_ADC_Mode_Config(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure; 

	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	// ------------------DMA Init 结构体参数 初始化--------------------------
	// ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
	// 开启DMA时钟
	RCC_AHB1PeriphClockCmd(RHEOSTAT_ADC_DMA_CLK, ENABLE); 
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = RHEOSTAT_ADC_DR_ADDR;	
	// 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;  
  // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小为，指一次传输的数据量
	DMA_InitStructure.DMA_BufferSize = RHEOSTAT_NOFCHANEL;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO 大小，FIFO模式禁止时，这个不用配置	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// 选择 DMA 通道，通道存在于流中
  DMA_InitStructure.DMA_Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
  //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(RHEOSTAT_ADC_DMA_STREAM, &DMA_InitStructure);
	

	DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);							//DMA?????????
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;				//???????€????????????
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;				//???????? 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;						//??????0 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;							//????????????
	NVIC_Init(&NVIC_InitStructure);  	  									//???NVIC_InitStruct?????????????????VIC?????
	
	
	// 使能DMA流
	DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);             //
	
	// 开启ADC时钟
	RCC_APB2PeriphClockCmd(RHEOSTAT_ADC_CLK , ENABLE);
  // -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // 时钟为fpclk x分频	
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;       //      90/4 = 22.5Mhz
  // 禁止DMA直接访问模式	
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // 采样时间间隔	 
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;    //5        为多路ADC采样时的延时
	ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init 结构体 参数 初始化--------------------------
	ADC_StructInit(&ADC_InitStructure);
  // ADC 分辨率 
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;           
  // 扫描模式，多通道采集需要	
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  // 连续转换	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  //禁止外部边沿触发
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //外部触发通道，本例子使用软件触发，此值随便赋值即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //数据右对齐	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //转换通道 1个
	ADC_InitStructure.ADC_NbrOfConversion = 2;                                    
	ADC_Init(RHEOSTAT_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // 配置 ADC 通道转换顺序和采样时间周期
	ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL1, 1, 
	                         ADC_SampleTime_3Cycles);                                //       22.5M/(3+12)
	ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL2, 2, 
													 ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL3, 3, 
	                         ADC_SampleTime_3Cycles); 
	ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL4, 4, 
	                         ADC_SampleTime_3Cycles); 
//	ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL5, 5, 
//	                         ADC_SampleTime_3Cycles); 
//	ADC_RegularChannelConfig(RHEOSTAT_ADC, RHEOSTAT_ADC_CHANNEL6, 6, 
//	                         ADC_SampleTime_3Cycles); 

  // 使能DMA请求 after last transfer (Single-ADC mode)
	ADC_DMARequestAfterLastTransferCmd(RHEOSTAT_ADC, ENABLE);
	
//	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
  // 使能ADC DMA
	ADC_DMACmd(RHEOSTAT_ADC, ENABLE);
	
	// 使能ADC
	ADC_Cmd(RHEOSTAT_ADC, ENABLE);   
  //开始adc转换，软件触发
	ADC_SoftwareStartConv(RHEOSTAT_ADC);
}



void Rheostat_Init(void)
{
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
}


float Get_Channelx_V(uint16_t x)
{
	float ADC_V;
	
	ADC_V = (float)ADC_ConvertedValue[x-10]/4096*(float)3.3;
	
	return ADC_V;
}



void DMA2_Stream0_IRQHandler(void)
{
	uint16_t i,err;
	float read;
	double max;
	char str[50];

	
	DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, DISABLE);
//	ADC_MultiModeDMARequestAfterLastTransferCmd(DISABLE);
  ADC_Cmd(RHEOSTAT_ADC, DISABLE);        
//	ADC_Cmd(ADC2, DISABLE);       
	DMA_ClearITPendingBit(RHEOSTAT_ADC_DMA_STREAM,DMA_IT_TCIF0); 
//	ADC_Cmd(ADC3, DISABLE);        
//	for (i=0; i<1024; i++)  {
//		ADC1_TURE[i] = (float)ADC_ConvertedValue[2*i]/4096*(float)3.3;
//		ADC2_TURE[i] = (float)ADC_ConvertedValue[2*i+1]/4096*(float)3.3;
//	}
//	

	ADC_Cmd(RHEOSTAT_ADC, ENABLE);                       
	DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);
	ADC_SoftwareStartConv(RHEOSTAT_ADC);
	
	
	
//	DMA_Cmd(RHEOSTAT_ADC_DMA_STREAM, ENABLE);
}


