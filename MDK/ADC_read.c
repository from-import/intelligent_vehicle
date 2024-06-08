#include "headfile.h"
#include "Initial.h"

float data_last[5];   //电磁值
float Data[5][10] = {0};      //读取电磁值	
int i=0,j,k;
float data_s[5] ={0,0,0,0,0};
int offset;          //电磁差值计算
int offset2;
float right_value,left_value;

//归一化算法最大最小值存储
int AD_1MAX = 580;
int AD_2MAX = 80;
int AD_4MAX = 60;
int AD_5MAX = 691;

#define AD_1MIN 0             // data_last[0]最小电感值
#define AD_5MIN 0             // data_last[4]最小电感值

#define AD_2MIN 0             // data_last[1]最小电感值
#define AD_4MIN 0           // data_last[3]最小电感值

//十字修正函数（off）
void Crossing(){
	offset2 = data_last[1] > data_last[3] ? offset2-15:offset2+15;
}

void ADC_read()
{
	/*****************************************************************
	Tips:以下部分为电磁读取程序
	*****************************************************************/		
		 Data[0][9]=adc_once(ADC_P00, ADC_12BIT);//00
	   Data[1][9]=adc_once(ADC_P01, ADC_12BIT);//01
 	   Data[2][9]=adc_once(ADC_P05, ADC_12BIT);//05
     Data[3][9]=adc_once(ADC_P06, ADC_12BIT);//06
		 Data[4][9]=adc_once(ADC_P03, ADC_12BIT);//03
	
	/*****************************************************************
	Tips:以下部分为滑动平均滤波
	*****************************************************************/	
		for(j=0;j<5;j++){
				for(i=0;i<10;i++){
						data_s[j] += Data[j][i];
				}
				
				data_last[j] = data_s[j]/10.0; 
				data_s[j]=0;
			
				for(k=0;k<9;k++){
						Data[j][k] = Data[j][k+1];
				}
		}
	/*****************************************************************
	Tips:以下部分为电磁归一化部分
	*****************************************************************/
	//更新max库
    if(Statu == 10){
			AD_1MAX = AD_1MAX > data_last[0] ? AD_1MAX:data_last[0];
			AD_5MAX = AD_5MAX > data_last[4] ? AD_5MAX:data_last[4];
			AD_2MAX = AD_2MAX > data_last[1] ? AD_2MAX:data_last[1];
			AD_4MAX = AD_4MAX > data_last[3] ? AD_4MAX:data_last[3];
		}
		
		data_last[0] = 400 *(data_last[0] - AD_1MIN)  / (AD_1MAX - AD_1MIN);
    data_last[1] = 400 *(data_last[1] - AD_2MIN)  / (AD_2MAX - AD_2MIN);
    data_last[3] = 400 *(data_last[3] - AD_4MIN)  / (AD_4MAX - AD_4MIN);
    data_last[4] = 400 *(data_last[4] - AD_5MIN)  / (AD_5MAX - AD_5MIN);


	
	/*****************************************************************
	Tips:以下部分为电磁数值处理部分
	*****************************************************************/
	 // 计算左值和右值时,给第二个和第四个电感 乘以 比例系数 
		left_value = sqrt(pow(data_last[0],2)+pow(adjust*data_last[1],2));
		right_value = sqrt(pow(data_last[4],2)+pow(adjust*(data_last[3]),2));
	
	// 差比和加权算法
		offset=(int)100*((left_value-right_value)/(left_value+right_value));
		offset2 = 100*(data_last[0]-data_last[4])/(data_last[0]+data_last[4]);	
		
				
//		//十字修正函数
//	if(Type == 5){
//			Crossing();
//	}

}