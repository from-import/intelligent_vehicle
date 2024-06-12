#include "headfile.h"
#include "Initial.h"

//Button参数
int adjust = 5;
int Statu = 0;
int Speed_Clock = 0;

	
void Button()
{
	//校准中值和横竖比值
	if(First == 0)
	{
		//除号后面是实验室目标值
		//adjust = (data_last[0]+data_last[4])/(data_last[1]+data_last[3]);
	}
	if(Second == 0){
		delay_ms(1000);
		Statu = 1;
		Speed_Clock = 100;
	}		
}