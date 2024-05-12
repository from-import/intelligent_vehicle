#include "headfile.h"
#include "Initial.h"

//Button����
double Standard = 1;
int adjust = 5;
int Statu = 0;
//Battery����
uint16 ad_result = 0;           //���ŵ�ѹ
uint16 battery_voltage = 0;     //��ص�ѹ
uint32 temp;

void Battery()
{
	ad_result = adc_once(BAT_VOL_PIN, ADC_12BIT);
		temp = (((uint32)ad_result * 5000) / 4096);  //�������ǰadc���ŵĵ�ѹ ���㹫ʽΪ ad_result*VCC/ADC�ֱ���    VCC��λΪmv
		battery_voltage =  temp * 3;//�������ŵ�ѹ  �ͷ�ѹ�������ֵ�����ص�ѹ ���㹫˾Ϊ   ���ŵ�ѹ*(R2+R3)/R3   R3Ϊ�ӵض˵���
	
}
	

// Button pressed :  First == 0


void Button()
{
	//У׼��ֵ�ͺ�����ֵ
	if(First == 0)
	{
		Standard = 300/data_last[2];
		//���ź�����ʵ����Ŀ��ֵ
		//adjust = (data_last[0]+data_last[4])/(data_last[1]+data_last[3]);
	}
	if(Second == 0)
	{Statu = 1;}	
	
	if (demo1 == 0){
		speed_R = 10;
		speed_L = 10;
	}
	if (demo1 == 1){
		speed_R = 25;
		speed_L = 25;
	}

}