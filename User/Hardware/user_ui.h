#ifndef     __USER_UI_H
#define     __USER_UI_H

#include "main.h"
#include "lcd.h"
#include "string.h"
#include "lcd_key_led.h"

#define   MENU_NUM     4
#define   PARA_NUM     10
#define   TASK_NUM     10


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

back_mesg Menu_table(back_mesg num);
back_mesg Para_table(back_mesg num);
back_mesg Task_table(back_mesg num);

back_mesg Para_change(back_mesg num);
back_mesg Task_change(back_mesg num);
back_mesg Start_table();
void Para_Task_Start(void);

	
#endif
