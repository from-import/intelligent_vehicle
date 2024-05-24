#include "headfile.h"
#include "Initial.h"

float Angle_gz = 0;
int Yaw;
int Distance = 0;
int Record_Dis = 0;

void Gyro(){
		mpu6050_get_accdata();	//获取陀螺仪数据
		mpu6050_get_gyro();		//获取加速度计数据
    mpu6050_gyro_z /= 54.63;
    Angle_gz = Angle_gz + mpu6050_gyro_z*0.4;
    Yaw = (int16)Angle_gz;
}

void Disney(){
    /***距离记录***/
    if(Record_Dis)
    Distance=Distance+Current_speed*10*0.005;
    else Distance=0;
}