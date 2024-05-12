#include "headfile.h"
#include "Initial.h"

// *************************** ����Ӳ������˵�� ***************************
// 		���� DL1B ģ��
//      ģ��ܽ�            ��Ƭ���ܽ�
//      SCL                 �鿴 SEEKFREE_DL1B.h �� DL1B_SCL_PIN  	�궨�� Ĭ�� P65
//      SDA                 �鿴 SEEKFREE_DL1B.h �� DL1B_SDA_PIN  	�궨�� Ĭ�� P63
//      XS                  �鿴 SEEKFREE_DL1B.h �� DL1B_XS_PIN  	�궨�� Ĭ�� P47
//      VCC                 5V ��Դ
//      GND                 ��Դ��
// *************************** ����Ӳ������˵�� ***************************

uint8  pit_ms_flag          = 0;
uint16 pit_ms_count         = 0;



void main()
{
	board_init();			// ��ʼ���ڲ��Ĵ�������ɾ���˾���롣	
	All_int();
	pit_timer_ms(TIM_4, 5);

	
    while(1)
	{
    Show();				
		Battery();
		//��ȡ���ٶȼ����� 
			mpu6050_get_gyro();
		// 1000ms��ӡһ������
        if(pit_ms_flag)
        {
            printf("\r\nDL1B distance data: %5d", dl1b_distance_mm);
            pit_ms_flag = 0;
		      //��Ļ��ʾ	  
//					Show();				  
        }
				
    }
}


void Time()
{
	pit_ms_count = (pit_ms_count == 995) ? (0) : (pit_ms_count + 5);            // 1000ms ���ڼ���
    if(0 == pit_ms_count % 1000)
    {
        pit_ms_flag = 1;
    }
}