#include "headfile.h"
#include "Initial.h"

void All_int()
{
	//��������ʼ��
	ctimer_count_init(SPEEDL_PLUSE);	//��ʼ����ʱ��0��Ϊ�ⲿ����
	ctimer_count_init(SPEEDR_PLUSE);	//��ʼ����ʱ��3��Ϊ�ⲿ����
	//�����ʼ��
	pwm_init(PWM_L, 17000, 0); 	//��ʼ��PWM1  ʹ��P66����  ��ʼ��Ƶ��Ϊ17Khz
	pwm_init(PWM_R, 17000, 0); 	//��ʼ��PWM2  ʹ��P62����  ��ʼ��Ƶ��Ϊ17Khz
	
	gpio_mode(P6_4, GPO_PP);	// P64��������Ϊ�������
	gpio_mode(P6_0, GPO_PP);	// P60��������Ϊ�������
	//��Ļ��ʼ��
	lcd_init();			//��ʼ��1.8��TFT��Ļ
	//Adc_read��ʼ��
	adc_init(ADC_P00,0);						//P00����		
	adc_init(ADC_P01,0);						//P01����		
	adc_init(ADC_P05,0);						//P05����		
	adc_init(ADC_P06,0);						//P06����		
	adc_init(ADC_P03,0);						//P03����	
	//�����ǳ�ʼ��
	 //mpu6050_init();
	//��س�ʼ��
	 adc_init(BAT_VOL_PIN, ADC_SYSclk_DIV_32);
	//������ģ���ʼ��
	 //dl1b_init();
	
}