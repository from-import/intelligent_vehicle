#include "headfile.h"
#include "Initial.h"

// Button参数
double Standard = 1;
int adjust = 1;
int Statu = 0;
// Battery参数
uint16 ad_result = 0;           // 引脚电压
uint16 battery_voltage = 0;     // 电池电压
uint32 temp;

void Battery()
{
    ad_result = adc_once(BAT_VOL_PIN, ADC_12BIT);
    temp = (((uint32)ad_result * 5000) / 4096);  // 计算出当前adc引脚的电压，计算公式为 ad_result * VCC / ADC分辨率，VCC单位为mv
    battery_voltage = temp * 3;  // 根据引脚电压和分压电阻的阻值计算电池电压，计算公式为引脚电压 * (R2 + R3) / R3，R3为接地端电阻
}

void Button()
{
    // 校准中值和横竖比值
    if (First == 0)
    {
		Statu = 2;
    }
		// 电机启动
    if (Second == 0)
    {
        Statu = 1;
    }		
}
