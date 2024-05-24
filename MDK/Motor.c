#include "headfile.h"
#include "Initial.h"

// Motor_PIDL
float Motor_P = 0.5, Motor_I = 1.5, Motor_D = 0.6;
int16 speed_R = 0, speed_L = 0;  // 理想电机值
float error1, error2;  // 电机第一次误差值
float error_pre1 = 0, error_pre2 = 0;
float error_pre_last1 = 0, error_pre_last2 = 0;  // 电机累计二次误差值

// Dir_PID
float Dir_P = 3, Dir_I = 8, Dir_D = 0.01;
float dir_error;
float dir_error_last;
float Dir_value = 0;

// Dir_PIDR
float Dir_Pr = 3, Dir_Ir = 8, Dir_Dr = 0.01;  // 右侧物理阻力较大

// Line_PID
float Line_P = 0.1, Line_I = 0.3;
float Line_error = 0;
float Line_value = 0;


// 数值变量
int16 templ_pluse = 0;
int16 tempr_pluse = 0;
float Current_speed;  // 实速
int16 dutyL, dutyR;
float a, b;  // 计算过渡值

void Motor()
{
    // Tips:以下部分为编码器部分
    // 读取采集到的编码器脉冲数
    templ_pluse = ctimer_count_read(SPEEDL_PLUSE);
    tempr_pluse = ctimer_count_read(SPEEDR_PLUSE);

    // 计数器清零
    ctimer_count_clean(SPEEDL_PLUSE);
    ctimer_count_clean(SPEEDR_PLUSE);

    // 采集方向信息
    if (SPEEDL_DIR == 0) templ_pluse = -templ_pluse;
    if (SPEEDR_DIR == 1) tempr_pluse = -tempr_pluse;

    // 计算目前速度
    Current_speed = (abs(templ_pluse) + abs(tempr_pluse)) / 2;
    Current_speed = (1) * Current_speed * 20.4 / (2355.2 * 0.02);  // 速度 = 脉冲数 * 周长 / 2368 * 周期; 得到实际值

    // 直线参数设置
    speed_R = 10;
	  speed_L = 10;

    // PID控制部分
    a = (1) * templ_pluse * 20.4 / (2355.2 * 0.02);  // 速度 = 脉冲数 * 周长 / 2368 * 周期; 得到实际值
    b = (1) * tempr_pluse * 20.4 / (2355.2 * 0.02);  // 速度 = 脉冲数 * 周长 / 2368 * 周期; 得到实际值

    error1 = (int)(speed_L - b) * (1);
    dutyL = dutyL + (error1 - error_pre1) * Motor_P + error1 * Motor_I + (error1 - 2 * (error_pre1) + error_pre_last1) * Motor_D;
    error_pre_last1 = error_pre1;
    error_pre1 = error1;

    error2 = (int)(speed_R - a) * (1);
    dutyR = dutyR + (error2 - error_pre2) * Motor_P + error2 * Motor_I + (error2 - 2 * (error_pre2) + error_pre_last2) * Motor_D;
    error_pre_last2 = error_pre2;
    error_pre2 = error2;

    // a,b分别为目前的右轮和左轮速度
    // error1 为 左轮的设定速度与实际速度的差值,越大说明误差越大
    // error2 为 右轮的设定速度与实际速度的差值,越大说明误差越大
    // multiple == 1 为修正值

	/*****************************************************************
	Tips:以下部分为// Type = 0 为直线状态（闭环调节）
	*****************************************************************/	    

    // offset 为 01 的电磁值和 与 34 的电磁值和比值
    // offset2 为 0 的电磁值和 4 的电磁值 比值
    if (Type == 0){
        if(abs(offset2) < 10000){
            Line_value = (offset2 - Line_error) * Line_P + offset2 * Line_I;
            Line_error = offset2;
            dutyR = dutyR - Line_value;
            dutyL = dutyL + Line_value;
        }
    }
		
	/*****************************************************************
	Tips:以下部分为// Type = 0 为直线状态（开环调节）
	*****************************************************************/
		/*
    if(Type == 0){
        if (abs(offset2) > 80){
            dutyR = dutyR-0.4*offset2;
            dutyL = dutyL+0.4*offset2;
        }

        //else if (abs(offset2) > 70)
        //{dutyR = dutyR-0.3*offset2;
        //dutyL = dutyL+0.3*offset2;}

        else if (abs(offset2) > 60){
            dutyR = dutyR-0.3*offset2;
            dutyL = dutyL+0.3*offset2;
        }	

        //else if (abs(offset2) > 50)
        //{dutyR = dutyR-0.2*offset2;
        //dutyL = dutyL+0.2*offset2;}

//        else if (abs(offset2) > 40){
//            dutyR = dutyR-0.5*offset2;
//            dutyL = dutyL+0.5*offset2;
//        }

        else if (abs(offset2) > 30){
            dutyR = dutyR-0.15*offset2;
            dutyL = dutyL+0.15*offset2;
        }
				else if (abs(offset2) > 10){
            dutyR = dutyR-0.05*offset2;
            dutyL = dutyL+0.05*offset2;
        }
    }
		*/
		
	/*****************************************************************
	Tips:以下部分为// Type == 1 表示转弯
	*****************************************************************/	
    if (Type == 1){
//			speed_R = 10;
//	    speed_L = 10;
        if (offset2 > 0){
            // 使用比例-积分-微分（PID）控制器计算 Dir_value
            Dir_value = (offset - dir_error) * Dir_P  // 比例项：当前偏差与前一偏差的差值
                        + offset * Dir_I             // 积分项：偏差的累积
                        + (offset - 2 * dir_error + dir_error_last) * Dir_D; // 微分项：偏差的变化率

			      dir_error = offset;
            
            dutyR = dutyR - multiple * Dir_value; // 右侧电机占空比减少
            dutyL = dutyL + multiple * Dir_value; // 左侧电机占空比增加
        }
        else {
            // 使用另一个 PID 控制器计算 Dir_value
            Dir_value = (offset - dir_error) * Dir_Pr  // 比例项：当前偏差与前一偏差的差值
                        + offset * Dir_Ir             // 积分项：偏差的累积
                        + (offset - 2 * dir_error + dir_error_last) * Dir_Dr; // 微分项：偏差的变化率

            dir_error_last = dir_error;
			dir_error = offset;
        
            // 调整电机的占空比，以控制车辆转向
            dutyR = dutyR - multiple * Dir_value; // 右侧电机占空比减少
            dutyL = dutyL + multiple * Dir_value; // 左侧电机占空比增加
        }
    }


	/*****************************************************************
	Tips:以下部分为开环入环程序
	*****************************************************************/	
    if (delay_circle == 1) {
        dutyR = 1200;
        dutyL = 0;
    }

	/*****************************************************************
	Tips:以下部分为出线停车部分
	*****************************************************************/	
    if (data_last[0] + data_last[1] + data_last[4] + data_last[3] <= 5)
        dutyL = dutyR = 0;

	/*****************************************************************
	Tips:开环避障程序
	*****************************************************************/	
    if (delay_avoiding == 1) {
        if (Distance < 50) {
            dutyL = 2000;
            dutyR = -1800;
        } else if (Distance < 110) {
            dutyL = 2000;
            dutyR = 2000;
        } else if (Distance < 210) {
            dutyR = 4500;
            dutyL = -2300;
        } else {
            dutyL = 2000;
            dutyR = 2000;
        }
    }

    // Tips:最高优先，按键启动
    if (Statu == 0 || Statu == 2)
        dutyL = dutyR = 0;
    
    // 限幅
    if (dutyL > 6000) dutyL = 6000;
    if (dutyL < -2000) dutyL = -2000;
    if (dutyR > 6000) dutyR = 6000;
    if (dutyR < -2000) dutyR = -2000;

    //  给电机赋值
    // pwm_duty
    if (dutyL <= 0) {  // 正转
        DIR_L = 0;
        pwm_duty(PWM_L, -dutyL);
    } else {
        DIR_L = 1;
        pwm_duty(PWM_L, dutyL);
    }
    if (dutyR <= 0) {  // 正转
        DIR_R = 1;
        pwm_duty(PWM_R, -dutyR);
    } else {  // 反转
        DIR_R = 0;
        pwm_duty(PWM_R, dutyR);
    }
}
