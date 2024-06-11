#include "headfile.h"
#include "Initial.h"

//逻辑变量
int Type = 0;
int delay_circle = 0;
int delay_avoiding = 0;
int Chance = 0;
int Hand = 0;
int OPEN_corner = 0;

//数值变量
int multiple = 1;
int Time_Down = 1;
int Time_clock = 0;

void CORNER(){
	    //打开直角延迟
		if(Type == 1){
			Record_Dis = 1;
			Distance = 0;
			OPEN_corner = 1;
			Hand = data_last[3] > data_last[1] ? 1:0;//左1右0
		}
		
		if(OPEN_corner == 1 && Distance > 10000){
			OPEN_corner = 0;//重新准备下一次直角
			Distance = 0;
			Record_Dis = 0;
//			OPEN_corner = 2;//防止转角误判
		}
		
	}

void Flag()
{
	/*****************************************************************
	Tips:
	Type=0  为直线状态
	Type=1  为转弯状态
	Type=2  为R入环状态
	Type=3  为L入环状态
	Type=4  为避障状态
	*****************************************************************/	
	
	//计数关断电磁判断
	if(!Time_Down){
		Time_clock--;
		BEEP = 1;
		if(!Time_clock){
			Time_Down = 1;
		}
	}
	//十字和直角模块处理
	else if(data_last[1] > 1000 || data_last[3] > 1000){		
//		if (abs(data_last[1]-data_last[3]) > 970 &&  && abs(data_last[0]-data_last[4]) > 80){
		if (abs(data_last[0]-data_last[4]) > 80){
			Type = 0; //十字直行
			Time_clock = 50;
   		}
		else{
			Type = 1; //转弯
      multiple = 3;
			Time_clock = 300;
    	}
		}
	//其他元素
		else if((data_last[2] > 600) && (data_last[1] + data_last[3] > 50) && (Type != 114)&& (Type != 514) && (data_last[0] > data_last[4])){
			Type = 114; //入环判定
			multiple = 0.6;
			BEEP = 1;
		}
		else if((data_last[2] > 600) && (data_last[1] + data_last[3] > 50) && (Type != 114)&& (Type != 514) && (data_last[0] < data_last[4])){
			Type = 514; //入环判定
			multiple = 0.6;
			BEEP = 1;
		}
    	else{
				  BEEP = 0;
        	Type = 0;
    	}
/*****************************************************************
		Tips:以下为其他
	*****************************************************************/			
    //直角距离延迟
		CORNER();
	
	//避免再次入环
	if(delay_circle == 2)
		Type = 0;

	//避障判定
	if(dl1b_distance_mm < 620 && Chance == 1)
		Type = 4;
	
	//delay_circle,延迟入环标记
	if(Type == 2 || Type == 3)
	{delay_circle = 1;Record_Dis = 1;}
	//退出入环
	if(delay_circle == 1 && Distance > 50 )
	{delay_circle = 2;Record_Dis = 0;}
	
	//delay_avoiding,避障标志
	if(Type == 4)
	{delay_avoiding = 1;Record_Dis = 1;}
	else if(delay_avoiding == 1 && Distance > 290)
	{delay_avoiding = 0;Record_Dis = 0;Type = 0;Chance = 0;}
}

