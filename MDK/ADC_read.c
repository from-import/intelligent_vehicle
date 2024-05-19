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
    // data_last[0] ----- data_last[4] 为第n个电感测到的值

	
    // 电磁数值处理
    left_value = sqrt(pow(data_last[0], 2) + pow(adjust * data_last[1], 2)); // 左侧电磁传感器计算
    right_value = sqrt(pow(data_last[4], 2) + pow(adjust * data_last[3], 2)); // 右侧电磁传感器计算
    offset = (int)100 * ((left_value - right_value) / (left_value + right_value)); // 计算电磁差值
    offset2 = 100 * (data_last[0] - data_last[4]) / (data_last[0] + data_last[4]); // 计算另一种电磁差值

    difference1 = abs(data_last[0] - data_last[4])/data_last[2]	
    difference2 = (data_last[2] - 0.5 * (data_last[0] - data_last[4]))/data_last[2]
    difference3 = (data_last[1] - data_last[3]) / (data_last[1] + data_last[3])

    // offset 为 01 的电磁值和 与 34 的电磁值和 比值
    // offset2 为 0 的电磁值和 4 的电磁值 比值
}
