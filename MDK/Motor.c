#include "headfile.h"
#include "Initial.h"

//Motor_PIDL
float Motor_P=40,Motor_I=5.5,Motor_D=5;
int16 speed_R=0,speed_L=0;			//理想电机值
float error1,error2;      //电机第一次误差值
float error_pre1=0,error_pre2=0;
float error_pre_last1=0,error_pre_last2=0;	//电机累计二次误差值

//Dir_PIDR
float DirR_P=50,DirR_I=220,DirR_D=6;
float dir_errorR;
float dir_error_lastR;	
float Dir_valueR = 0 ;

//Dir_PIDL
float DirL_P=2,DirL_I=220,DirL_D=4;
float dir_errorL;
float dir_error_lastL;	
float Dir_valueL = 0 ;

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
			speed_R = 55;
			speed_L = 55;
			
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
				 dutyR = dutyR-3*offset2;
				 dutyL = dutyL+3*offset2;
				}

			else if (abs(offset2) > 30){
				dutyR = dutyR-1*offset2;
				dutyL = dutyL+1*offset2;
			}

			else if(abs(offset2) > 20 )
				{Line_P=60;Line_I=0.3;}
			
	
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
			if(Hand){
				Dir_valueL = (offset-dir_errorL)*DirL_P+offset*DirL_I+(offset-2*(dir_errorL)+dir_error_lastL)*DirL_D;
				dir_errorL = offset;
				dir_error_lastL = dir_errorL;
				dutyR = dutyR+multiple*Dir_valueL;
				dutyL = dutyL-multiple*Dir_valueL;
			}
			else{
				Dir_valueR = (offset-dir_errorR)*DirR_P+offset*DirR_I+(offset-2*(dir_errorR)+dir_error_lastR)*DirR_D;
				dir_errorR = offset;
				dir_error_lastR = dir_errorR;
				dutyR = dutyR+multiple*Dir_valueR;
				dutyL = dutyL-multiple*Dir_valueR;
			}
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