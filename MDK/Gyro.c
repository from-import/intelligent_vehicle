#include "headfile.h"
#include "Initial.h"
//自己的延时函数调用
int clockk = 0;
int Time_over = 0;

//数值变量
float Angle_gz = 0;
int Yaw;
int Distance = 0;

//逻辑变量
int Record_Dis = 0;
int Record_Gy = 0;

void Gyro(){
		if(Record_Gy){
			mpu6050_get_accdata();	//获取陀螺仪数据
			mpu6050_get_gyro();		//获取加速度计数据
			mpu6050_gyro_z /= 54.63;
			Angle_gz = Angle_gz + mpu6050_gyro_z*0.4;
			Yaw = (int16)Angle_gz;
		}
		else Yaw = 0;
}

void Disney(){
		/***距离记录***/
		if(Record_Dis)
			Distance=Distance+Current_speed*10*0.005;
		else Distance=0;
}

//ms只能是5的倍数
int delay_mine(int ms){
		clockk = (ms == clockk) ? (0) : (clockk + 5);            // 1000ms 周期计数
    Time_over = (ms == clockk) ? 1 : 0;
	return Time_over;
	}