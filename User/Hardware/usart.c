/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F429 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
  
#include "usart.h"







//parameters for DMA transports
#if DATA_TEST
#define USART3_Tx_DataLen  26
#else 
#define USART3_Tx_DataLen  6

#endif

/*              parameter for usart3              */
char USART3_Tx_Buffer[USART3_Tx_DataLen]={0};



/*              parameter for JY901              */
int angle_Z_data;
int gyro_Z_data;
int gps_raw_data;


/*              parameter for usart2              */
uint8_t USART2_Tx_Buffer[9]={0x51, 0x0b, 0x00, 0x01, 0x00, 0x05, 0x02, 0x00, 0x00};


//parameters for online test
//extern float encoder_targt[4];
//extern double wheel_V_kp[4];
//extern double wheel_V_ki[4];

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  

  
  /* ??USART???? */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  /* ??????1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  /* ?????1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ???? */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ?????NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO 配置,工作模式配置。115200 9-E-1 
  * @param  无
  * @retval 无
  */
void USART3_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOC,ENABLE);

  /* 使能 USART 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10  ;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3);
  
  /* 配置串DEBUG_USART 模式 */
  /* 波特率设置：DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = 115200;
  /* 字长(数据位+校验位)：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 校验位选择：不使用校验 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(USART3, &USART_InitStructure); 
	
//	NVIC_Configuration();
	
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	
	/*  enable DMA transport  */
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	
  /* 使能串口 */
  USART_Cmd(USART3, ENABLE);
}




void DMA_USART3_init(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	DMA_DeInit(DMA1_Stream3);
	while (DMA_GetCmdStatus(DMA1_Stream3) != DISABLE);//wait for DMA
	
	/* DMA1 Stream3-Channel4 ,USART3_TX */
	DMA_InitStructure.DMA_Channel            = DMA_Channel_4;               //channel4
  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART3->DR;            //DMA Peripheral address
  DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)USART3_Tx_Buffer;       //DMA Memory address
  DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;  //Memory To Peripheral
  DMA_InitStructure.DMA_BufferSize         = USART3_Tx_DataLen;       		//data-len
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //
	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             //
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;         //
  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;      //
  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;  //
  DMA_Init(DMA1_Stream3, &DMA_InitStructure);                             //DMA Stream3



	
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	
	DMA_ITConfig(DMA1_Stream3, DMA_IT_TC, ENABLE);							//interrupt for DMA transport
	DMA_Cmd(DMA1_Stream3, DISABLE);											//disopen
	
}








/*****************  发送一个字节 **********************/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx,ch);
		
	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET);	
}

/****************** 发送8位的数组 ************************/
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
{
  uint8_t i;
	
	for(i=0; i<num; i++)
  {
	    /* 发送一个字节数据到USART */
	    Usart_SendByte(pUSARTx,array[i]);	
  
  }
	/* 等待发送完成 */
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}

/*****************  发送字符串 **********************/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* 等待发送完成 */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}

/*****************  发送一个16位数 **********************/
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;
	
	/* 取出高八位 */
	temp_h = (ch&0XFF00)>>8;
	/* 取出低八位 */
	temp_l = ch&0XFF;
	
	/* 发送高八位 */
	USART_SendData(pUSARTx,temp_h);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
	
	/* 发送低八位 */
	USART_SendData(pUSARTx,temp_l);	
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}


///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART3, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART3);
}



void Test_Send_User(int data1, int data2, int data3, int data4, int32_t data5, int32_t data6 , int data7, int data8)
{
//    char testdatatosend[50]= {0};    //??buffer
    int _cnt=0;
    unsigned char i;
    unsigned char sum = 0;
    unsigned char add = 0;

	
		if(USART3_Tx_DataLen == 26)  {
		
			USART3_Tx_Buffer[_cnt++]=0xAA;
			USART3_Tx_Buffer[_cnt++]=0xFF;    //????
			USART3_Tx_Buffer[_cnt++]=0xF1;    //???
			USART3_Tx_Buffer[_cnt++]=0x14;    //????  length*2


			USART3_Tx_Buffer[_cnt++]=BYTE0(data1);
			USART3_Tx_Buffer[_cnt++]=BYTE1(data1);

			USART3_Tx_Buffer[_cnt++]=BYTE0(data2);
			USART3_Tx_Buffer[_cnt++]=BYTE1(data2);

			USART3_Tx_Buffer[_cnt++]=BYTE0(data3);
			USART3_Tx_Buffer[_cnt++]=BYTE1(data3);

			USART3_Tx_Buffer[_cnt++]=BYTE0(data4);
			USART3_Tx_Buffer[_cnt++]=BYTE1(data4);

			USART3_Tx_Buffer[_cnt++]=BYTE0(data5);
			USART3_Tx_Buffer[_cnt++]=BYTE1(data5);
			USART3_Tx_Buffer[_cnt++]=BYTE2(data5);
			USART3_Tx_Buffer[_cnt++]=BYTE3(data5);

			USART3_Tx_Buffer[_cnt++]=BYTE0(data6);
			USART3_Tx_Buffer[_cnt++]=BYTE1(data6);
			USART3_Tx_Buffer[_cnt++]=BYTE2(data6);
			USART3_Tx_Buffer[_cnt++]=BYTE3(data6);

			USART3_Tx_Buffer[_cnt++]=BYTE0(data7);
			USART3_Tx_Buffer[_cnt++]=BYTE1(data7);

			USART3_Tx_Buffer[_cnt++]=BYTE0(data8);
			USART3_Tx_Buffer[_cnt++]=BYTE1(data8);
	
			


			for( i=0;i<_cnt;i++)
			{
					sum += USART3_Tx_Buffer[i];
					add += sum;
			}

			USART3_Tx_Buffer[_cnt++]=sum;
			USART3_Tx_Buffer[_cnt++]=add;
		}
		
		else {
			USART3_Tx_Buffer[0] = 0x55;
			USART3_Tx_Buffer[1] = 0x53;

			USART3_Tx_Buffer[2] = (data1&0xff000000)>>24;
			USART3_Tx_Buffer[3] = (data1&0x00ff0000)>>16;
			USART3_Tx_Buffer[4] = (data1&0x0000ff00)>>8;
			USART3_Tx_Buffer[5] = (data1&0x000000ff)>>0;
		}
		
		
		

//    UART_PutBuff(USART3, testdatatosend, _cnt);
////	Usart_SendArray( USART3, (unsigned char*)USART3_Tx_Buffer, USART3_Tx_DataLen);       //old ways for usart3 transport
		DMA_SetCurrDataCounter(DMA1_Stream3,USART3_Tx_DataLen);              //DMA-transpoert
		DMA_Cmd(DMA1_Stream3, ENABLE);
		
		
}


void DMA1_Stream3_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA1_Stream3,DMA_FLAG_TCIF3)!=RESET)//
	{
		DMA_ClearFlag(DMA1_Stream3,DMA_FLAG_TCIF3); //
		DMA_Cmd(DMA1_Stream3,DISABLE);
    
	}
}




//void USART3_IRQHandler(void)                                //swap for test       usart with no lines
//{
//	static unsigned int Res;
//	static uint8_t count=0;
////	static unsigned char K210_Buf[110];
//	Res = USART3->DR;	
////	K210_Buf[count++]=Res;
//	if (Res!='A')
//	{
//		count=0;
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//		return;
//	}
//	if (count<110) {
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//		return;
//	}
//	else
//	{
//		
//		count=0;
//	}
//	
//	USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	
//}














/**************************UART5 for JY901**************************/

static void NVIC_JY901_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ??USART???? */
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
  /* ??????1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* ?????1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ???? */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ?????NVIC */
  NVIC_Init(&NVIC_InitStructure);
}



 /**
  * @brief  USART5 GPIO 配置,工作模式配置。115200 9-E-1 
  * @param  无
  * @retval 无
  */
void USART5_JY901_Config(void)         //RX-PD2    TX-PC12
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD,ENABLE);

  /* 使能 USART 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12  ;  
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5);

  /*  连接 PXx 到 USARTx__Rx*/
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource2, GPIO_AF_UART5);
  
  /* 配置串DEBUG_USART 模式 */
  /* 波特率设置：DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = 115200;
  /* 字长(数据位+校验位)：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 校验位选择：不使用校验 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(UART5, &USART_InitStructure); 
	
	NVIC_JY901_Configuration();
	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	
  /* 使能串口 */
  USART_Cmd(UART5, ENABLE);
}










void UART5_IRQHandler(void)
{
	static unsigned int Res;
	static uint8_t count=0;
	static unsigned char JY901_BUF[11];
	Res = UART5->DR;	
	JY901_BUF[count++]=Res;
	if (JY901_BUF[0]!=0x55)
	{
		count=0;
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		return;
	}
	if (count<11) {
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
		return;
	}
	else
	{
		switch(JY901_BUF[1])
		{
			case 0x52:
				gyro_Z_data  = ((JY901_BUF[7]<<8)|JY901_BUF[6]);    //   gyroz
				break;
			case 0x53:
				angle_Z_data = ((JY901_BUF[7]<<8)|JY901_BUF[6]);    //   anglez
				break;
			case 0x58:
				break;
			default:
//				USART_SendData(USART3, 0xad);
				break;
		}
		count=0;
	}
	
	USART_ClearITPendingBit(UART5, USART_IT_RXNE);
}


























/*********************************************USART2 for MS53L1M**********************/
static void NVIC_USART2_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ??USART???? */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  /* ??????1 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* ?????1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ???? */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ?????NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  USART GPIO 配置,工作模式配置。115200 9-E-1 
  * @param  无
  * @retval 无
  */
void USART2_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
		
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOD,ENABLE);

  /* 使能 USART 时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  /* GPIO初始化 */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* 配置Tx引脚为复用功能  */   //PD5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5  ;  
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* 配置Rx引脚为复用功能 */    //PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
 /* 连接 PXx 到 USARTx_Rx*/
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

  /*  连接 PXx 到 USARTx_Tx*/
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
  
  /* 配置串DEBUG_USART 模式 */
  /* 波特率设置：DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = 115200;
  /* 字长(数据位+校验位)：8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* 停止位：1个停止位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* 校验位选择：不使用校验 */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* 硬件流控制：不使用硬件流 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USART模式控制：同时使能接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* 完成USART初始化配置 */
  USART_Init(USART2, &USART_InitStructure); 
	
	NVIC_USART2_Configuration();
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	/*  enable DMA transport  */
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
	
  /* 使能串口 */
  USART_Cmd(USART2, ENABLE);
}



//USART2_TX				DMA1_Stream6   DMA_Channel_4
//USART2_RX				DMA1_Stream5   DMA_Channel_4
//void DMA_USART2_init(void)
//{
//	DMA_InitTypeDef  DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
//	
//	DMA_DeInit(DMA1_Stream6);
//	while (DMA_GetCmdStatus(DMA1_Stream6) != DISABLE);//wait for DMA
//	
//	/* DMA1 Stream6-Channel4 ,USART2_TX */
//	DMA_InitStructure.DMA_Channel            = DMA_Channel_4;               //channel4
//  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;            //DMA Peripheral address
//  DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)USART2_Tx_Buffer;       //DMA Memory address
//  DMA_InitStructure.DMA_DIR                = DMA_DIR_MemoryToPeripheral;  //Memory To Peripheral
//  DMA_InitStructure.DMA_BufferSize         = 9;       										//data-len
//  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;   //
//	DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;        //
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //
//  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_Byte;     //
//  DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;             //
//  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;         //
//  DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_1QuarterFull;
//  DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;      //
//  DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;  //
//  DMA_Init(DMA1_Stream6, &DMA_InitStructure);                             //DMA Stream3



//	
//  
//  
//  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//	
//	DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);							//interrupt for DMA transport
//	DMA_Cmd(DMA1_Stream6, DISABLE);											//disopen
//	
//}







void USART2_IRQHandler(void)
{
	static unsigned int Res;
	static uint8_t count=0;
	static unsigned char MS53L1M[12];
	Res = USART2->DR;	
	MS53L1M[count++]=Res;
//	USART_SendData(USART3, Res);
	if (MS53L1M[0]!=0x55)
	{
		count=0;
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		return;
	}
	if (count<12) {
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		return;
	}
	else
	{
		switch(MS53L1M[3])
		{
			case 0x01:
				
				break;
			case 0x02:
				
				break;
			case 0x03:
				
				break;
			case 0x04:
				
				break;
			case 0x05:
				
				break;
			case 0x06:
				
				break;
			default:
				break;
		}
//		Usart_SendArray( USART3, MS53L1M, 12);
		count=0;
	}
	
	USART_ClearITPendingBit(USART2, USART_IT_RXNE);
}


