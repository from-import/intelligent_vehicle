#include "headfile.h"
#include "Initial.h"

//Button参数
int adjust = 5;
int Statu = 0;

	
void Button()
{
	//校准中值和横竖比值
	if(First == 0)
	{
		Dir_P = Dir_P + 0.1;
	}
	if(Second == 0){
		delay_ms(1000);
		Statu = 1;
	}		
}