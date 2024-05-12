#include "headfile.h"
#include "Initial.h"

//�߼�����
int Type = 0;
int multiple = 1;
int delay_circle = 0;
int delay_avoiding = 0;
int Chance = 0;
int Demo = 0;

void circle()
{	
	//delay_circle,�ӳ��뻷���
	if(Type == 2)
		{delay_circle = 1;Record_Dis = 1;Demo = 1;}
	if(Type == 3)
		{delay_circle = 2;Record_Dis = 1;Demo = 1;}

	//�뻷�ڶ��׶ν���
	if(Demo == 1 && Distance > 80 )
		{Demo = 2;Record_Dis = 0;Yaw=0;}
	//�뻷�����׶Σ�Ԥ����
	if(Demo == 2 && abs(Yaw) > 300)
		{Demo = 3;Record_Dis = 1;Distance = 0;}
	//�뻷�����׶Σ�����
	if(Demo == 3 && Distance > 100)
		{Type = 0;delay_circle = 0;Record_Dis = 0;}
}

void Flag()
{
	/*****************************************************************
	Tips:Type=0  Ϊֱ��״̬ // Go Long
	     Type=1  Ϊת��״̬ // turn 
			 Type=2  ΪR�뻷״̬  // RIGHT GO IN  CIRCLE
			 Type=3  ΪL�뻷״̬ 	// LEFT GO IN CIRCLE
			 Type=4  Ϊ����״̬ // Avoid Obstacles
	*****************************************************************/	
	
//	if((data_last[1] > 300 && data_last[3] > 10) && data_last[2] > 255)	//ʮ���ж�
//		Type = 0;
//	else if((data_last[3] > 300 && data_last[1] > 5) && data_last[2] > 255)	//ʮ���ж�
//		Type = 0;
	if((data_last[1] >  200 || data_last[3] > 200) && data_last[2] < 300){	//ת�䣨��ת��������0�����700+��ֵ��
	Type = 1;multiple = 1;}
//	else if(data_last[2] > 650 && data_last[0] > 700)	//���뻷
//	{Type = 2;Record_Dis = 0;}
//	else if(data_last[2] > 650 && data_last[4] > 700)	//���뻷
//	{Type = 3;Record_Dis = 0;}
//	else if(data_last[2] < 300 && (data_last[1] > 200 || data_last[3] > 130))
//	{Type = 1;multiple = 1.5;}
	else{
		Type = 0;
	}
	
	//�뻷
	circle();

	//�����ж�
	if(dl1b_distance_mm < 620 && Chance == 1){
		Type = 4;
	}
	
	//delay_circle,�ӳ��뻷���
	if(Type == 2 || Type == 3){
	{delay_circle = 1;Record_Dis = 1;}
	}

	//�˳��뻷
	if(delay_circle == 1 && Distance > 50 ){
	{delay_circle = 2;Record_Dis = 0;}}
	
	//delay_avoiding,���ϱ�־
	if(Type == 4){
	{delay_avoiding = 1;Record_Dis = 1;}}

	else if(delay_avoiding == 1 && Distance > 290){
	{delay_avoiding = 0;Record_Dis = 0;Type = 0;Chance = 0;}}
}

