#ifndef     __USER_UI_H
#define     __USER_UI_H

#include "main.h"
#include "lcd.h"
#include "string.h"
#include "lcd_key_led.h"
#include "stdio.h"

#define   MENU_NUM     3
#define   PARA_NUM     8
#define   TASK_NUM     8


typedef struct
{
	int table_index;
	int choose_index;
}   back_mesg;


typedef struct
{
	unsigned char current;         		//当前索引号
	unsigned char back;					//后退索引号
	back_mesg (*current_operation)(back_mesg);  		//函数指针
}   choose_table;

typedef struct
{
	fp32 Para[10];
}	Para_change_struct;


void Para_Task_Start(void);
const Para_change_struct * get_para_pointe(void);
const Para_change_struct * get_task_pointe(void);
	
#endif
