#include "headfile.h"
#include "Initial.h"

float data_last[5];   //????
float Data[5][10] = {0};      //???????	
int i=0,j,k;
float data_s[5] ={0,0,0,0,0};
int offset;          //?????????
int offset2;
float right_value,left_value;

void ADC_read()
{
	/*****************************************************************
	Tips:?????????????????
	*****************************************************************/		
		 Data[0][9]=Standard*adc_once(ADC_P00, ADC_12BIT);//00
	   Data[1][9]=Standard*adc_once(ADC_P01, ADC_12BIT);//01
 	   Data[2][9]=Standard*adc_once(ADC_P05, ADC_12BIT);//05
     Data[3][9]=Standard*adc_once(ADC_P06, ADC_12BIT);//06
		 Data[4][9]=Standard*adc_once(ADC_P03, ADC_12BIT);//03
	
	/*****************************************************************
	Tips:??????????????????
	*****************************************************************/	
	
	
	
	
	// upgrade in future
	/*
	Data :
	
				0 1 2 3 4 5 6 7 8 9
			0 
			1 
			2 
			3 
			4
			
			Function: Save Value
			time0 [0][9] -> [0][8] -> [0][0]
			
	*/
						
		for(j=0;j<5;j++) 
	{
			for(i=0;i<10;i++)
		{data_s[j] += Data[j][i];}
		data_last[j] = data_s[j]/10.0; 
		data_s[j]=0;
			for(k=0;k<9;k++)
		{
			Data[j][k] = Data[j][k+1];
		}
	}
	
	/*****************************************************************
	Tips:??????????????????????
	*****************************************************************/
	
	// left:  value0 + value1
	left_value = sqrt(pow(data_last[0],2)+pow(adjust*data_last[1],2));
	
	// right: value3 + value4
	right_value = sqrt(pow(data_last[4],2)+pow(adjust*(data_last[3]),2));
	
	// offset : Offset
	offset=(int)100*((left_value-right_value)/(left_value+right_value));//???????
	offset2 = 100*(data_last[0]-data_last[4])/(data_last[0]+data_last[4]);	

}