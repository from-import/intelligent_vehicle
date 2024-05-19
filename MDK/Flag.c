#include "headfile.h"
#include "Initial.h"

// 逻辑变量
int Type = 0;
int multiple = 1;
int delay_circle = 0;
int delay_avoiding = 0;
int Chance = 0;
int Demo = 0;

void circle()
{
    // delay_circle，延迟入环标记
    if (Type == 2)
    {
        delay_circle = 1;
        Record_Dis = 1;
        Demo = 1;
    }
    if (Type == 3)
    {
        delay_circle = 2;
        Record_Dis = 1;
        Demo = 1;
    }
    // 入环第二阶段进环
    if (Demo == 1 && Distance > 80)
    {
        Demo = 2;
        Record_Dis = 0;
        Yaw = 0;
    }
    // 入环第三阶段，预出环
    if (Demo == 2 && abs(Yaw) > 300)
    {
        Demo = 3;
        Record_Dis = 1;
        Distance = 0;
    }
    // 入环第三阶段，出环
    if (Demo == 3 && Distance > 100)
    {
        Type = 0;
        delay_circle = 0;
        Record_Dis = 0;
    }
}


/*****************************************************************
Tips:   Type = 0 为直线状态
        Type = 1 为转弯状态
        Type = 2 为R入环状态
        Type = 3 为L入环状态
        Type = 4 为避障状态
*****************************************************************/	

void Flag(){

    if (difference1>0.2){
     // 转弯（在转弯条件下0会出现700+的值）
        Type = 1;
        multiple = 1;
    }
    else{
        Type = 0;
    }
    
    // 入环
    circle();

    // 避障判定 ,delay_avoiding，避障标志
    if (dl1b_distance_mm < 620 && Chance == 1){
        Type = 4;
    }
    if (Type == 4){
        delay_avoiding = 1;
        Record_Dis = 1;
    }
    else if (delay_avoiding == 1 && Distance > 290){
        delay_avoiding = 0;
        Record_Dis = 0;
        Type = 0;
        Chance = 0;
    }
    
    // delay_circle，延迟入环标记
    if (Type == 2 || Type == 3)
    {
        delay_circle = 1;
        Record_Dis = 1;
    }
    // 退出入环
    if (delay_circle == 1 && Distance > 50)
    {
        delay_circle = 2;
        Record_Dis = 0;
    }
    

}
