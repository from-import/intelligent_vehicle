import numpy as np

# 常量定义
NUM_L = 6
NUM_POS = 50
CCD_W = 128
THRESHOLD1 = 100  # 二值化阈值1
THRESHOLD2 = 100  # 二值化阈值2
Servo_Center = 1500  # 舵机中值

# 全局变量定义
stop_flag = 1
jump = 0
black_L = 0
black_R = 0
array_L = [0] * NUM_L  # 左边沿跳变个数存储
R_Pos_Store = [0] * NUM_POS  # 右边线位置存储
L_Pos_Store = [0] * NUM_POS  # 左边线位置存储

# 速度
TargetSpeed_L = 0
TargetSpeed_R = 0
TargetSpeed = 0
M_error = 0
Encoder1 = 0
Encoder2 = 0

# 电机PID
L_Velocity_KP = 2.3
L_Velocity_KI = 0.1
L_Velocity_KD = 1.5
R_Velocity_KP = 2.3
R_Velocity_KI = 0.1
R_Velocity_KD = 1.5
Velocity_Add = 0

# 舵机PID
Servo_Kp = 2.1
Servo_Ki = 0
Servo_Kd = 0.02

# PWM
Servo_Duty = Servo_Center
Motor1_Duty = 0
Motor2_Duty = 0

# CCD
CCD1_ERR, CCD2_ERR, CCD_ERR = 0, 0, 0
Last_CCD1_ERR, Last_CCD2_ERR, Last_CCD_ERR = 0, 0, 0
CCD1_W, CCD2_W = 0, 0
ccd_flag = 2  # 0(CCD1); 1(CCD2); 2(CCD1&CCD2)
ratio_1_2 = 0  # 两个CCD图像数据使用比例

# 环岛
In_L = 0  # 进入环岛左边线是否丢失标志
Out = 0  # 出环岛标志位
isL = 0
isR = 0

L_Pos = 0
R_Pos = 0

State = 0
Width2 = 82

key_flag = 6  # 按键标志位,调参用


def timer0_int():
    global CCD1_ERR, CCD2_ERR, CCD_ERR, Last_CCD1_ERR, Last_CCD2_ERR, Last_CCD_ERR
    global Servo_Duty, TargetSpeed_L, TargetSpeed_R, Encoder1, Encoder2
    global Motor1_Duty, Motor2_Duty, ccd_flag, ratio_1_2, stop_flag

    # 模拟采集CCD数据
    CCD1_Data = np.random.randint(0, 256, CCD_W)
    CCD2_Data = np.random.randint(0, 256, CCD_W)

    # CCD二值化
    Bin1_CCD = ccd_binaryzation(THRESHOLD1, CCD1_Data)
    Bin2_CCD = ccd_binaryzation(THRESHOLD2, CCD2_Data)

    # CCD数据滤波
    Bin1_CCD = bin_ccd_filter(Bin1_CCD)
    Bin2_CCD = bin_ccd_filter(Bin2_CCD)

    # 计算左右边线位置
    isL = is_L_exist(Bin2_CCD)
    isR = is_R_exist(Bin2_CCD)
    if isL == 1:
        L_Pos = l_location(Bin2_CCD)
    line_shift(L_Pos_Store, NUM_POS, L_Pos)

    if isR == 1:
        R_Pos = r_location(Bin2_CCD)
    line_shift(R_Pos_Store, NUM_POS, R_Pos)

    # 计算赛道宽度
    CCD1_W = np.sum(Bin1_CCD)
    CCD2_W = np.sum(Bin2_CCD)

    # 环岛状态判断
    State = switch_cir(L_Pos_Store, R_Pos_Store, CCD2_W, isR, isL)
    if State == 1:
        CCD1_ERR = ccd_get_error(Bin1_CCD)
        if CCD1_ERR == 200:
            CCD1_ERR = Last_CCD1_ERR
        CCD2_ERR = ccd_get_error(Bin2_CCD)
        if CCD2_ERR == 200:
            CCD2_ERR = Last_CCD2_ERR
    elif State == 2:
        CCD2_ERR = compens_l_in(R_Pos, Width2)
    elif State == 3:
        CCD2_ERR = compens_r_in(L_Pos, Width2)
    elif State == 4:
        CCD2_ERR = ccd_get_error(Bin2_CCD)
        if CCD2_ERR == 200:
            CCD2_ERR = Last_CCD2_ERR
    elif State == 5:
        CCD2_ERR = -50
    elif State == 6:
        CCD2_ERR = compens_l_in(R_Pos, Width2)
    elif State == 7:
        CCD1_ERR = ccd_get_error(Bin1_CCD)
        if CCD1_ERR == 200:
            CCD1_ERR = Last_CCD1_ERR
        CCD2_ERR = ccd_get_error(Bin2_CCD)
        if CCD2_ERR == 200:
            CCD2_ERR = Last_CCD2_ERR
    elif State == 8:
        CCD1_ERR = ccd_get_error(Bin1_CCD)
        if CCD1_ERR == 200:
            CCD1_ERR = Last_CCD1_ERR
        CCD2_ERR = ccd_get_error(Bin2_CCD)
        if CCD2_ERR == 200:
            CCD2_ERR = Last_CCD2_ERR

    if CCD2_W >= 115:
        ratio_1_2 = 100
    else:
        ratio_1_2 = 0

    if CCD2_W > 90 or CCD2_W < 72:
        Servo_Kp = 3.1
    else:
        Servo_Kp = 2.1

    CCD_ERR = ((ratio_1_2 * CCD1_ERR) + ((100 - ratio_1_2) * CCD2_ERR)) / 100
    if ccd_flag == 0:
        Servo_Duty = Servo_Center - int(CCD1_ERR * Servo_Kp) - int((CCD1_ERR - Last_CCD1_ERR) * Servo_Kd)
    elif ccd_flag == 1:
        Servo_Duty = Servo_Center - int(CCD2_ERR * Servo_Kp) - int((CCD2_ERR - Last_CCD2_ERR) * Servo_Kd)
    else:
        Servo_Duty = Servo_Center - int(CCD_ERR * Servo_Kp) - int((CCD_ERR - Last_CCD_ERR) * Servo_Kd)

    Last_CCD1_ERR = CCD1_ERR
    Last_CCD2_ERR = CCD2_ERR
    Last_CCD_ERR = CCD_ERR

    if stop_flag == 0:
        TargetSpeed_L = 0
        TargetSpeed_R = 0

    # 模拟舵机控制
    servo_ctrl(Servo_Duty)

    # 模拟电机速度控制
    if Servo_Duty >= 1500 and Servo_Duty <= 1563:
        TargetSpeed_L = 660
        TargetSpeed_R = 660
    elif Servo_Duty > 1563:
        if Servo_Duty < 1630:
            TargetSpeed_L = 300
            TargetSpeed_R = 450
        elif Servo_Duty > 1630 and Servo_Duty < 1690:
            TargetSpeed_L = 250
            TargetSpeed_R = 420
        else:
            TargetSpeed_L = 240
            TargetSpeed_R = 400
    else:
        if Servo_Duty > 1462:
            TargetSpeed_L = 450
            TargetSpeed_R = 300
        elif Servo_Duty < 1450 and Servo_Duty > 1400:
            TargetSpeed_L = 420
            TargetSpeed_R = 250
        else:
            TargetSpeed_L = 400
            TargetSpeed_R = 240

    if stop_flag == 1:
        TargetSpeed_L = 0
        TargetSpeed_R = 0

    Encoder1 = -read_encoder(1)
    Encoder2 = read_encoder(2)
    Motor1_Duty = velocity_control_l(Encoder1)
    Motor2_Duty = velocity_control_r(Encoder2)

    # 模拟电机控制
    motor_ctrl(300, 300)


def ccd_binaryzation(threshold, ccd_data):
    """将CCD数据进行二值化处理"""
    return [1 if x >= threshold else 0 for x in ccd_data]


def bin_ccd_filter(bin_ccd):
    """对二值化后的CCD数据进行滤波"""
    filtered = bin_ccd.copy()
    for i in range(1, CCD_W - 1):
        if bin_ccd[i] == 1 and bin_ccd[i - 1] == 0 and bin_ccd[i + 1] == 0:
            filtered[i] = 0
        elif bin_ccd[i] == 0 and bin_ccd[i - 1] == 1 and bin_ccd[i + 1] == 1:
            filtered[i] = 1
    return filtered


def ccd_get_error(bin_ccd):
    """根据二值化后的CCD数据计算赛道偏差"""
    left, right = -1, -1
    for i in range(63, -1, -1):
        if bin_ccd[i] == 1 and bin_ccd[i + 1] == 0:
            left = i
            break
    for i in range(64, CCD_W):
        if bin_ccd[i] == 1 and bin_ccd[i - 1] == 0:
            right = i
            break
    if left == -1 or right == -1:
        return 200  # 表示两边都丢线
    return (left + right - 128)


def is_L_exist(bin_ccd):
    """检查左边线是否存在"""
    for i in range(1, 58):
        if (bin_ccd[64 - i] + bin_ccd[64 - i - 1] + bin_ccd[64 - i - 2] == 3) and (bin_ccd[64 - i - 3] + bin_ccd[64 - i - 4] + bin_ccd[64 - i - 5] == 0):
            return 1
    return 0


def is_R_exist(bin_ccd):
    """检查右边线是否存在"""
    for i in range(1, 58):
        if (bin_ccd[64 + i] + bin_ccd[64 + i + 1] + bin_ccd[64 + i + 2] == 3) and (bin_ccd[64 + i + 3] + bin_ccd[64 + i + 4] + bin_ccd[64 + i + 5] == 0):
            return 1
    return 0


def l_location(bin_ccd):
    """找到左边线位置"""
    for i in range(64, 2, -1):
        if (bin_ccd[i - 1] + bin_ccd[i - 2] + bin_ccd[i - 3] == 0) and (bin_ccd[i + 1] + bin_ccd[i + 2] + bin_ccd[i + 3] == 3):
            return i
    return 3


def r_location(bin_ccd):
    """找到右边线位置"""
    for i in range(64, 125):
        if (bin_ccd[i + 1] + bin_ccd[i + 2] + bin_ccd[i + 3] == 0) and (bin_ccd[i - 1] + bin_ccd[i - 2] + bin_ccd[i - 3] == 3):
            return i
    return 125


def line_shift(arr, N, pos):
    """数组右移一位"""
    for i in range(N - 1, 0, -1):
        arr[i] = arr[i - 1]
    arr[0] = pos


def switch_cir(L_Pos_Store, R_Pos_Store, CCD2_W, isR, isL):
    """判断进入环岛过程"""
    global In_L, Out

    if (isR == 1) and (isL == 0) and (Out == 0):
        if (abs(R_Pos_Store[NUM_POS - 1] - R_Pos_Store[0]) <= 10) and (R_Pos_Store[NUM_POS - 1] > 0):
            In_L = 1
            return 2
        else:
            return 1
    elif (In_L == 1) and ((L_Pos_Store[NUM_POS - 1] - L_Pos_Store[0]) > 1) and (CCD2_W > 87) and (Out == 0):
        In_L = 2
        return 3
    elif (In_L == 2) and (isR == 0) and ((abs(L_Pos_Store[NUM_POS - 1] - L_Pos_Store[0]) <= 2) or (isL == 0)):
        return 3
    elif (In_L == 2) and (CCD2_W < 90) and (Out == 0) and (isR == 1):
        Out = 1
        return 4
    elif (Out == 1) and (isR == 0) and (In_L == 2):
        Out = 2
        return 5
    elif (isR == 1) and (((L_Pos_Store[0] - L_Pos_Store[NUM_POS - 1]) < 0) or (isL == 0)) and (Out == 2):
        Out = 3
        return 6
    elif (Out == 3) and (isR == 1) and (isL == 1):
        Out = 0
        In_L = 0
        return 7
    else:
        return 8


def compens_l_in(R_Pos, CCDx_W):
    """进入环岛补线程序，根据右边线补左边线"""
    return R_Pos - CCDx_W + R_Pos - 128


def compens_r_in(L_Pos, CCDx_W):
    """进入环岛补线程序，根据左边线补右边线"""
    return L_Pos + L_Pos + CCDx_W - 128 - 50


def servo_ctrl(servo_duty):
    """模拟舵机控制"""
    print(f"Servo control with duty: {servo_duty}")


def read_encoder(encoder_id):
    """模拟读取编码器值"""
    return np.random.randint(-1000, 1000)


def motor_ctrl(duty1, duty2):
    """模拟电机控制"""
    print(f"Motor control with duty1: {duty1}, duty2: {duty2}")


def velocity_control_l(encoder):
    """左电机速度PI控制"""
    global TargetSpeed_L, L_Velocity_KP, L_Velocity_KI
    static_Encoder = 0
    static_Encoder_Integral = 0

    Encoder_Least = TargetSpeed_L - encoder

    static_Encoder *= 0.7
    static_Encoder += Encoder_Least * 0.3

    static_Encoder_Integral += Encoder_Least
    static_Encoder_Integral = max(min(static_Encoder_Integral, 10000), -10000)

    Velocity = static_Encoder * L_Velocity_KP + static_Encoder_Integral * L_Velocity_KI
    if TargetSpeed_L == 0:
        static_Encoder_Integral = 0

    Velocity_int = int(Velocity)
    return max(min(Velocity_int, 1200), -1200)


def velocity_control_r(encoder):
    """右电机速度PI控制"""
    global TargetSpeed_R, R_Velocity_KP, R_Velocity_KI
    static_Encoder = 0
    static_Encoder_Integral = 0

    Encoder_Least = TargetSpeed_R - encoder

    static_Encoder *= 0.7
    static_Encoder += Encoder_Least * 0.3

    static_Encoder_Integral += Encoder_Least
    static_Encoder_Integral = max(min(static_Encoder_Integral, 50000), -50000)

    Velocity = static_Encoder * R_Velocity_KP + static_Encoder_Integral * R_Velocity_KI
    if TargetSpeed_R == 0:
        static_Encoder_Integral = 0

    Velocity_int = int(Velocity)
    return max(min(Velocity_int, 1200), -1200)


def main():
    # 主函数，模拟定时器中断调用
    while True:
        timer0_int()
        # 模拟100ms的延迟
        import time
        time.sleep(0.1)


if __name__ == "__main__":
    main()
