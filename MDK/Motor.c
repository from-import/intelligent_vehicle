#include "headfile.h"
#include "Initial.h"

float correction_value = 1.0;
//Motor_PIDL
float Motor_P=0.25,Motor_I=5,Motor_D=0.4;
int16 speed_R=0,speed_L=0;			//������ֵ
float error1,error2;      //�����һ�����ֵ
float error_pre1=0,error_pre2=0;
float error_pre_last1=0,error_pre_last2=0;	//����ۼƶ������ֵ

//Dir_PID
float Dir_P=0.8,Dir_I=1.2,Dir_D=0;
float dir_error;
float dir_error_last;	
float Dir_value = 0 ;

//Dir_PIDR
float Dir_Pr=1.2,Dir_Ir=1.6,Dir_Dr=0;//�Ҳ����������ϴ�

//Line_PID
float Line_P=10,Line_I=1;
float Line_error;
float Line_value = 0 ;

//�߼�����

//��ֵ����
int16 templ_pluse = 0;
int16 tempr_pluse = 0;
float Current_speed;  //ʵ��
int16 dutyL,dutyR;
float a,b;			//�������ֵ  

void Motor()
{

	// Tips:���²���Ϊ����������
	//��ȡ�ɼ����ı�����������
	templ_pluse = ctimer_count_read(SPEEDL_PLUSE);
	tempr_pluse = ctimer_count_read(SPEEDR_PLUSE);

	//����������
	ctimer_count_clean(SPEEDL_PLUSE);
	ctimer_count_clean(SPEEDR_PLUSE);

	//�ɼ�������Ϣ
	if(SPEEDL_DIR == 0)   templ_pluse = -templ_pluse;
	if(SPEEDR_DIR == 1)   tempr_pluse = -tempr_pluse;
	//����Ŀǰ�ٶ�
	Current_speed=(abs(templ_pluse)+abs(tempr_pluse))/2;
	Current_speed=(1)*Current_speed*20.4/(2355.2*0.02);
	//�ٶ�=������*�ܳ�/2368*����;�õ�ʵ��ֵ  Tips:���²���Ϊֱ�߲�������

	speed_R = 25;
	speed_L = 25;
		
	// Tips:���²���ΪPID���Ʋ���

	// Speed Value: Left
	leftSpeed = (1)*templ_pluse*20.4/(2355.2*0.02);
	//�ٶ�=������*�ܳ�/2368*����;�õ�ʵ��ֵ

	// Speed Value: Right
	rightSpeed = (1)*tempr_pluse*20.4/(2355.2*0.02);
	//�ٶ�=������*�ܳ�/2368*����;�õ�ʵ��ֵ

	// PID
	error1=(int)(speed_L-leftSpeed)*(1);
	dutyL=dutyL+(error1-error_pre1)*Motor_P+error1*Motor_I+	(error1-2*(error_pre1)+error_pre_last1)*Motor_D;
	error_pre_last1=error_pre1;
	error_pre1=error1;

	error2=(int)(speed_R-rightSpeed)*(1);
	dutyR=dutyR+(error2-error_pre2)*Motor_P+error2*Motor_I+	(error2-2*(error_pre2)+error_pre_last2)*Motor_D;
	error_pre_last2=error_pre2;
	error_pre2=error2;
					
	// Tips:���²���Ϊ���ٿ��Ʋ���(PID)
	// Turn Around

	if(Type == 1){
		if(offset2 > 0){	
			// offset = data_last0 - data_last4
			// dir_error : last
			// dir_error_last : last last
			// Dir_value -> Target
			// Upgrade:
			Dir_value = (offset-dir_error)*Dir_P+offset*Dir_I+(offset-2*(dir_error)+dir_error_last)*Dir_D;
			dir_error_last = dir_error;
			dir_error = offset;
			// dutyR -> Speed
			dutyR = dutyR-multiple*Dir_value;
			dutyL = dutyL+multiple*Dir_value;
		}

		else{
			Dir_value = (offset-dir_error)*Dir_Pr+offset*Dir_Ir+(offset-2*(dir_error)+dir_error_last)*Dir_Dr;
			dir_error_last = dir_error;
			dir_error = offset;
			dutyR = dutyR-multiple*Dir_value;
			dutyL = dutyL+multiple*Dir_value;
		}
	}

			

// Tips:����ֱ�ߵ��ڲ���(off)
//	if(abs(offset2) > 20 )
//	{Line_P=0.2;Line_I=0.15;}

	 // Value
	if(abs(offset2) > 30 ){
		Line_P=10;Line_I=1;correction_value = 1.0;
		}

	if(Type == 0){
		// offset2 : now  Line_error : last 
		Line_value = (offset2-Line_error)*Line_P+offset2*Line_I;
		Line_error = offset2;
		dutyR = dutyR-Line_value*correction_value;
		dutyL = dutyL+Line_value*correction_value;
		}

	
	// Tips:������������
	

//		if(Type == 0)
//		{
//			  if (abs(offset2) > 80)	
//				{
//				 dutyR = dutyR-0.35*offset2;
//				 dutyL = dutyL+0.35*offset2;
//				}
//				else if (abs(offset2) > 70)
//				{
//				 dutyR = dutyR-0.35*offset2;
//				 dutyL = dutyL+0.35*offset2;
//				}
////        else if (abs(offset2) > 60)
////				{
////					dutyR = dutyR-0.6*offset2;
////					dutyL = dutyL+0.6*offset2;
////				}
//				else if (abs(offset2) > 50)
//				{
//					dutyR = dutyR-0.3*offset2;
//					dutyL = dutyL+0.3*offset2;
//				}
////				else if (abs(offset2) > 40)
////				{
////					dutyR = dutyR-0.4*offset2;
////					dutyL = dutyL+0.4*offset2;
////				}
//		}
		
	// Tips:�����뻷����

	// delay_circle : IN Circle
	if(delay_circle == 1)
		{dutyR = 1200 ; dutyL = 0;}
	if(delay_circle == 0)
		{dutyR = 0 ; dutyL = 1200;}
		

	// Tips:���²���Ϊ����ͣ������
	// Stop Car
	if(data_last[0]+data_last[1]+data_last[4]+data_last[3]<=200){
		dutyL = dutyR = 0;
		}

	
	// Tips:�������ϳ���
	// Avoid obstacles		
	if(delay_avoiding == 1){
		if(Distance < 50)
			{dutyL = 2000;dutyR = -1800;}
		else if(Distance < 110)
			{dutyL = 2000;dutyR = 2000;}
		else if(Distance < 210)
			{dutyR = 4500;dutyL = -2300;}
		else
			{dutyL = 2000;dutyR = 2000;}	
		}
	

	// Tips:������ȣ���������
	
	if(Statu == 0){
		dutyL = dutyR = 0;}
	

	// Tips:���²���Ϊ������д���

	//�޷�
	if(dutyL>6000) dutyL=6000;
	if(dutyL<-2000) dutyL = -2000;
	//�޷�
	if(dutyR>6000) dutyR=6000;
	if(dutyR<-2000) dutyR = -2000;
	

	// NO CHANGE
	if(dutyL <= 0){
		DIR_L = 0;
		pwm_duty(PWM_L, -dutyL);}
	else{
		DIR_L = 1;
		pwm_duty(PWM_L, dutyL);}

	if(dutyR <= 0){			
		DIR_R = 0;
		pwm_duty(PWM_R, -dutyR);}  
	else{			
		DIR_R = 1;
		pwm_duty(PWM_R, dutyR);}
}	