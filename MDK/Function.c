#include "headfile.h"
#include "Initial.h"

// 初始化PID控制器
void initPID(PIDController *pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->prev_error = 0;
    pid->integral = 0;
}

// 计算PID输出
float calculatePID(PIDController *pid, float setpoint, float process_variable) {
    float error = setpoint - process_variable;

    // 比例项
    float p_term = pid->kp * error;

    // 积分项
    pid->integral += error;
    float i_term = pid->ki * pid->integral;

    // 微分项
    float d_term = pid->kd * (error - pid->prev_error);

    // PID输出总和
    float output = p_term + i_term + d_term;

    // 更新前一次误差
    pid->prev_error = error;

    return output;
}

// int main() {
//     PIDController pid;
//     float setpoint = 100;  // 设定值
//     float process_variable = 0;  // 过程变量
//     float output;

//     // 设置PID参数
//     initPID(&pid, 0.5, 0.1, 0.2);

//     // 模拟控制过程
//     for (int i = 0; i < 100; ++i) {
//         // 在实际应用中，process_variable 应该是从传感器读取的值
//         // 这里简单地模拟为每次增加1
//         process_variable += 1;

//         // 计算PID输出
//         output = calculatePID(&pid, setpoint, process_variable);

//         // 在实际应用中，将输出发送到执行器进行控制
//         // 这里简单地打印输出值
//         printf("输出: %f\n", output);
//     }

//     return 0;
// }
