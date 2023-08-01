#include "lcd_key_led.h"
key_mode KEY_POSITION={0,0,1,0};
/**********************************LED*****************************************/
//	led_R           PB3            high_level_light    
//	led_G           PA14           high_level_light 
//	led_B           PA13           high_level_light 

void led_init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOA, ENABLE);        //A

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                   //led_R
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //led_G
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;             
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //led_B             A13
	
	led_red_off();
	led_green_off();
	led_blue_off();

//	GPIO_InitTypeDef GPIO_InitStruct;                             
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //ʹĜGPIOAµĊ±֓
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  //ʹĜGPIOAµĊ±֓
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI, ENABLE);  //ʹĜGPIOAµĊ±֓
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);  //ʹĜGPIOAµĊ±֓
//	
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;          //¸´Ӄ͆ͬʤ³ö
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;//PA8 PA9 PA10 PA11
//	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStruct);
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
//	GPIO_Init(GPIOD, &GPIO_InitStruct);
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
//	GPIO_Init(GPIOI, &GPIO_InitStruct);
//	
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
//	GPIO_Init(GPIOG, &GPIO_InitStruct);
//	led_green_off();
	
}


void led_red_on(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_3);
}

void led_red_off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_3);
}

void led_red_toggle(void)
{
	GPIO_ToggleBits(GPIOB, GPIO_Pin_3);
}

void led_green_on(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_14);
//	GPIO_ResetBits(GPIOD, GPIO_Pin_11);
}

void led_green_off(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_14);
//	GPIO_SetBits(GPIOD, GPIO_Pin_11);
}

void led_green_toggle(void)
{
	GPIO_ToggleBits(GPIOA, GPIO_Pin_14);
//	GPIO_ToggleBits(GPIOD, GPIO_Pin_11);
}

void led_blue_on(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_13);
//	GPIO_ResetBits(GPIOI, GPIO_Pin_3);
}

void led_blue_off(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_13);
//	GPIO_SetBits(GPIOI, GPIO_Pin_3);
}

void led_blue_toggle(void)
{
	GPIO_ToggleBits(GPIOA, GPIO_Pin_13);
//	GPIO_ToggleBits(GPIOI, GPIO_Pin_3);
}
/**********************************LED*****************************************/






/***********************************BUZZER*************************************/
//buzzer           PB4            high_level_beep    

void buzzer_init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                   //buzzer
	buzzer_off();
}


void buzzer_on(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_4);
}

void buzzer_off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
}

void buzzer_toggle(void)
{
	GPIO_ToggleBits(GPIOB, GPIO_Pin_4);
}
/***********************************BUZZER*************************************/




/*************************************KEY**************************************/
//	Key1           PI3            falling_interrupter    
//	Key2           PB12           falling_interrupter  
//	Key3           PD11           falling_interrupter  
//	Key4           PB2            falling_interrupter  
void Key_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOI, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin    =  GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode   =  GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed  =  GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd  =  GPIO_PuPd_UP ;
	GPIO_Init(GPIOI, &GPIO_InitStruct);														//KEY1-PI3
	
	GPIO_InitStruct.GPIO_Pin    =  GPIO_Pin_12;
	GPIO_Init(GPIOB, &GPIO_InitStruct);														//KEY2-PB12
	
	GPIO_InitStruct.GPIO_Pin    =  GPIO_Pin_11;
	GPIO_Init(GPIOD, &GPIO_InitStruct);														//KEY3-PD11
	
	GPIO_InitStruct.GPIO_Pin    =  GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStruct);														//KEY4-PB2
	
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource3);
//	EXTI_InitStruct.EXTI_Line     =   EXTI_Line3;
//	EXTI_InitStruct.EXTI_Mode     =   EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger  =   EXTI_Trigger_Falling;
//	EXTI_InitStruct.EXTI_LineCmd  =   ENABLE;
//	EXTI_Init( &EXTI_InitStruct );									   					 	//KEY1-PI3
//	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);
//	EXTI_InitStruct.EXTI_Line     =   EXTI_Line12;
//	EXTI_Init( &EXTI_InitStruct );									    					//KEY2-PB12
//	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource11);
//	EXTI_InitStruct.EXTI_Line     =   EXTI_Line11;
//	EXTI_Init( &EXTI_InitStruct );									    					//KEY3-PD11
//	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource2);
//	EXTI_InitStruct.EXTI_Line     =   EXTI_Line2;
//	EXTI_Init( &EXTI_InitStruct );									    					//KEY4-PB2
//	
//	
//	
//	NVIC_InitStruct.NVIC_IRQChannel                     =   EXTI3_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority   =   2;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority          =   2;
//	NVIC_InitStruct.NVIC_IRQChannelCmd                  =   ENABLE;
//	NVIC_Init( &NVIC_InitStruct );									    					//KEY1-PI3
//	
//	NVIC_InitStruct.NVIC_IRQChannel                     =   EXTI15_10_IRQn;
//	NVIC_Init( &NVIC_InitStruct );									    					//KEY2-PB12  KEY3-PD11
//	
//	NVIC_InitStruct.NVIC_IRQChannel                     =   EXTI2_IRQn;
//	NVIC_Init( &NVIC_InitStruct );									    					//KEY4-PB2
	
	
}




void EXTI2_IRQHandler(void)
{
	
	if( EXTI_GetITStatus(EXTI_Line2) != RESET && (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) == 0) )        {//KEY4
		EXTI_ClearITPendingBit(EXTI_Line2);
		KEY_POSITION.key4 = !KEY_POSITION.key4;
		led_blue_toggle();
		
	}
	
}

void EXTI15_10_IRQHandler(void)
{
	
	if( EXTI_GetITStatus(EXTI_Line11) != RESET && (GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == 0) )        {//KEY3
		EXTI_ClearITPendingBit(EXTI_Line11);
		KEY_POSITION.key3 = !KEY_POSITION.key3;
		led_red_toggle();
		
	}
	
	
	else if( EXTI_GetITStatus(EXTI_Line12) != RESET && (GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0))        {//KEY2
		EXTI_ClearITPendingBit(EXTI_Line12);
		KEY_POSITION.key2 = !KEY_POSITION.key2;
		led_green_toggle();
		
	}
	
}

void EXTI3_IRQHandler(void)
{
	
	if( EXTI_GetITStatus(EXTI_Line3) != RESET && (GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_3) == 0) )        {//KEY1
		EXTI_ClearITPendingBit(EXTI_Line3);
		KEY_POSITION.key1 = !KEY_POSITION.key1;
//		buzzer_toggle();
		
	}
	
}



/*************************************KEY**************************************/





/*************************************LCD**************************************/
//  	CS_Pin               PB11
//  	DC_Pin               PI1
//  	RES_Pin              PA2
//  	DATA_Pin             PC4
//  	SCK_Pin              PC5
static void LCD_Pin_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOI|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);                   //CS_Pin_PB11
	
	CS_H;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOI, &GPIO_InitStructure);                   //DC_Pin_PI1
	DC_H;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOA, &GPIO_InitStructure);                   //RES_Pin_PA2
	RES_H;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //DATA_Pin_PC4
	DATA_L;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStructure);                   //SCK_Pin_PC5
	SCK_L;
}


static void LCD_Write_Bus(u8 data) 
{	
	uint8_t cnt;
	
	CS_L;
	
  for(cnt = 0; cnt<8; cnt++) {
		SCK_L;
		
		if(data & 0x80) DATA_H;
		else DATA_L;
		
		
		SCK_H;
		data = (data << 1);
		
	}		
	
	CS_H;
}




void LCD_Write_DATA8(uint8_t data8)
{
	LCD_Write_Bus(data8);
}


void LCD_Write_DATA16(uint16_t data16)
{
	LCD_Write_Bus((u8)(data16>>8));         //DATA_H8
	LCD_Write_Bus((u8)(data16&0xff));       //DATA_L8
}


void LCD_Write_Register(uint8_t data)
{
	DC_L;
	LCD_Write_Bus(data);
	DC_H;
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		LCD_Write_Register(0x2a);//列地址设置
		LCD_Write_DATA16(x1+2);
		LCD_Write_DATA16(x2+2);
		LCD_Write_Register(0x2b);//行地址设置
		LCD_Write_DATA16(y1+1);
		LCD_Write_DATA16(y2+1);
		LCD_Write_Register(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		LCD_Write_Register(0x2a);//列地址设置
		LCD_Write_DATA16(x1+2);
		LCD_Write_DATA16(x2+2);
		LCD_Write_Register(0x2b);//行地址设置
		LCD_Write_DATA16(y1+1);
		LCD_Write_DATA16(y2+1);
		LCD_Write_Register(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		LCD_Write_Register(0x2a);//列地址设置
		LCD_Write_DATA16(x1+1);
		LCD_Write_DATA16(x2+1);
		LCD_Write_Register(0x2b);//行地址设置
		LCD_Write_DATA16(y1+2);
		LCD_Write_DATA16(y2+2);
		LCD_Write_Register(0x2c);//储存器写
	}
	else
	{
		LCD_Write_Register(0x2a);//列地址设置
		LCD_Write_DATA16(x1+1);
		LCD_Write_DATA16(x2+1);
		LCD_Write_Register(0x2b);//行地址设置
		LCD_Write_DATA16(y1+2);
		LCD_Write_DATA16(y2+2);
		LCD_Write_Register(0x2c);//储存器写
	}
}



void LCD_Init(void)
{
	int i;
	LCD_Pin_init();//初始化GPIO
	RES_L;  //复位

	for( i=0; i<0xfff; i++);

	RES_H;

	for( i=0; i<0xfff; i++);
	
	//************* Start Initial Sequence **********//
	LCD_Write_Register(0x11); //Sleep out 

	for( i=0; i<0xfff; i++);
	
	//------------------------------------ST7735S Frame Rate-----------------------------------------// 
	LCD_Write_Register(0xB1); 
	LCD_Write_DATA8(0x05); 
	LCD_Write_DATA8(0x3C); 
	LCD_Write_DATA8(0x3C); 
	LCD_Write_Register(0xB2); 
	LCD_Write_DATA8(0x05);
	LCD_Write_DATA8(0x3C); 
	LCD_Write_DATA8(0x3C); 
	LCD_Write_Register(0xB3); 
	LCD_Write_DATA8(0x05); 
	LCD_Write_DATA8(0x3C); 
	LCD_Write_DATA8(0x3C); 
	LCD_Write_DATA8(0x05); 
	LCD_Write_DATA8(0x3C); 
	LCD_Write_DATA8(0x3C); 
	//------------------------------------End ST7735S Frame Rate---------------------------------// 
	LCD_Write_Register(0xB4); //Dot inversion 
	LCD_Write_DATA8(0x03); 
	//------------------------------------ST7735S Power Sequence---------------------------------// 
	LCD_Write_Register(0xC0); 
	LCD_Write_DATA8(0x28); 
	LCD_Write_DATA8(0x08); 
	LCD_Write_DATA8(0x04); 
	LCD_Write_Register(0xC1); 
	LCD_Write_DATA8(0XC0); 
	LCD_Write_Register(0xC2); 
	LCD_Write_DATA8(0x0D); 
	LCD_Write_DATA8(0x00); 
	LCD_Write_Register(0xC3); 
	LCD_Write_DATA8(0x8D); 
	LCD_Write_DATA8(0x2A); 
	LCD_Write_Register(0xC4); 
	LCD_Write_DATA8(0x8D); 
	LCD_Write_DATA8(0xEE); 
	//---------------------------------End ST7735S Power Sequence-------------------------------------// 
	LCD_Write_Register(0xC5); //VCOM 
	LCD_Write_DATA8(0x1A); 
	LCD_Write_Register(0x36); //MX, MY, RGB mode 
	if(USE_HORIZONTAL==0)LCD_Write_DATA8(0x00);
	else if(USE_HORIZONTAL==1)LCD_Write_DATA8(0xC0);
	else if(USE_HORIZONTAL==2)LCD_Write_DATA8(0x70);
	else LCD_Write_DATA8(0xA0); 
	//------------------------------------ST7735S Gamma Sequence---------------------------------// 
	LCD_Write_Register(0xE0); 
	LCD_Write_DATA8(0x04); 
	LCD_Write_DATA8(0x22); 
	LCD_Write_DATA8(0x07); 
	LCD_Write_DATA8(0x0A); 
	LCD_Write_DATA8(0x2E); 
	LCD_Write_DATA8(0x30); 
	LCD_Write_DATA8(0x25); 
	LCD_Write_DATA8(0x2A); 
	LCD_Write_DATA8(0x28); 
	LCD_Write_DATA8(0x26); 
	LCD_Write_DATA8(0x2E); 
	LCD_Write_DATA8(0x3A); 
	LCD_Write_DATA8(0x00); 
	LCD_Write_DATA8(0x01); 
	LCD_Write_DATA8(0x03); 
	LCD_Write_DATA8(0x13); 
	LCD_Write_Register(0xE1); 
	LCD_Write_DATA8(0x04); 
	LCD_Write_DATA8(0x16); 
	LCD_Write_DATA8(0x06); 
	LCD_Write_DATA8(0x0D); 
	LCD_Write_DATA8(0x2D); 
	LCD_Write_DATA8(0x26); 
	LCD_Write_DATA8(0x23); 
	LCD_Write_DATA8(0x27); 
	LCD_Write_DATA8(0x27); 
	LCD_Write_DATA8(0x25); 
	LCD_Write_DATA8(0x2D); 
	LCD_Write_DATA8(0x3B); 
	LCD_Write_DATA8(0x00); 
	LCD_Write_DATA8(0x01); 
	LCD_Write_DATA8(0x04); 
	LCD_Write_DATA8(0x13); 
	//------------------------------------End ST7735S Gamma Sequence-----------------------------// 
	LCD_Write_Register(0x3A); //65k mode 
	LCD_Write_DATA8(0x05); 
	LCD_Write_Register(0x29); //Display on 
}






/*************************************LCD**************************************/
