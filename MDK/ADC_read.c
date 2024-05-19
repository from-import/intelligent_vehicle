#include "headfile.h"
#include "Initial.h"

float data_last[5];   // 电磁值
float Data[5][10] = {0}; // 读取电磁值	
int i = 0, j, k;
float data_s[5] = {0, 0, 0, 0, 0};
int offset;          // 电磁差值计算
int offset2;
float right_value, left_value;

void ADC_read()
{
    /*****************************************************************
    Tips: 以下部分为电磁读取程序
    *****************************************************************/		
    Data[0][9] = Standard * adc_once(ADC_P00, ADC_12BIT); // 00
    Data[1][9] = Standard * adc_once(ADC_P01, ADC_12BIT); // 01
    Data[2][9] = Standard * adc_once(ADC_P05, ADC_12BIT); // 05
    Data[3][9] = Standard * adc_once(ADC_P06, ADC_12BIT); // 06
    Data[4][9] = Standard * adc_once(ADC_P03, ADC_12BIT); // 03
	
    /*****************************************************************
    Tips: 以下部分为滑动平均滤波
    *****************************************************************/	
    for (j = 0; j<5; j++) 
    {
        for (i = 0; i<10; i++)
        {
            data_s[j] += Data[j][i];
        }
        data_last[j] = data_s[j] / 10.0; 
        data_s[j] = 0;
        for (k = 0; k<9; k++)
        {
            Data[j][k] = Data[j][k+1];
        }
    }
	
    /*****************************************************************
    Tips: 以下部分为电磁数值处理部分
    *****************************************************************/
    left_value = sqrt(pow(data_last[0], 2) + pow(adjust * data_last[1], 2));
    right_value = sqrt(pow(data_last[4], 2) + pow(adjust * data_last[3], 2));
    offset = (int)100 * ((left_value - right_value) / (left_value + right_value)); // 差比和差加权
    offset2 = 100 * (data_last[0] - data_last[4]) / (data_last[0] + data_last[4]);	
}
