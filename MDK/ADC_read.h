#ifndef ADC_READ_H_
#define ADC_READ_H_

void ADC_read();
extern int offset;         // 电磁差值计算
extern float data_last[5]; // 电磁值
extern int offset2;        // 电磁差值计算
extern double difference1;
extern double difference2;
extern double difference3;
extern int ad_sum;
extern int rightOrLeft ;
extern int AD_1MAX;
extern int AD_5MAX;
extern int AD_2MAX;
extern int AD_4MAX;

#endif
