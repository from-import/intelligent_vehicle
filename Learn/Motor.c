#include "headfile.h"
#include "myfile.h"

//�������ɼ���������ֵ
int16 templ_pluse = 0;
int16 tempr_pluse = 0;
uint8 lost_flag = 0;
//�����ٶ�
int16 exp_speed_r = 0;
int16 exp_speed_l = 0;
int base = 150;
int base_fork = 140, base_straight = 130, base_bend = 150, base_loop = 140, base_ramp = 110; //����ֱ���������Բ������
//������ʵ�ʵ����
float L_err[3] = {0, 0, 0};
float R_err[3] = {0, 0, 0};
//���pid����
// float KP_L=100,KD_L=137,KI_L=41;
// float KP_R=100,KD_R=137,KI_R=41;
// float KP_L = 53, KD_L = 41, KI_L = 20.5;
// float KP_R = 53, KD_R = 41, KI_R = 20.5;
float KP_L = 80, KD_L = 139, KI_L = 48;
float KP_R = 80, KD_R = 139, KI_R = 48;
//������ռ�ձ�
int32 L_duty = 0, R_duty = 0;
int16 L_sum = 0;
int16 R_sum = 0;
int flag_0 = 0;
//ϵ��
int k = 14;
//ʵ���ٶ�
int16 L_speed, R_speed;
//
int8 number = 1;
/*******************************************************************************
 * �� �� ��       :motor_control()
 * ��������		   : �����������
 * ��    ��       : ��
 * ��    ��    	 : ��
 *******************************************************************************/
void motor_control()
{
	if (P36 == 0)
	{
		delay_ms(20);
		if (P36 == 0)
		{
			delay_ms(500);
			flag_0 = 1;
		}
	}

	if (flag_0 == 1)
	{
		if (L_duty > 0)
		{
			pwm_duty(PWMA_CH3P_P64, 0);
			pwm_duty(PWMA_CH4P_P66, (uint32)L_duty); //������ת
		}
		else if (L_duty < 0)
		{
			pwm_duty(PWMA_CH3P_P64, (uint32)(-L_duty)); //���Ʒ�ת
			pwm_duty(PWMA_CH4P_P66, 0);
		}
		if (R_duty > 0)
		{
			pwm_duty(PWMA_CH1P_P60, (uint32)(R_duty));
			pwm_duty(PWMA_CH2P_P62, 0);
		}
		else if (R_duty < 0)
		{
			pwm_duty(PWMA_CH2P_P62, (uint32)(-R_duty));
			pwm_duty(PWMA_CH1P_P60, 0);
		}
	}
}
/*******************************************************************************
 * �� �� ��       :encode()
 * ��������		 	 : ���������ٺ���
 * ��    ��       : ��
 * ��    ��    	 : ��
 *******************************************************************************/
void encode()
{
	templ_pluse = ctimer_count_read(SPEEDL_PLUSE);
	tempr_pluse = ctimer_count_read(SPEEDR_PLUSE);

	if (SPEEDL_DIR == 1)
	{
		templ_pluse = -templ_pluse;
	}
	if (SPEEDR_DIR == 1)
	{
		tempr_pluse = -tempr_pluse;
	}

	L_speed = templ_pluse; //�����ٶ�
	R_speed = tempr_pluse; //�����ٶ�
	//����������
	ctimer_count_clean(SPEEDL_PLUSE);
	ctimer_count_clean(SPEEDR_PLUSE);
	road_L += (templ_pluse * 2 * (3.14159265) * 0.032) / 2250;
	road_R -= (tempr_pluse * 2 * (3.14159265) * 0.032) / 2250;
	road_sum = (road_L + road_R) / 2;
}

/*******************************************************************************
 * �� �� ��       :motor_CHA()
 * ��������		   :����
 * ��    ��       : ��
 * ��    ��    	 : ��
 *******************************************************************************/
void different_chaspeed()
{
	if (lost_flag == 0)
	{
		exp_speed_r = base;
		exp_speed_l = -base;
		if (space_flag == 1)
		{
			if (Error_angle_out > 0 && inku == 0)
			{
				// exp_speed_l = exp_speed_l * (float)(Error_angle_out * (-0.005f) + 0.80f); // && space_flag!=1
				// exp_speed_r = base + Error_angle_out * 0.15;
				exp_speed_l = exp_speed_l * (float)(Error_angle_out * (-0.005f) + 0.8f);
				exp_speed_r = base + (int16)(Error_angle_out * 0.10);
			}
			if (Error_angle_out < 0 && inku == 0) //
			{
				// exp_speed_r = exp_speed_r * (float)(Error_angle_out * (0.005f) + 0.80f); //
				// exp_speed_l = -(base + Error_angle_out * 0.15);
				exp_speed_r = exp_speed_r * (float)(Error_angle_out * (0.005f) + 0.8f);
				exp_speed_l = -(base + (int16)(Error_angle_out * 0.10));
			}
		}
		if (Error_angle_out > 0 && three_on == 0 && inku == 0 && space_flag != 1)
		{
			exp_speed_l = exp_speed_l * (float)(Error_angle_out * (-0.006f) + 0.80f); // && space_flag!=1
			exp_speed_r = base + Error_angle_out * 0.15;
			// exp_speed_l = exp_speed_l * (float)(Error_angle_out * (-0.0045f) + 0.99f);
			// exp_speed_r = base + (int16)(Error_angle_out * 0);
		}
		if (Error_angle_out < 0 && three_on == 0 && inku == 0 && space_flag != 1) //
		{
			exp_speed_r = exp_speed_r * (float)(Error_angle_out * (0.006f) + 0.80f); //
			exp_speed_l = -(base + Error_angle_out * 0.15);
			// exp_speed_l = exp_speed_l * (float)(Error_angle_out * (0.0045f) + 0.99f);
			// exp_speed_r = base + (int16)(Error_angle_out * 0);
		}

		if (Error_angle_out > 0 && (inku == 4 || inku == 3) && inku != 5)
		{
			exp_speed_l = exp_speed_l * (float)(Error_angle_out * (-0.008f) + 0.80f);
			exp_speed_r = base;
		}
		if (Error_angle_out < 0 && (inku == 4 || inku == 3) && inku != 5)
		{
			exp_speed_r = exp_speed_r * (float)(Error_angle_out * (0.008f) + 0.80f);
			exp_speed_l = -(base * 1.3);
		}

		//����·��ʱ�Ĳ���
		if (Error_angle_out > 0 && three_on == 1 && inku == 0) //
		{
			exp_speed_l = exp_speed_l * (float)(Error_angle_out * (-0.008f) + 0.75f);
			exp_speed_r = base + (85 - MAD) * 0.1; // Error_angle_out * 0.1
		}
		if (Error_angle_out < 0 && three_on == 1 && inku == 0) //
		{
			exp_speed_r = exp_speed_r * (float)(Error_angle_out * (0.03f) + 0.90f);
			exp_speed_l = -(base + Error_angle_out * 0.3);
		}
	}
}

/*******************************************************************************
 * �� �� ��       :motor_pid()
 * ��������		 : ���pid����
 * ��    ��       : ��
 * ��    ��    	 : ��
 *******************************************************************************/
void motor_pid()
{
	if (MAD <= 6 && ZLAD <= 6 && ZRAD <= 6 && out_ku != 0 && inku == 0 && chuku_time > 400)
	{
		if ((three_on == 1 || three_in_L == 1 && three_in_R == 1) && fork_time < 200)
			lost_flag = 0;
		else
		{
			lost_flag = 1;
		}
	}

	if (lost_flag == 0)
	{
		different_chaspeed();
	}

	if (lost_flag == 1) 
	{
		base = 0;
		exp_speed_r = 0;
		exp_speed_l = 0;
	}

	//�������
	L_err[2] = L_err[1];
	L_err[1] = L_err[0];
	L_err[0] = exp_speed_r - L_speed;
	//��������ۼ�
	L_sum += L_err[0];

	//�������
	R_err[2] = R_err[1];
	R_err[1] = R_err[0];
	R_err[0] = exp_speed_l - R_speed;
	//��������ۼ�
	R_sum += R_err[0];
	//�����޷�
	if (L_sum > A) // L_sum>1000 L_sum=1000
		L_sum = A;
	if (L_sum < -A)
		L_sum = -A;
	//�����޷�
	if (R_sum > A) // L_sum>1000 L_sum=1000
		R_sum = A;
	if (R_sum < -A)
		R_sum = -A;

	L_duty = (int32)(KP_L * L_err[0] + KI_L * L_sum + KD_L * (L_err[0] - L_err[1]));
	R_duty = (int32)(KP_R * R_err[0] + KI_R * R_sum + KD_R * (R_err[0] - R_err[1]));

	if (L_duty > C)
		L_duty = C;
	if (L_duty < -C)
		L_duty = -C;

	if (R_duty > C)
		R_duty = C;
	if (R_duty < -C)
		R_duty = -C;
}

/*******************************************************************************
 * �� �� ��       :huoer()
 * ��������		   : ����д�ĺ�ţ�ƵĻ������
 * ��    ��       : ��
 * ��    ��    	 : ��
 *******************************************************************************/
void huoer()
{
	if (HALL_PIN == 0 && number < 24 && out_ku == 1) //&& hall_detection_count<3
	{												 //&& (cha_num == 2 || SLhuan_num == 2 || SRhuan_num == 2)
		number += 1;
		if (number >= 24)
		{
			number = 0;
		}
	}
	if (number >= 5)
	{
		out_ku = 2;
		BEEP = 1;
	}
}


static int16 Ackerman = 380;//��ԽС������Խ�����Ҫ����410����С��Ҫ180
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 if (turn >= 0)
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 {
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 L_Motor.exp_speed = (int)(L_except-PWM_decrease) *(1.0-1.0*(tan((float)turn*10*PI/Ackerman)*0.396));
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 R_Motor.exp_speed = (int)(R_except-PWM_decrease) *(1.0+0*(tan((float)turn*10*PI/Ackerman)*0.396));
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2// if(L_Motor.exp_speed<=110)L_Motor.exp_speed=110;
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 }
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 else if (turn <0) //����
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 {
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 L_Motor.exp_speed = (int)(L_except-PWM_decrease) *(1.0-0*(tan((float)turn*10*PI/Ackerman)*0.396));
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 R_Motor.exp_speed = (int)(R_except-PWM_decrease) *(1.0+1.0*(tan((float)turn*10*PI/Ackerman)*0.396));
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 //if(R_Motor.exp_speed<=110)L_Motor.exp_speed=110;
�0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 �0�2 }