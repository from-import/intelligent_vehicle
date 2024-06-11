#include "headfile.h"
#include "Initial.h"

//逻辑变量
int Type = 0;
int delay_circle = 0;
int delay_avoiding = 0;
int Chance = 0;
int Hand = 0;
int OPEN_corner = 0;
int longCheck = 0;

//数值变量
int multiple = 1;

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
		if (data_last[1] > 400 && data_last[3] > 400 ){
			if(longCheck == 0){Type = 0;multiple = 2;}
			if(longCheck != 0) {longCheck = longCheck -1;}
   		}		
		else if ((data_last[1] > 600 || data_last[3] > 600) && data_last[2] < 390){
			longCheck = longCheck + 5;
			Type = 1; //转弯
        	multiple = 1;
			BEEP = 1;

    	}
		else if((data_last[2] > 600) && (data_last[1] + data_last[3] > 50) && (Type != 114)&& (Type != 514) && (data_last[0] > data_last[4])){
			Type = 114; //入环判定
			multiple = 0.8;
			BEEP = 1;
		}
		else if((data_last[2] > 600) && (data_last[1] + data_last[3] > 50) && (Type != 114)&& (Type != 514) && (data_last[0] < data_last[4])){
			Type = 514; //入环判定
			multiple = 0.8;
			BEEP = 1;
		}
    	else{
			BEEP = 0;
        	Type = 0;
    	}
	/*****************************************************************
		Tips:以下为其他
	*****************************************************************/			

	
	//避免再次入环
	if(delay_circle == 2)
		Type = 0;

	//避障判定
	if(dl1b_distance_mm < 1000 && Chance == 1)
		Type = 666;
		Chance = Chance - 1;
	
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

