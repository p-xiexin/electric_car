#ifndef __LCD_KEY_LED_H
#define __LCD_KEY_LED_H

#include "stm32f4xx.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"



/*********************LCD_Pin************************/
#define       CS_Pin          GPIO_Pin_11
#define       DC_Pin          GPIO_Pin_1
#define       RES_Pin         GPIO_Pin_2
#define       DATA_Pin        GPIO_Pin_4
#define       SCK_Pin         GPIO_Pin_5


#define       CS_H            (GPIOB->BSRR = CS_Pin)              							//PB11
#define       CS_L            (GPIOB->BSRR = (uint32_t)CS_Pin << 16)

#define       DC_H            (GPIOI->BSRR = DC_Pin)              							//PI1
#define       DC_L            (GPIOI->BSRR = (uint32_t)DC_Pin << 16)

#define       RES_H           (GPIOA->BSRR = RES_Pin)              							//PA2
#define       RES_L           (GPIOA->BSRR = (uint32_t)RES_Pin << 16)

#define       DATA_H          (GPIOC->BSRR = DATA_Pin)              	  				//PC4
#define       DATA_L          (GPIOC->BSRR = (uint32_t)DATA_Pin << 16)

#define       SCK_H           (GPIOC->BSRR = SCK_Pin)              							//PC5
#define       SCK_L           (GPIOC->BSRR = (uint32_t)SCK_Pin << 16)



typedef struct 
{
	unsigned char key1;
	unsigned char key2;
	unsigned char key3;
	unsigned char key4;
}key_mode;

extern key_mode KEY_POSITION;


#define USE_HORIZONTAL 3  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif









/**********************LED**************************/
void led_init(void);

void led_red_on(void);
void led_red_off(void);
void led_red_toggle(void);

void led_green_on(void);
void led_green_off(void);
void led_green_toggle(void);

void led_blue_on(void);
void led_blue_off(void);
void led_blue_toggle(void);



/*********************BUZZER************************/
void buzzer_init(void);

void buzzer_on(void);
void buzzer_off(void);
void buzzer_toggle(void);


/**********************KEY**************************/
void Key_init(void);


/**********************LCD**************************/
void LCD_Init(void);

void LCD_Write_DATA8(uint8_t data8);
void LCD_Write_DATA16(uint16_t data16);
void LCD_Write_Register(uint8_t data);
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);

#endif
