#include "headfile.h"
#include "Initial.h"

//Motor_PIDL
float Motor_P=3,Motor_I=8,Motor_D=0.4;
int16 speed_R=0,speed_L=0;			//理想电机值
float error1,error2;      //电机第一次误差值
float error_pre1=0,error_pre2=0;
float error_pre_last1=0,error_pre_last2=0;	//电机累计二次误差值

//Dir_PID
float Dir_P=0,Dir_I=153,Dir_D=0;
float dir_error;
float dir_error_last;	
float Dir_value = 0 ;

//Dir_PIDR
//float Dir_Pr=3.9,Dir_Ir=6.4,Dir_Dr=0;//右侧物理阻力较大

//Line_PID
float Line_D,Line_I,Line_P;
float Line_error;
float Line_error_last;
float Line_value = 0 ;



//逻辑变量

//数值变量
int16 templ_pluse = 0;
int16 tempr_pluse = 0;
float Current_speed;  //实速
int16 dutyL,dutyR;
float a,b;			//计算过渡值  

void Motor()
{

	/*****************************************************************
	Tips:以下部分为编码器部分
	*****************************************************************/
	//读取采集到的编码器脉冲数
  templ_pluse = ctimer_count_read(SPEEDL_PLUSE);
	tempr_pluse = ctimer_count_read(SPEEDR_PLUSE);

   //计数器清零
  ctimer_count_clean(SPEEDL_PLUSE);
	ctimer_count_clean(SPEEDR_PLUSE);

    //采集方向信息
	if(1 == SPEEDR_DIR) tempr_pluse = -tempr_pluse;    
	if(0 == SPEEDR_DIR) tempr_pluse = abs(tempr_pluse);
	//计算目前速度
	Current_speed=(abs(templ_pluse)+abs(tempr_pluse))/2;
	Current_speed=(1)*Current_speed*20.4/(2355.2*0.02);//速度=脉冲数*周长/2368*周期;得到实际值 

	/*****************************************************************
	Tips:以下部分为直线参数设置
	*****************************************************************/	
			speed_R = 30;
			speed_L = 30;
			
	/*****************************************************************
	Tips:以下部分为PID控制部分
	*****************************************************************/		
			a=(1)*templ_pluse*20.4/(2355.2*0.02);//速度=脉冲数*周长/2368*周期;得到实际值
			b=(1)*tempr_pluse*20.4/(2355.2*0.02);//速度=脉冲数*周长/2368*周期;得到实际值
			
			//消除未启动的自积分误差
			if(Statu == 1){

			error1=(int)(speed_L-a)*(1);
			dutyL=dutyL+(error1-error_pre1)*Motor_P+error1*Motor_I+	(error1-2*(error_pre1)+error_pre_last1)*Motor_D;
			error_pre_last1=error_pre1;
			error_pre1=error1;
		
			error2=(int)(speed_R-b)*(1);
			dutyR=dutyR+(error2-error_pre2)*Motor_P+error2*Motor_I+	(error2-2*(error_pre2)+error_pre_last2)*Motor_D;
			error_pre_last2=error_pre2;
			error_pre2=error2;
			}
				
	/*****************************************************************
	Tips:以下直线调节部分
	*****************************************************************/	
		if(Type == 0)
		{
			if (abs(offset2) > 80)	{
				 dutyR = dutyR-4*offset2;
				 dutyL = dutyL+4*offset2;
				}

			else if (abs(offset2) > 30){
				dutyR = dutyR-1*offset2;
				dutyL = dutyL+1*offset2;
			}

			else if(abs(offset2) > 20 )
				{Line_P=100;Line_I=0.5;}
			else if(abs(offset2) > 15 )
				{Line_P=80;Line_I=1;}
	
			Line_value = (offset2-Line_error)*Line_P+offset2*Line_I;
			Line_error = offset2;
			dutyR = dutyR-Line_value;
			dutyL = dutyL+Line_value;
		}
		
	/*****************************************************************
	Tips:开环入环程序
	*****************************************************************/	
		if(Type == 114)
		{dutyR = 3000;dutyL = 0;}

		if(Type == 514)
		{dutyL = 3000;dutyR = 0;}

	/*****************************************************************
	Tips:以下部分为差速控制部分(PID)
	*****************************************************************/	
		if(Type == 1)
		{
		Dir_value = offset*Dir_I+(offset-2*(dir_error)+dir_error_last)*Dir_D;
		dir_error = offset;
		dir_error_last = dir_error;
		dutyR = dutyR+multiple*Dir_value;
		dutyL = dutyL-multiple*Dir_value;
		}
			
		
	/*****************************************************************
	Tips:以下部分为出线停车部分
	*****************************************************************/	
	if(data_last[0]+data_last[1]+data_last[4]+data_last[3]<=100)
			dutyL = dutyR = 0;

	/*****************************************************************
	Tips:开环避障程序
	*****************************************************************/		
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
	
	/*****************************************************************
	Tips:最高优先，按键启动
	*****************************************************************/	
	if(Statu == 0) dutyL = dutyR = 0;
	
	/*****************************************************************
	Tips:以下部分为电机运行代码
	*****************************************************************/	
		//限幅
		if(dutyL>6000) dutyL=6000;
		if(dutyL<-2000) dutyL = -2000;
		//限幅
		if(dutyR>6000) dutyR=6000;
		if(dutyR<-2000) dutyR = -2000;
	
	if(dutyL <= 0) //正转
        {
			DIR_L = 0;
			pwm_duty(PWM_L, -dutyL);}
	else
				{
			DIR_L = 1;
			pwm_duty(PWM_L, dutyL);}
	if(dutyR <= 0) //正转
				{			
			DIR_R = 0;
			pwm_duty(PWM_R, -dutyR);}  
  else         //反转
        {			
			DIR_R = 1;
			pwm_duty(PWM_R, dutyR);}
}	