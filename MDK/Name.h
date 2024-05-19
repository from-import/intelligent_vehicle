#ifndef NAME_H_
#define NAME_H_

//定义编码器脉冲引脚
#define SPEEDL_PLUSE   CTIM0_P34
#define SPEEDR_PLUSE   CTIM3_P04
//定义编码器方向引脚
#define SPEEDL_DIR     P35
#define SPEEDR_DIR     P53
//电机方向
#define DIR_L P64
#define DIR_R P60
//电机PWM
#define PWM_L PWMA_CH4P_P66
#define PWM_R PWMA_CH2P_P62
//按钮名
#define First P72
#define Second P71
//电池
#define BAT_VOL_PIN ADC_P15
//拨码开关
#define demo1 P75
#define demo2 P76

//蜂鸣器
#define BEEP P67

#endif