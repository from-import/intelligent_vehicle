#include "headfile.h"
#include "Initial.h"

		
void Show()
{
		//��TFT����ʾ����Ҫ��ʼ��1.8��TFT��Ļ������ʹ�á�
	  lcd_showstr(0,0, "1");			
		lcd_showstr(0,1, "2");			
		lcd_showstr(0,2, "3");
		lcd_showstr(0,3, "4");
		lcd_showstr(0,4, "5");
		lcd_showstr(0,5, "6");
		lcd_showint16(6*10,0,data_last[0]);
		lcd_showint16(6*10,1,data_last[1]);
		lcd_showint16(6*10,2,data_last[2]);
		lcd_showint16(6*10,3,data_last[3]);
		lcd_showint16(6*10,4,data_last[4]);
//		lcd_showint16(8*10,4,(int)Dir);
//		lcd_showint16(8*10,5,(int)dl1a_distance_mm);
		lcd_showint16(8*10,5,data_last[1]/data_last[3]);
 //		lcd_showint16(8*10,5,Standard);
		lcd_showint16(8*10,6,offset2);
//		lcd_showint16(8*10,5,(int)Flag);
//		lcd_showint16(8*10,7,Yaw);

}