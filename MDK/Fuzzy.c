#include "headfile.h"
#include "Initial.h"


/**
* 列坐标：NB,NM,NS,O,PS,PM,PB
* 横坐标：-6,-5,-4,-3,-2,-1,0,1,2,3,4,5,6
* @param 建立输入、输出隶属度函数,进行微调调整模糊量的范围
*/
/***************************************误差隶属度函数***************************************/
float Input1_Terms_Membership[7][13] =
{ 1,0.15,0,0,0,0,0,0,0,0,0,0,0,
0,0.2,1,0.2,0,0,0,0,0,0,0,0,0,
0,0,0,0.2,1,0.2,0,0,0,0,0,0,0,
0,0,0,0,0,0.1,1,0.1,0,0,0,0,0,
0,0,0,0,0,0,0,0.1,1,0.1,0,0,0,
0,0,0,0,0,0,0,0,0,0.2,1,0.2,0,
0,0,0,0,0,0,0,0,0,0,0,0.2,1
};
/***************************************误差变化率隶属度函数***************************************/
float Input2_Terms_Membership[7][13] =
{ 1,0.15,0,0,0,0,0,0,0,0,0,0,0,
0,0.2,1,0.2,0,0,0,0,0,0,0,0,0,
0,0,0,0.2,1,0.2,0,0,0,0,0,0,0,
0,0,0,0,0,0.1,1,0.1,0,0,0,0,0,
0,0,0,0,0,0,0,0.1,1,0.1,0,0,0,
0,0,0,0,0,0,0,0,0,0.2,1,0.2,0,
0,0,0,0,0,0,0,0,0,0,0,0.2,1
};
/***************************************输出（速度）***************************************/
float Output_Terms_Membership[7][13] =
{ 1,0.15,0,0,0,0,0,0,0,0,0,0,0,
0,0.2,1,0.2,0,0,0,0,0,0,0,0,0,
0,0,0,0.2,1,0.2,0,0,0,0,0,0,0,
0,0,0,0,0,0.1,1,0.1,0,0,0,0,0,
0,0,0,0,0,0,0,0.1,1,0.1,0,0,0,
0,0,0,0,0,0,0,0,0,0.2,1,0.2,0,
0,0,0,0,0,0,0,0,0,0,0,0.2,1
};


/**
* 纵轴为E(error)，横轴为EC(error_delta)，值为速度七档NB(0),NM(1),NS(2),Z(3),PS(4),PM(5),PB(6)速度由小变大再变小
* 列坐标：E（NB,NM,NS,O,PS,PM,PB）
* 横坐标：EC（NB,NM,NS,O,PS,PM,PB）
* 值：U（1：NB:2,NM,3:NS,4:O,5:PS,6:PM,7:PB）
* @param 模糊控制规则表，调整速度变化趋势
*/
int Rule[7][7] =
{ 1,1,2,2,6,7,7,
 1,1,2,2,6,6,6,
 1,2,3,4,5,6,6,
 1,3,4,4,4,5,7,
 2,2,3,4,5,6,7,
 2,2,2,2,6,7,7,
 1,1,2,2,6,7,7
};//调试参数

float  R[169][13] = { 0 };
float R1[13][13] = { 0 };
float AdBd1[13][13] = { 0 };
float R2[169] = { 0 };
float AdBd2[169] = { 0 };
float R3[169][13] = { 0 };
float  Cd[13] = { 0 };
float Fuzzy_Table[13][13] = { 0 };
float SPEED[13] = { 200,220,230,240,250,270,300,270,250,240,230,220,200 };//调试参数
int Max_Input1_value = 0, Max_Input2_value = 0;

/**
* @param 模糊化过程实现论域内不同值对应隶属度最大的语言值
*/
int  E_MAX(int e)
{
	int i = 0, max = 0;
	for (i = 0; i < 7; i++)
		if (Input1_Terms_Membership[i][e] > Input1_Terms_Membership[max][e])
			max = i;
	return max;
}

int  EC_MAX(int ex)
{
	int i = 0, max = 0;
	for (i = 0; i < 7; i++)
		if (Input2_Terms_Membership[i][ex] > Input1_Terms_Membership[max][ex])
			max = i;
	return max;
}

void calculate()
{
	/***************************************计算所有规则模糊关系的并集Rule***************************************/
	int i = 0, j = 0, k = 0;
	int Input1_value_index = 0, Input2_value_index = 0;
	int Input1_Terms_Index = 0, Input2_Terms_Index = 0;
	int Output_Terms_Index;

	//计算Rule(初始化），计算Rij，并对所有的R取并集，R=(EXEC)XU
	for (Input1_Terms_Index = 0; Input1_Terms_Index < 7; Input1_Terms_Index++)
		for (Input2_Terms_Index = 0; Input2_Terms_Index < 7; Input2_Terms_Index++)
		{
			// E和EC的语言值两两组合及其输出计算Rule
			Output_Terms_Index = Rule[Input1_Terms_Index][Input2_Terms_Index] - 1;
			k = 0;
			for (i = 0; i < 13; i++)
				for (j = 0; j < 13; j++)
				{
					// E和EC进行取小运算
					if (Input1_Terms_Membership[Input1_Terms_Index][i] < Input2_Terms_Membership[Input2_Terms_Index][j])
						R1[i][j] = Input1_Terms_Membership[Input1_Terms_Index][i];
					else
						R1[i][j] = Input2_Terms_Membership[Input2_Terms_Index][j];
					// 转换R1矩阵为R2一维向量
					R2[k] = R1[i][j];
					k++;
				}
			///<A=Input1_Terms_Membership[Input1_Terms_Index],B=Input2_Terms_Membership[Input2_Terms_Index]
			///<R1=AXB建立13x13的矩阵,R2=R1'把矩阵转成169x1的列向量
			for (i = 0; i < 169; i++)
				for (j = 0; j < 13; j++)
				{
					// R1(E, EC)与U进行取小运算
					if (R2[i] < Output_Terms_Membership[Output_Terms_Index][j])
						R3[i][j] = R2[i];
					else
						R3[i][j] = Output_Terms_Membership[Output_Terms_Index][j];
					// R进行取大运算，为所有规则模糊关系的并集
					if (R3[i][j] > R[i][j])
						R[i][j] = R3[i][j];
				}
		}


	/*************************对于每种可能的E、EC的精确取值模糊化后进行推理得到模糊输出Cd，Cd=(AdxBd)oR*************************/
	for (Input1_value_index = 0; Input1_value_index < 13; Input1_value_index++) {
		for (Input2_value_index = 0; Input2_value_index < 13; Input2_value_index++)
		{
			for (j = 0; j < 13; j++)
				Cd[j] = 0;
			int kd = 0;
			float temp = 0;
			Max_Input1_value = E_MAX(Input1_value_index);	///<找出误差隶属度最大的语言值
			Max_Input2_value = EC_MAX(Input2_value_index);	///<找出误差变化率隶属度最大的语言值
			for (i = 0; i < 13; i++)
				for (j = 0; j < 13; j++)
				{
					// E(Ad)和EC(Bd)进行取小运算
					if (Input1_Terms_Membership[Max_Input1_value][i] < Input2_Terms_Membership[Max_Input2_value][j])
						AdBd1[i][j] = Input1_Terms_Membership[Max_Input1_value][i];
					else
						AdBd1[i][j] = Input2_Terms_Membership[Max_Input2_value][j];
					AdBd2[kd] = AdBd1[i][j];
					kd++;
				}
			for (i = 0; i < 169; i++)
				for (j = 0; j < 13; j++)
				{
					// 模糊矩阵的合成，将乘积运算换成“取小”，将加法运算换成“取大”
					if (AdBd2[i] < R[i][j])
						temp = AdBd2[i];
					else
						temp = R[i][j];
					if (temp > Cd[j])
						Cd[j] = temp;
				}


			/*************************去模糊化（加权平均法），计算实际输出*************************/
			float sum1 = 0, sum2 = 0;
			float OUT;
			for (i = 0; i < 13; i++)
			{
				sum1 = sum1 + Cd[i];
				sum2 = sum2 + Cd[i] * SPEED[i];
			}
			OUT = (int)(sum2 / sum1 + 0.5);///<四舍五入
			Fuzzy_Table[Input1_value_index][Input2_value_index] = OUT;
			cout << OUT << ",";
		}
		cout << endl;
	}
}

int_16 Fuzzy_Table[13][13]= 
{ 
203,211,211,211,226,226,230,230,228,210,210,210,210,
209,221,221,221,238,238,241,241,237,231,231,231,227,
209,221,221,221,238,238,241,241,237,231,231,231,227,
209,221,221,221,238,238,241,241,237,231,231,231,227,
215,238,238,238,245,245,266,266,246,237,237,237,232,
215,238,238,238,245,245,266,266,246,237,237,237,232,
218,250,250,250,276,276,283,283,280,245,245,245,216,
218,250,250,250,276,276,283,283,280,245,245,245,216,
232,240,240,240,250,250,271,271,246,236,236,236,217,
226,230,230,230,236,236,239,239,236,214,214,214,208,
226,230,230,230,236,236,239,239,236,214,214,214,208,
226,230,230,230,236,236,239,239,236,214,214,214,208,
211,211,211,211,226,226,230,230,228,208,208,208,203
}  ;

int_16 get_speed_set(void) {
	int_16 E = 0, EC = 0;
	int_16 speed_target;
	static int_16 re_pos = 0, ek = 0, eck = 0;
	float ke = 400, kec = 10;
	ek = 2500 - row;
	eck = 2500 - row - re_pos;
	re_pos = ek;

	if (ek > 0) {

		E = (int_32)(ek / ke + 0.5);
	}
	else {

		E = (int_32)(ek / ke - 0.5);
	}
	//将E的论域转换到模糊控制器的论域
	if (E > 6)
		E = 6;
	else if (E < -6)
		E = -6;
	if (eck > 0) {

		EC = (int_16)(eck / kec + 0.5);
	}
	else {

		EC = (int_16)(eck / kec - 0.5);
	}//将EC的论域转换到模糊控制器的论域
	if (EC > 6)
		EC = 6;
	else if (EC < -6)
		EC = -6;

	speed_target = (int_16)(Fuzzy_Table[E + 6][EC + 6]);
	return speed_target ;
}
