#ifndef __USART_H
#define	__USART_H

#include "stm32f4xx.h"
#include <stdio.h>

#define DATA_TEST  1

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

/************************************************************/

void USART3_Config(void);
void USART5_JY901_Config(void);         //RX-PD2    TX-PC12;
void JY901_test(void);
void DMA_USART3_init(void);
void USART2_Config(void);
void DMA_USART2_init(void);


void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);


void Test_Send_User(int data1, int data2, int data3, int data4, int32_t data5, int32_t data6 , int data7, int data8);
void Modbus_Send(char ADDRESS);


#endif /* __USART_H */
