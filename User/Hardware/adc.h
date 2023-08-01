#ifndef ADC_H
#define	ADC_H

#include "stm32f4xx.h" 
#include "main.h"
#include "pwm.h"

/*
ADC:
		PC0:     ADC123_IN10         PC1:     ADC123_IN11
		PC2:     ADC123_IN12         PC3:     ADC123_IN13
*/

#define RHEOSTAT_NOFCHANEL      2
#define NUM_OF_CYCLE            1024

/*=====================通道10 IO======================*/
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT1    GPIOC
#define RHEOSTAT_ADC_GPIO_PIN1     GPIO_Pin_0
#define RHEOSTAT_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL1      ADC_Channel_10
/*=====================通道11 IO ======================*/
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT2    GPIOC
#define RHEOSTAT_ADC_GPIO_PIN2     GPIO_Pin_1
#define RHEOSTAT_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL2      ADC_Channel_11
/*=====================通道12 IO ======================*/
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT3    GPIOC
#define RHEOSTAT_ADC_GPIO_PIN3     GPIO_Pin_2
#define RHEOSTAT_ADC_GPIO_CLK3     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL3      ADC_Channel_12
/*=====================通道13 IO ======================*/
// ADC IO宏定义
#define RHEOSTAT_ADC_GPIO_PORT4    GPIOC
#define RHEOSTAT_ADC_GPIO_PIN4     GPIO_Pin_3
#define RHEOSTAT_ADC_GPIO_CLK4     RCC_AHB1Periph_GPIOC
#define RHEOSTAT_ADC_CHANNEL4      ADC_Channel_13


// ADC 序号宏定义
#define RHEOSTAT_ADC              ADC1
#define RHEOSTAT_ADC_CLK          RCC_APB2Periph_ADC1
// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define RHEOSTAT_ADC_DR_ADDR      ((u32)ADC1+0x4c)


// ADC DMA 通道宏定义，这里我们使用DMA传输
#define RHEOSTAT_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define RHEOSTAT_ADC_DMA_CHANNEL  DMA_Channel_0
#define RHEOSTAT_ADC_DMA_STREAM   DMA2_Stream0



void Rheostat_Init(void);
float Get_Channelx_V(uint16_t x);      /* x: 10, 11, 12, 13, 14, 15*/

#endif /* ADC_H */



