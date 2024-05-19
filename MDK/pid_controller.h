#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

typedef struct {
    float kp;  // Proportional gain
    float ki;  // Integral gain
    float kd;  // Derivative gain
    float prev_error; // Previous error
    float integral;    // Integral term
} PIDController;

void initPID(PIDController *pid, float kp, float ki, float kd);
float calculatePID(PIDController *pid, float setpoint, float process_variable);

#endif /* PID_CONTROLLER_H */
