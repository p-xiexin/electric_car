#include "user_ui.h"

extern key_mode KEY_POSITION;                                                       //记录当前按键状态
key_mode KEY_POSITION_OLD;                                                          //记录上一次按键状态

static const unsigned char Title_name[10] = "Choose";                       					//标题
static const unsigned char Menu_name[10][10] = {"Para","Task","Start","----"};     					//开始选择界面的选项栏
static const unsigned char Para_name[10][10] = {"Vice","Vice_kp","Vice_ki","Vice_kd",   \
									"Angle","Angle_kp","Angle_ki","Angle_kd"};		//修改参数列表
static const unsigned char Task_name[10][10] = {"1","2","3","4",   \
									"5","6","7","8"};								//修改任务目标列表	

choose_table table[30] = {
	//当前，前进，后退，函数指针
	//Menu
	{0,0,(*Menu_table)},
	
	//Para
	{1,0,(*Para_table)},
	//Para_List
	{2,1,(*Para_change)},
	{3,1,(*Para_change)},
	{4,1,(*Para_change)},
	{5,1,(*Para_change)},
	{6,1,(*Para_change)},
	{7,1,(*Para_change)},
	{8,1,(*Para_change)},
	{9,1,(*Para_change)},
	{10,1,(*Para_change)},
	{11,1,(*Para_change)},
	
	//Task
	{12,0,(*Task_table)},
	//Task_List
	{13,12,(*Task_change)},
	{14,12,(*Task_change)},
	{15,12,(*Task_change)},
	{16,12,(*Task_change)},
	{17,12,(*Task_change)},
	{18,12,(*Task_change)},
	{19,12,(*Task_change)},
	{20,12,(*Task_change)},
	{21,12,(*Task_change)},
	{22,12,(*Task_change)},
	
	//Start
	{23,0,(*Start_table)},
	
	
};


static void Title_Show(const unsigned char * title)
{
	unsigned char str[20];
	uint8_t len;
	
	strcpy((char*)str, (char*)title);
	len = strlen((char*)str);
		
	LCD_Fill(0,0,160,32,DARKBLUE);    									//填充标题色
	LCD_ShowString((160 - len*16)/2,0,str,BLACK,DARKBLUE,32,0);  	//填充标题
}


static void Content_Show(const unsigned char * content, int locat)
{
	LCD_Fill(0,32+24*locat,160,56+24*locat,LBBLUE);       								//填充条目背景色
	LCD_ShowString(35,32+24*locat,content,GBLUE,LBBLUE,24,0);  		//填充选项条目
}

static void Choose_Show(int choose)
{
	LCD_ShowChar(11,32+24*choose,'*',BLACK,LBBLUE,24,1);
}



back_mesg Menu_table(back_mesg num)
{
	static int enter_choose = 0;
	char enter_flag = 0;
	char back_flag = 0;
	char i;
	back_mesg action;
	
	//信息获取，决定下一轮去向
	if(KEY_POSITION_OLD.key1 != KEY_POSITION.key1) {          //上翻键
		enter_choose--;
		KEY_POSITION_OLD.key1 = KEY_POSITION.key1;
	}
	if(KEY_POSITION_OLD.key2 != KEY_POSITION.key2) {          //后退键
		back_flag = 1;
		KEY_POSITION_OLD.key2 = KEY_POSITION.key2;
	}
	if(KEY_POSITION_OLD.key3 != KEY_POSITION.key3) {          //下翻键
		enter_choose++;
		KEY_POSITION_OLD.key3 = KEY_POSITION.key3;
	}
	if(KEY_POSITION_OLD.key4 != KEY_POSITION.key4) {          //确定键
		enter_flag = 1;
		KEY_POSITION_OLD.key4 = KEY_POSITION.key4;
	}
	if(enter_choose<=0)
		enter_choose += MENU_NUM;
	enter_choose = enter_choose%MENU_NUM;                            //获取选择位置
	
	
	//根据宏定义及传入参数，选择显示内容
	Title_Show (Title_name);
	
	for(i=0; i<4; i++) 
		Content_Show(Menu_name[i], i);
	
	Choose_Show(num.choose_index);
	
	
	//计算回传数据
	if(enter_flag)    {                                         
		action.table_index = enter_choose*11+1;                   //下一次位置为选择的偏移量
		action.choose_index = 0;								  //光标复位
		return action;
	}
	if(back_flag)    {                                          
		action.table_index = table[num.table_index].back;		  //下一次位置为返回的index
		action.choose_index = 0;								  //光标复位
		return action;
	}
	
	action.table_index = num.table_index;                   	  //下一次位置为当前位置
	action.choose_index = enter_choose;							  //光标计算
	return action;
	
		

}

back_mesg Para_table(back_mesg num)
{
	static int enter_choose = 0;
	char enter_flag = 0;
	char back_flag = 0;
	char up_flag = 0;
	char down_flag = 0;
	char i;
	back_mesg action;
	
	//信息获取，决定下一轮去向
	if(KEY_POSITION_OLD.key1 != KEY_POSITION.key1) {          //上翻键
		enter_choose--;
		up_flag = 1;
		KEY_POSITION_OLD.key1 = KEY_POSITION.key1;
	}
	if(KEY_POSITION_OLD.key2 != KEY_POSITION.key2) {          //后退键
		back_flag = 1;
		KEY_POSITION_OLD.key2 = KEY_POSITION.key2;
	}
	if(KEY_POSITION_OLD.key3 != KEY_POSITION.key3) {          //下翻键
		enter_choose++;
		down_flag = 1;
		KEY_POSITION_OLD.key3 = KEY_POSITION.key3;
	}
	if(KEY_POSITION_OLD.key4 != KEY_POSITION.key4) {          //确定键
		enter_flag = 1;
		KEY_POSITION_OLD.key4 = KEY_POSITION.key4;
	}
	if(enter_choose<=0)
		enter_choose += PARA_NUM;
	enter_choose = enter_choose%PARA_NUM;                     //获取选择位置
	
	
	//根据宏定义及传入参数，选择显示内容
	Title_Show (Menu_name[0]);
	for(i=0; i<4; i++) 
		Content_Show(Para_name[enter_choose-num.choose_index+i], i);
	Choose_Show(num.choose_index);
	
	
	
	
	//计算回传数据
	if(enter_flag)    {                                         
		action.table_index = enter_choose+1+num.table_index;                   //下一次位置为选择的偏移量
		action.choose_index = 0;								  //光标复位
		return action;
	}
	if(back_flag)    {                                          
		action.table_index = table[num.table_index].back;		  //下一次位置为返回的index
		action.choose_index = 0;								  //光标复位
		return action;
	}
	
	action.table_index = num.table_index;                   	  //下一次位置为当前位置
	if(up_flag) {
		if(num.choose_index != 0)
			num.choose_index --;
		else {
			if(enter_choose-num.choose_index==0)
				num.choose_index = 3;
			else
				num.choose_index = 0;
		}
	}
	if(down_flag) {
		if(num.choose_index != 3)
			num.choose_index ++;
		else {
			if(enter_choose-num.choose_index==PARA_NUM)
				num.choose_index = 0;
			else
				num.choose_index = 3;
		}
	}
		
	return action;
}

back_mesg Task_table(back_mesg num)
{
	
}

back_mesg Start_table()
{
	
}

back_mesg Para_change(back_mesg num)
{
	
}

back_mesg Task_change(back_mesg num)
{
	
}

void Para_Task_Start(void)
{
	int i;
	back_mesg start={0,0};
	
	
	while(1)
	{
		start = table[start.table_index].current_operation(start);
		for(i=0;i<0xfff;i++);
	}
}
