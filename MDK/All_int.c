#include "headfile.h"
#include "Initial.h"

void All_int()
{
    // 编码器初始化
    ctimer_count_init(SPEEDL_PLUSE);    // 初始化定时器0作为外部计数
    ctimer_count_init(SPEEDR_PLUSE);    // 初始化定时器3作为外部计数
    
    // 电机初始化
    pwm_init(PWM_L, 17000, 0);  // 初始化PWM1  使用P66引脚  初始化频率为17Khz
    pwm_init(PWM_R, 17000, 0);  // 初始化PWM2  使用P62引脚  初始化频率为17Khz
    
    gpio_mode(P6_4, GPO_PP);    // P64引脚设置为推挽输出
    gpio_mode(P6_0, GPO_PP);    // P60引脚设置为推挽输出
    
    // 屏幕初始化
    lcd_init();                 // 初始化1.8寸TFT屏幕
    
    // Adc_read初始化
    adc_init(ADC_P00, 0);       // P00引脚        
    adc_init(ADC_P01, 0);       // P01引脚        
    adc_init(ADC_P05, 0);       // P05引脚        
    adc_init(ADC_P06, 0);       // P06引脚        
    adc_init(ADC_P03, 0);       // P03引脚    
    
    // 陀螺仪初始化
    // mpu6050_init();
    
    // 电池初始化
    adc_init(BAT_VOL_PIN, ADC_SYSclk_DIV_32);
    
    // 红外测距模块初始化
    // dl1b_init();
}
