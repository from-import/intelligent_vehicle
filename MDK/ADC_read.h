#ifndef ADC_READ_H_
#define ADC_READ_H_

void ADC_read();
extern int offset;         // 电磁差值计算
extern float data_last[5]; // 电磁值
extern int offset2;        // 电磁差值计算
extern int difference1;
extern int difference2;
extern int difference3;
#endif
