#include "user_ui.h"

extern key_mode KEY_POSITION;                                                      					//记录当前按键状态
key_mode KEY_POSITION_OLD;                                                         					//记录上一次按键状态

static const unsigned char Title_name[10] = "Choose";                       						//标题
static const unsigned char Menu_name[10][10] = {"Para","Task","Start","----"};     					//开始选择界面的选项栏

static const unsigned char Para_name[10][10] = {"Mode","Para2","Para3","Para4",   \
									"Para5","Para6","Para7","Para8"};								//修改参数列表

static fp32 para_change[10];                                                                        //修改参数值
Para_change_struct para_Para={0};
Para_change_struct task_Para={0};
									
static const unsigned char Task_name[10][10] = {"Task1","Task2","Task3","Task4",   \
									"Task5","Task6","Task7","Task8"};												//修改任务目标列表

static fp32 task_change[10];                                                                        //修改参数值

									
static back_mesg Para_change(back_mesg num);
static back_mesg Task_change(back_mesg num);
									
static back_mesg Menu_table(back_mesg num);
static back_mesg Para_table(back_mesg num);
static back_mesg Task_table(back_mesg num);
static back_mesg Start_table();
									
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
		
	LCD_Fill(0,0,(160 - len*16)/2,32,DARKBLUE);    									//填充标题色
	LCD_ShowString((160 - len*16)/2,0,str,BLACK,DARKBLUE,32,0);  	//填充标题
	LCD_Fill((160 + len*16)/2,0,160,32,DARKBLUE);    									//填充标题色
}


static void Content_Show(const unsigned char * content, int locat)
{
	unsigned char str[20];
	uint8_t len;
	
	strcpy((char*)str, (char*)content);
	len = strlen((char*)str);
	
//	LCD_Fill(0,32+24*locat,160,56+24*locat,LBBLUE);       								//填充条目背景色
	LCD_ShowString(35,32+24*locat,content,GBLUE,LBBLUE,24,0);  		//填充选项条目
	LCD_Fill(35 + len*12,32+24*locat,160,56+24*locat,LBBLUE);  
}

static void Choose_Show(int choose)
{
	LCD_Fill(0,32,35,56,LBBLUE);                                    //第一行
	
	LCD_Fill(0,56,12,80,LBBLUE);                                    //第二行
	LCD_ShowChar(11,32+24*1,'*',BLACK,LBBLUE,24,0);
	
	LCD_Fill(0,80,35,128,LBBLUE);
	LCD_Fill(23,56,35,80,LBBLUE);	
}

static void Choose_Num_Show(unsigned char * content,int choose)
{
	LCD_Fill(0,32,160,56,LBBLUE);                                    //第一行
	LCD_ShowString(10,56,content,GBLUE,LBBLUE,24,0);                 //第二行数字行
	
	if(choose<=3)  {
		LCD_Fill(0,80,10+12*choose,104,LBBLUE);                          //第三行选项行
		LCD_ShowChar(10+12*choose,80,'^',BLACK,LBBLUE,24,0);
		LCD_Fill(22+12*choose,80,160,104,LBBLUE);
	}
	else if(choose>=4&&choose!=8) {
		LCD_Fill(0,80,22+12*choose,104,LBBLUE);                          //第三行选项行
		LCD_ShowChar(22+12*choose,80,'^',BLACK,LBBLUE,24,0);
		LCD_Fill(34+12*choose,80,160,104,LBBLUE);
	}
	else {
		LCD_Fill(0,80,40+12*choose,104,LBBLUE);                          //第三行选项行
		LCD_ShowChar(40+12*choose,80,'^',BLACK,LBBLUE,24,0);
		LCD_Fill(52+12*choose,80,160,104,LBBLUE);
	}
	
	LCD_Fill(0,104,160,128,LBBLUE);                                  //第四行
}



static back_mesg Menu_table(back_mesg num)
{
	static int enter_choose = 1;
	char show_string[4][10];
	char enter_flag = 0;
	char back_flag = 0;
	char i;
	back_mesg action;
	
	//信息获取，决定下一轮去向
	if(GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_3) == 0)          //上翻键
		enter_choose--;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0))          //后退键
		back_flag = 1;
	if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == 0))          //下翻键
		enter_choose++;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) == 0))          //确定键
		enter_flag = 1;
	if(enter_choose<=0)
		enter_choose += MENU_NUM;
	enter_choose = enter_choose%MENU_NUM;                            //获取选择位置
	
	
	//根据宏定义及传入参数，选择显示内容
	Title_Show (Title_name);
	
	for(i=0;i<4;i++) {
		if(enter_choose-1+i<0)
			strcpy((char*)show_string[i], (char*)"----");
		else if(enter_choose-1+i>=MENU_NUM)
			strcpy((char*)show_string[i], (char*)"----");
		else
			strcpy((char*)show_string[i], (char*)Menu_name[enter_choose-1+i]);
	}
	
	for(i=0; i<4; i++) 
		Content_Show(show_string[i], i);
	
	Choose_Show(num.choose_index);
	
	
	//计算回传数据
	if(enter_flag)    {                                         
		action.table_index = enter_choose*11+1;                   //下一次位置为选择的偏移量
		action.choose_index = 1;								  //光标复位
		return action;
	}
	if(back_flag)    {                                          
		action.table_index = table[num.table_index].back;		  //下一次位置为返回的index
		action.choose_index = 1;								  //光标复位
		return action;
	}
	
	action.table_index = num.table_index;                   	  //下一次位置为当前位置
	action.choose_index = enter_choose;							  //光标计算
	return action;
	
		

}

static back_mesg Para_table(back_mesg num)
{
	static int enter_choose = 1;
	char show_string[4][10];
	char enter_flag = 0;
	char back_flag = 0;
	char i;
	back_mesg action;
	
	//信息获取，决定下一轮去向
	if(GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_3) == 0)          //上翻键
		enter_choose--;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0))          //后退键
		back_flag = 1;
	if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == 0))          //下翻键
		enter_choose++;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) == 0))          //确定键
		enter_flag = 1;
	if(enter_choose<=0)
		enter_choose += PARA_NUM;
	enter_choose = enter_choose%PARA_NUM;                            //获取选择位置
	
	
	//根据宏定义及传入参数，选择显示内容
	Title_Show (Menu_name[0]);
	
	for(i=0;i<4;i++) {
		if(enter_choose-1+i<0)
			strcpy((char*)show_string[i], (char*)"----");
		else if(enter_choose-1+i>=PARA_NUM)
			strcpy((char*)show_string[i], (char*)"----");
		else
			strcpy((char*)show_string[i], (char*)Para_name[enter_choose-1+i]);
	}
	
	for(i=0; i<4; i++) 
		Content_Show(show_string[i], i);
	
	Choose_Show(num.choose_index);
	
	
	//计算回传数据
	if(enter_flag)    {                                         
		action.table_index = enter_choose+2;                   //下一次位置为选择的偏移量
		action.choose_index = 0;								  //光标复位
		return action;
	}
	if(back_flag)    {                                          
		action.table_index = table[num.table_index].back;		  //下一次位置为返回的index
		action.choose_index = 1;								  //光标复位
		return action;
	}
	
	action.table_index = num.table_index;                   	  //下一次位置为当前位置
	action.choose_index = enter_choose;							  //光标计算
	return action;
}

static back_mesg Task_table(back_mesg num)
{
	static int enter_choose = 1;
	char show_string[4][10];
	char enter_flag = 0;
	char back_flag = 0;
	char i;
	back_mesg action;
	
	//信息获取，决定下一轮去向
	if(GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_3) == 0)          //上翻键
		enter_choose--;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0))          //后退键
		back_flag = 1;
	if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == 0))          //下翻键
		enter_choose++;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) == 0))          //确定键
		enter_flag = 1;
	if(enter_choose<=0)
		enter_choose += TASK_NUM;
	enter_choose = enter_choose%TASK_NUM;                            //获取选择位置
	
	
	//根据宏定义及传入参数，选择显示内容
	Title_Show (Menu_name[1]);
	
	for(i=0;i<4;i++) {
		if(enter_choose-1+i<0)
			strcpy((char*)show_string[i], (char*)"----");
		else if(enter_choose-1+i>=TASK_NUM)
			strcpy((char*)show_string[i], (char*)"----");
		else
			strcpy((char*)show_string[i], (char*)Task_name[enter_choose-1+i]);
	}
	
	for(i=0; i<4; i++) 
		Content_Show(show_string[i], i);
	
	Choose_Show(num.choose_index);
	
	
	//计算回传数据
	if(enter_flag)    {                                         
		action.table_index = enter_choose+13;                   //下一次位置为选择的偏移量
		action.choose_index = 0;								  //光标复位
		return action;
	}
	if(back_flag)    {                                          
		action.table_index = table[num.table_index].back;		  //下一次位置为返回的index
		action.choose_index = 1;								  //光标复位
		return action;
	}
	
	action.table_index = num.table_index;                   	  //下一次位置为当前位置
	action.choose_index = enter_choose;							  //光标计算
	return action;
}

static back_mesg Start_table()
{
	back_mesg action;
	
	LCD_Fill(0,0,160,128,BLACK);
	action.table_index = -1;
	return action;
}




static back_mesg Para_change(back_mesg num)
{
	static int enter_choose = 1;
	float enter_num;
	static int show_num[8]={0};
	char show_string[15];
	char up_flag = 0;
	char down_flag = 0;
	char i;
	back_mesg action;
	
	//信息获取，决定下一轮去向
	if(GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_3) == 0)          //上翻键
		up_flag=1;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0))          //后退键
		enter_choose--;
	if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == 0))          //下翻键
		down_flag = 1;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) == 0))          //确定键
		enter_choose++;
	if(enter_choose<=0)
		enter_choose += 9;
	enter_choose = enter_choose%9;                            //获取选择位置
	
	sprintf(show_string,"%d%d%d%d.%d%d%d%d OK",show_num[0],show_num[1],show_num[2],show_num[3],show_num[4],show_num[5],show_num[6],show_num[7]);
	
	Title_Show (Para_name[num.table_index-2]);
	Choose_Num_Show(show_string,enter_choose);
	
	if(up_flag) {
		if(enter_choose==8) {
			action.table_index = table[num.table_index].back;
			action.choose_index = 1;
			for(i=0; i<8;i++)
				show_num[i] = 0;
			
			para_change[num.table_index-2] = show_num[0]*1000+show_num[1]*100+show_num[2]*10+show_num[3]*1+show_num[4]*0.1+show_num[5]*0.01+show_num[6]*0.001+show_num[7]*0.0001;
			return action;
		}
		show_num[enter_choose]++;
			
	}
		
	if(down_flag) {
		if(enter_choose==8) {
			action.table_index = table[num.table_index].back;
			action.choose_index = 1;
			for(i=0; i<8;i++)
				show_num[i] = 0;
			
			para_change[num.table_index-2] = show_num[0]*1000+show_num[1]*100+show_num[2]*10+show_num[3]*1+show_num[4]*0.1+show_num[5]*0.01+show_num[6]*0.001+show_num[7]*0.0001;
			return action;
		}
		show_num[enter_choose]--;
	}
	
	if(show_num[enter_choose]<=0)
		show_num[enter_choose] += 10;
	show_num[enter_choose] = show_num[enter_choose]%10;                            //获取选择位置
	
	return num; 
}

static back_mesg Task_change(back_mesg num)
{
	static int enter_choose = 1;
	float enter_num;
	static int show_num[8]={0};
	char show_string[15];
	char up_flag = 0;
	char down_flag = 0;
	char i;
	back_mesg action;
	
	//信息获取，决定下一轮去向
	if(GPIO_ReadInputDataBit(GPIOI,GPIO_Pin_3) == 0)          //上翻键
		up_flag=1;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12) == 0))          //后退键
		enter_choose--;
	if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11) == 0))          //下翻键
		down_flag = 1;
	if((GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) == 0))          //确定键
		enter_choose++;
	if(enter_choose<=0)
		enter_choose += 9;
	enter_choose = enter_choose%9;                            //获取选择位置
	
	sprintf(show_string,"%d%d%d%d.%d%d%d%d OK",show_num[0],show_num[1],show_num[2],show_num[3],show_num[4],show_num[5],show_num[6],show_num[7]);
	
	Title_Show (Task_name[num.table_index-13]);
	Choose_Num_Show(show_string,enter_choose);
	
	if(up_flag) {
		if(enter_choose==8) {
			action.table_index = table[num.table_index].back;
			action.choose_index = 1;
			for(i=0; i<8;i++)
				show_num[i] = 0;
			
			para_change[num.table_index-13] = show_num[0]*1000+show_num[1]*100+show_num[2]*10+show_num[3]*1+show_num[4]*0.1+show_num[5]*0.01+show_num[6]*0.001+show_num[7]*0.0001;
			return action;
		}
		show_num[enter_choose]++;
			
	}
		
	if(down_flag) {
		if(enter_choose==8) {
			action.table_index = table[num.table_index].back;
			action.choose_index = 1;
			for(i=0; i<8;i++)
				show_num[i] = 0;
			
			para_change[num.table_index-13] = show_num[0]*1000+show_num[1]*100+show_num[2]*10+show_num[3]*1+show_num[4]*0.1+show_num[5]*0.01+show_num[6]*0.001+show_num[7]*0.0001;
			return action;
		}
		show_num[enter_choose]--;
	}
	
	if(show_num[enter_choose]<=0)
		show_num[enter_choose] += 10;
	show_num[enter_choose] = show_num[enter_choose]%10;                            //获取选择位置
	
	return num; 
}



static void Para_changle_in(void)                      //将修改数据装填入实际数据
{
	int i;
	for(i=0; i<8; i++)
		para_Para.Para[i] = para_change[i];
}


static void Task_changle_in(void)                      //将修改数据装填入实际数据
{
	int i;
	for(i=0; i<8; i++)
		task_Para.Para[i] = task_change[i];
}


void Para_Task_Start(void)
{
	static int i;
	back_mesg start={0,0};
	
	Para_changle_in();                      //将修改数据装填入实际数据
	Task_changle_in();                      //将修改数据装填入实际数据
	while(start.table_index!=-1)
	{
		start = table[start.table_index].current_operation(start);
		for(i=0; i<0xfffff;i++);
	}
	
}

const Para_change_struct * get_para_pointe(void)
{
	return &para_Para;
}

const Para_change_struct * get_task_pointe(void)
{
	return &task_Para;
}
