#include "jy901.h"


//陀螺仪处理函数
static void JY901_TO_Ture(volatile const uint8_t *jy901_buf, fp32 *angle, fp32 *gyro);
static void USART5_JY901_Config(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);

//陀螺仪数据
static fp32 Angle_Z;
static fp32 Gyro_Z;
//接收原始数据，为11个字节
static uint8_t JY901_rx_buf[2][JY901_RX_BUF_NUM];

/**
  * @brief  USART5 GPIO 配置,工作模式配置。115200 9-E-1 
  * @param  无
  * @retval 无
  */


void JY901_init(void)
{
	USART5_JY901_Config(JY901_rx_buf[0], JY901_rx_buf[1], JY901_RX_BUF_NUM);
}


const fp32 *get_angle_point(void)
{
	return &Angle_Z;
}

const fp32 *get_gyro_point(void)
{
	return &Gyro_Z;
}


static void USART5_JY901_Config(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)         //RX-PD2    TX-PC12
{

	/* -------------- Enable Module Clock Source ----------------------------*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); // PC12  usart3_tx
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);  // PD2   usart3_rx
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_DeInit(UART5);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(UART5, &USART_InitStructure);

	USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);

	// USART_ClearFlag(USART3, USART_FLAG_RXNE);
	// USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ClearFlag(UART5, USART_FLAG_IDLE);
	USART_ITConfig(UART5, USART_IT_IDLE, ENABLE);

	USART_Cmd(UART5, ENABLE);

	/* ------------------ Configure USART3 NVIC -----------------------------------*/
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = JY901_NVIC;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// DMA1 stream0 ch4
	/* ----------------------- Configure DMA -----------------------------------*/
	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA1_Stream0);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (UART5->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = dma_buf_num;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream0, &DMA_InitStructure);

	DMA_DoubleBufferModeConfig(DMA1_Stream0, (uint32_t)rx2_buf, DMA_Memory_0); // 双缓冲模式
	DMA_DoubleBufferModeCmd(DMA1_Stream0, ENABLE);
	DMA_Cmd(DMA1_Stream0, ENABLE);
}



void UART5_IRQHandler(void)
{

	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
     {
         USART_ReceiveData(UART5);
     }
     else if (USART_GetITStatus(UART5, USART_IT_IDLE) != RESET)
     {
         static uint16_t this_time_rx_len = 0;
         USART_ReceiveData(UART5);

         if(DMA_GetCurrentMemoryTarget(DMA1_Stream0) == 0)
         {
             //重新设置DMA
             DMA_Cmd(DMA1_Stream0, DISABLE);
             this_time_rx_len = JY901_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream0);
             DMA_SetCurrDataCounter(DMA1_Stream0, JY901_RX_BUF_NUM);
             DMA1_Stream0->CR |= DMA_SxCR_CT;
             //清DMA中断标志
             DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF4 | DMA_FLAG_HTIF4);
             DMA_Cmd(DMA1_Stream0, ENABLE);
             if(this_time_rx_len == JY901_FRAME_LENGTH)
             {
                 //处理陀螺仪数据
                 JY901_TO_Ture(JY901_rx_buf[0], &Angle_Z, &Gyro_Z);
             }
         }
         else
         {
             //重新设置DMA
             DMA_Cmd(DMA1_Stream0, DISABLE);
             this_time_rx_len = JY901_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream0);
             DMA_SetCurrDataCounter(DMA1_Stream0, JY901_RX_BUF_NUM);
             DMA1_Stream0->CR &= ~(DMA_SxCR_CT);
             //清DMA中断标志
             DMA_ClearFlag(DMA1_Stream0, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
             DMA_Cmd(DMA1_Stream0, ENABLE);
             if(this_time_rx_len == JY901_FRAME_LENGTH)
             {
                 //处理陀螺仪数据
                 JY901_TO_Ture(JY901_rx_buf[0], &Angle_Z, &Gyro_Z);
             }
         }
     }
	
}



static void JY901_TO_Ture(volatile const uint8_t *jy901_buf, fp32 *angle, fp32 *gyro)
{
	int angle_Z_data;
	int gyro_Z_data;
	if(jy901_buf[0]!=JY901_START_BYTE)
		return;
	else {
		switch(jy901_buf[1]) {
			case 0x52:
				gyro_Z_data  = ((jy901_buf[7]<<8)|jy901_buf[6]);
			break;
			
			case 0x53:
				angle_Z_data = ((jy901_buf[7]<<8)|jy901_buf[6]);    //   anglez
			break;
			
			default:
				break;
		}
	}
//	Angle_Z = (float)(angle_Z_data)*1.0/32768*180;
	Gyro_Z  = (float)(gyro_Z_data)/32768*2000;
	if(Gyro_Z>=2000)
		Gyro_Z -= 4000;                       //度每秒
	
	Gyro_Z = Gyro_Z*ANGLE_TO_RAD;
	Angle_Z += Gyro_Z*0.01;
}


