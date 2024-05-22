#include "headfile.h"
#include "Initial.h"

float data_last[5];   // 上一次的电磁值
float Data[5][10] = {0}; // 存储电磁值的数组	
int i = 0, j, k;
float data_s[5] = {0, 0, 0, 0, 0}; // 用于滑动平均滤波的临时变量
int offset;          // 电磁差值计算
int offset2;
float right_value, left_value;
double difference1 = 0.0;
double difference2 = 0.0;
double difference3 = 0.0;
int rightOrLeft ;
int AD_1MAX = 0;
int AD_5MAX = 0;
int AD_2MAX = 0;
int AD_4MAX = 0;
	
//#define AD_1MAX 800            // data_last[0]最大电感值
#define AD_1MIN 10             // data_last[0]最小电感值
//#define AD_5MAX 800            // data_last[4]最大电感值
#define AD_5MIN 10             // data_last[4]最小电感值

//#define AD_2MAX 900        // data_last[1]最大电感值
#define AD_2MIN 10             // data_last[1]最小电感值
//#define AD_4MAX 900          // data_last[3]最大电感值
#define AD_4MIN 10            // data_last[3]最小电感值

int ad_sum;


void ADC_read()
{
    // 读取电磁传感器的数据
    Data[0][9] = Standard * adc_once(ADC_P00, ADC_12BIT); // 00
    Data[1][9] = Standard * adc_once(ADC_P01, ADC_12BIT); // 01
    Data[2][9] = Standard * adc_once(ADC_P05, ADC_12BIT); // 05
    Data[3][9] = Standard * adc_once(ADC_P06, ADC_12BIT); // 06
    Data[4][9] = Standard * adc_once(ADC_P14, ADC_12BIT); // 14
	

    // 滑动平均滤波
    for (j = 0; j<5; j++){
        for (i = 0; i<10; i++){
            data_s[j] += Data[j][i];
        }
        data_last[j] = data_s[j] / (10.0); // 计算平均值作为当前电磁值
        data_s[j] = 0;
        for (k = 0; k<9; k++){
            Data[j][k] = Data[j][k+1]; // 更新数据数组
        }
    }

		//更新max库
    if(Statu == 0){
			AD_1MAX = AD_1MAX > data_last[0] ? AD_1MAX:data_last[0];
			AD_5MAX = AD_5MAX > data_last[4] ? AD_5MAX:data_last[4];
			AD_2MAX = AD_2MAX > data_last[1] ? AD_2MAX:data_last[1];
			AD_4MAX = AD_4MAX > data_last[3] ? AD_4MAX:data_last[3];
		}
			
    difference1 = abs(data_last[0] - data_last[4])/data_last[2];
    difference2 = (data_last[2] - 0.5 * (data_last[0] - data_last[4]))/data_last[2];
    difference3 = (data_last[1] - data_last[3]) / (data_last[1] + data_last[3]);

    data_last[0] = (data_last[0] - AD_1MIN) * 400 / (AD_1MAX - AD_1MIN);
    data_last[1] = (data_last[1] - AD_2MIN) * 400 / (AD_2MAX - AD_2MIN);
    data_last[3] = (data_last[3] - AD_4MIN) * 400 / (AD_4MAX - AD_4MIN);
    data_last[4] = (data_last[4] - AD_5MIN) * 400 / (AD_5MAX - AD_5MIN);

    // 计算左值和右值时,给第二个和第四个电感 乘以 比例系数 
    left_value  = sqrt(pow(data_last[0], 2) + adjust * pow( data_last[1], 2)); // 左侧电磁传感器计算
    right_value = sqrt(pow(data_last[4], 2) + adjust * pow( data_last[3], 2)); // 右侧电磁传感器计算

    ad_sum = left_value - right_value; // 左边的电磁值 - 右边的电磁值
    rightOrLeft = ad_sum > 0 ? 1:2; // rightOrLeft 表示应该往哪个方向拐弯

		// 差比和加权算法
    offset = (int)100 * ((left_value - right_value) / (left_value + right_value)); // 含竖电感的差值
    offset2 = 100 * (data_last[0] - data_last[4]) / (data_last[0] + data_last[4]); // 计算另一种电磁差值

    // Values:
    // difference1 直线下取值范围: [0 , 0.075]
    // difference2 直线下取值范围: [0.962 , 1]
    // difference2 直线下取值范围: 无参考价值

    // difference1 左转下取值范围: [0 , 0.075]
    // difference2 直线下取值范围: [0.962 , 1]
    // difference2 直线下取值范围: 无参考价值



}

