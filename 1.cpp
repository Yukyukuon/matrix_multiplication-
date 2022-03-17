#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;


int StartTime;
int EndTime;
int RunTime;

int thread_num;
int thread_numCol;
int thread_numRow;
#define thread_maxnum  50

const int LEN = 1024;
const int xLEN = 1024;
const int yLEN = 1024;
const int max=xLEN*yLEN;

int **aMatrix = new int *[LEN];
int **bMatrix = new int *[LEN];
int **cMatrix = new int *[LEN];

//typedef struct  //文件结构体
//{
//	int _fd;  /* 文件号 */
//	int _cleft;  /* 缓冲区中剩下的字节数 */
//	int _mode;  /* 文件操作模式 */
//	char * _nextc;  /* 下一个字节的位置 */
//	char * _buff;  /* 文件缓冲区位置 */
//}FILE;
struct ProcParam
{
	int	threadID;
	int range_bottom, range_top;
	int threadid;
};

struct ProcParam_2
{
	int threadID;
	int range_RowTop, range_RowBottom;
	int range_ColTop, range_ColBottom;
	int threadid;
};

void gen()  //创建动态二维数组(矩阵)
{
	int i;

	for(i=0;i<LEN;i++)  //循环初始化a二维数组
	{
		aMatrix[i] = new int [LEN];
	}

	for(i=0;i<LEN;i++)  //循环初始化b二维数组
	{
		bMatrix[i] = new int [LEN];
	}

	for(i=0;i<LEN;i++)  //循环初始化c二维数组
	{
		cMatrix[i] = new int [LEN];
	}
	srand((unsigned int)time(NULL));  //随机数发生器的初始化函数,让rand()随机生成
}

void assign()  //为2个矩阵赋初值
{
	int i,j;
	for(i=0;i<LEN;i++)
	{
		for(j=0;j<LEN;j++)  //为ab2个二维数组赋值
		{
			aMatrix[i][j] = rand()%1000;
			bMatrix[i][j] = rand()%1000;
		}
	}
}

int save()  //将生成的矩阵保存到文件中去
{
	int i,j;
	FILE *fw;
	fw = fopen("E:\\mat\\martix.txt","wb");
	if(fw == NULL)
	{
		printf("openf file error. \n");
		return -1;
	}
	//fprintf(fw,"%d\n", LEN);
	for(i=0;i<LEN;i++)
	{
		for(j=0;j<LEN;j++)
		{
			fprintf(fw,"%d ",aMatrix[i][j]);
		}
	}
	for(i=0;i<LEN;i++)
	{
		for(j=0;j<LEN;j++)
		{
			fprintf(fw,"%d ",bMatrix[i][j]);
		}
	}
	fclose(fw);
	return 0;
}

int save_answer()  //将计算结果保存到文件中去
{
	int i,j;
	FILE *fw;
	fw = fopen("E:\\mat\\answer.txt","wb");
	if(fw == NULL)
	{
		printf("openf file error. \n");
		return -1;
	}
	//fprintf(fw,"%d\n", LEN);
	for(i=0;i<LEN;i++)
	{
		for(j=0;j<LEN;j++)
		{
			fprintf(fw,"%d ",cMatrix[i][j]);
		}
	}
	fclose(fw);
	return 0;
}

int read()  //函数作用：将文件中的矩阵读入内存的数组中函数的返回值：读取成功返回0，失败返回-1
{
	printf("进入read函数()");
	int i,j,x=0;
	ifstream fin;
	fin.open("E:\\mat\\martix.txt");
	if(!fin)
	{
		printf("打开失败\n");
		return -1;
	}
	if(x<max)
	{
		for(i=0;i<xLEN;i++)
		{
			for(j=0;j<yLEN;j++)
			{
				fin>>aMatrix[i][j];
				x++;
				fin.get();
			}
		}
	}
    if(x<(2*max))
	{
		for(i=0;i<xLEN;i++)
		{
			for(j=0;j<yLEN;j++)
			{
				fin>>bMatrix[i][j];
				x++;
				fin.get();
			}
		}
	}
	printf("写入矩阵A与B成功\n");
	return 0;
}

unsigned __stdcall FirstThreadFunc(void * arg)  //函数作用：多线程计算内存中两个矩阵的乘积
{
	
	ProcParam* param = (ProcParam*)arg;  //获取参数结构体	
	/*从参数中获得信息*/
	DWORD id = param->threadid;
	int top = param->range_top;
	int bottom = param->range_bottom;
	int i,j,k;
	int result=0;
	//printf("线程%d已经创建。\n",id);
	for(i=bottom;i<top;i++)
	{
		for(j=0;j<yLEN;j++)
		{
			for(k=0;k<LEN;k++)
			{
				result = result+aMatrix[i][k] * bMatrix[k][j];
			}
			cMatrix[i][j] = result;
			result = 0;
		}
	}
	return 0;

}

unsigned __stdcall SecondThreadFunc(void * arg)  //函数作用：多线程计算内存中两个矩阵的乘积
{
	ProcParam* param = (ProcParam*)arg;  //获取参数结构体	
	/*从参数中获得信息*/
	DWORD id = param->threadid;
	int top = param->range_top;
	int bottom = param->range_bottom;
	int i,j,k;
	int result=0;

	//printf("线程%d已经创建。\n",id);
	for(j=bottom;j<top;j++)
	{
		for(i=0;i<xLEN;i++)
		{
			for(k=0;k<LEN;k++)
			{
				result = result+aMatrix[i][k] * bMatrix[k][j];
			}
			cMatrix[i][j] = result;
			result = 0;
		}
	}
	return 0;

}


void ColColFunc()  //执行“列列”矩阵运算
{
	StartTime = GetTickCount();
	HANDLE hThread[thread_maxnum];
	unsigned int threadID[thread_maxnum];
	ProcParam *param=new ProcParam[thread_maxnum];
	param[0].range_bottom = 0;
	param[0].range_top = 0;
	for(int i=0;i<thread_num;i++)
	{
		if(i != (thread_num-1))
		{	
			param[i].threadID = i;
			param[i].range_top = param[i].range_bottom+(xLEN/thread_num);
			param[i].threadid = i+1;
		   hThread[i] = (HANDLE)_beginthreadex(NULL,0,SecondThreadFunc,(void *)&param[i],0,&threadID[i]);
			param[i+1].range_bottom = param[i].range_top;
		}
		else
		{
			param[i].threadID = i;
			param[i].range_top = LEN;
			param[i].threadid = i+1;
		   hThread[i] = (HANDLE)_beginthreadex(NULL,0,SecondThreadFunc,(void *)&param[i],0,&threadID[i]);
		}
	}
	WaitForMultipleObjects(thread_num,hThread,TRUE,INFINITE);
	for(i=0;i<thread_num;i++)
	{
		CloseHandle(hThread[i]);
	}
	EndTime = GetTickCount();
	printf("所有线程都执行完毕\n");
}


void RowRowFunc()  //执行“行行”矩阵运算
{
	StartTime = GetTickCount();
	HANDLE hThread[thread_maxnum];
	unsigned int threadID[thread_maxnum];
	ProcParam *param=new ProcParam[thread_maxnum];
	param[0].range_bottom = 0;
	param[0].range_top = 0;
	for(int i=0;i<thread_num;i++)
	{
		if(i != (thread_num-1))
		{	
			param[i].threadID = i;
			param[i].range_top = param[i].range_bottom+(xLEN/thread_num);
			param[i].threadid = i+1;
		   hThread[i] = (HANDLE)_beginthreadex(NULL,0,FirstThreadFunc,(void *)&param[i],0,&threadID[i]);
			//GetThreadTimes(hThread[i], &ftCreatTime, &ExitTime,
            //   &ftKernelTimeStart, &ftUserTimeStart);
			//   printf("第%d个线程运行时间为:%d-%d-%d-%d\n",i,ftCreatTime,ExitTime,ftKernelTimeStart,ftUserTimeStart);
			param[i+1].range_bottom = param[i].range_top;
		}
		else
		{
			param[i].threadID = i;
			param[i].range_top = LEN;
			param[i].threadid = i+1;
		   hThread[i] = (HANDLE)_beginthreadex(NULL,0,FirstThreadFunc,(void *)&param[i],0,&threadID[i]);
		}
	}
	//for(i=0;i<thread_num;i++)
	//{
	//			GetThreadTimes(hThread[i], &ftCreatTime, &ExitTime,
    //           &ftKernelTimeStart, &ftUserTimeStart);
	//		  printf("第%d个线程运行时间为:%d-%d-%d-%d\n",i,ftCreatTime,ExitTime,ftKernelTimeStart,ftUserTimeStart);
	//}
	WaitForMultipleObjects(thread_num,hThread,TRUE,INFINITE);
	for(i=0;i<thread_num;i++)
	{
		CloseHandle(hThread[i]);
	}
	EndTime = GetTickCount();
	
	printf("所有线程都执行完毕\n");
}

unsigned __stdcall ThirdThreadFunc(void * arg)  //函数作用：多线程计算内存中两个矩阵的乘积
{
	ProcParam_2* param = (ProcParam_2*)arg;  //获取参数结构体	
	/*从参数中获得信息*/
	DWORD id = param->threadid;
	int RowTop = param->range_RowTop;
	int RowBottom = param->range_RowBottom;
	int ColTop = param->range_ColTop;
	int ColBottom = param->range_ColBottom;
	int i,j,k;
	int result=0;


	//printf("线程%d已经创建。\n",id);
	for(i=RowBottom;i<RowTop;i++)
	{
		for(j=ColBottom;j<ColTop;j++)
		{
			for(k=0;k<LEN;k++)
			{
				result = result+aMatrix[i][k] * bMatrix[k][j];
			}
			cMatrix[i][j] = result;
			result = 0;
		}
	}
	return 0;

}



void RowColFunc()  //执行“行列”矩阵运算
{
	StartTime = GetTickCount();
	int i,j;
	HANDLE hThread[thread_maxnum];
	unsigned int threadID[thread_maxnum];
	ProcParam_2 *param=new ProcParam_2[thread_maxnum];
	param[0].range_RowBottom = 0;
	param[0].range_RowTop = 0;
	param[0].range_ColBottom = 0;
	param[0].range_ColTop = 0;
	int sum = 0;
	thread_num = thread_numRow*thread_numCol;
	for(i=0;i<thread_numRow;i++)
	{
		if(i != (thread_numRow-1))
		{
			param[sum].range_RowTop = param[sum].range_RowBottom+(yLEN/thread_numRow);
			for(j=0;j<thread_numCol;j++)
			{
				if(j != (thread_numCol-1))
				{
					param[sum].threadID = sum;
					param[sum].range_ColTop = param[sum].range_ColBottom+(xLEN/thread_numCol);
					param[sum].threadid = sum+1;
		           hThread[sum] = (HANDLE)_beginthreadex(NULL,0,ThirdThreadFunc,(void *)&param[sum],0,&threadID[sum]);
				   //printf("在非右侧非底下函数中---%d,----%d,----%d,----%d\n",param[sum].range_RowTop,param[sum].range_RowBottom,param[sum].range_ColTop,param[sum].range_ColBottom); //测试
					param[sum+1].range_ColBottom = param[sum].range_ColTop;
					param[sum+1].range_RowBottom = param[sum].range_RowBottom;
					param[sum+1].range_RowTop = param[sum].range_RowTop;
					sum++;
				}
				else
				{
					param[sum].threadID =sum;
					param[sum].range_ColTop = xLEN;
					param[sum].threadid = sum+1;
                    hThread[sum] = (HANDLE)_beginthreadex(NULL,0,ThirdThreadFunc,(void *)&param[sum],0,&threadID[sum]);
					//printf("在右侧非底下函数中---%d,----%d,----%d,----%d\n",param[sum].range_RowTop,param[sum].range_RowBottom,param[sum].range_ColTop,param[sum].range_ColBottom); //测试
					param[sum+1].range_ColBottom = 0;
					param[sum+1].range_RowBottom = param[sum].range_RowTop;
					sum++;
				}
			}
		}
		else
		{
			param[sum].range_RowTop = yLEN;
			for(int j=0;j<thread_numCol;j++)
			{
				if(j != (thread_numCol-1))
				{
					param[sum].threadID = sum;
					param[sum].range_ColTop = param[sum].range_ColBottom+(xLEN/thread_numCol);
					param[sum].threadid = sum+1;
		           hThread[sum] = (HANDLE)_beginthreadex(NULL,0,ThirdThreadFunc,(void *)&param[sum],0,&threadID[sum]);
					//printf("在非右侧底下函数中---%d,----%d,----%d,----%d\n",param[sum].range_RowTop,param[sum].range_RowBottom,param[sum].range_ColTop,param[sum].range_ColBottom); //测试
					param[sum+1].range_ColBottom = param[sum].range_ColTop;
					param[sum+1].range_RowBottom = param[sum].range_RowBottom;
					param[sum+1].range_RowTop = param[sum].range_RowTop;
					sum++;
				}
				else
				{
					param[sum].threadID =sum;
					param[sum].range_ColTop = xLEN;
					param[sum].threadid = sum+1;
                    hThread[sum] = (HANDLE)_beginthreadex(NULL,0,ThirdThreadFunc,(void *)&param[sum],0,&threadID[sum]);
					//printf("在右侧底下函数中---%d,----%d,----%d,----%d\n",param[sum].range_RowTop,param[sum].range_RowBottom,param[sum].range_ColTop,param[sum].range_ColBottom); //测试
					param[sum+1].range_ColBottom = 0;
					param[sum+1].range_RowBottom = param[sum].range_RowTop;
					sum++;
				}
			}
		}
	}
	WaitForMultipleObjects(thread_num,hThread,TRUE,INFINITE);
	for(int o=0;o<sum;o++)
	{
		CloseHandle(hThread[o]);
	}
	EndTime = GetTickCount();
	printf("所有线程都执行完毕\n");

}


int main()
{
	int start;
	int ret;
	printf("***********************************\n");
	printf("请问需要生成新的矩阵么?\n");
    printf("     1: YEP!!来生成一个吧!\n") ;
	printf("     2: NOP!!用旧的!\n") ;
 	printf("***********************************\n");
    printf( "Enter your choice (1 or 2): ");
	scanf("%d",&start);
	while(start != 1&&start != 2)
	{
		printf("输入有误，请重新输入\n");
	   scanf("%d",&start);
	}
    gen();  //为abc3个矩阵创造内存空间

	if(start == 1)
	{
		assign();
		ret = save();
		if(ret != 0)
		{
			printf("save file error.\n",ret);
		   getchar();
		   exit(1);
		}
		read();  //读取矩阵到内存
	}
	else
	{
		read();
	}


	int way;
	printf("***********************************\n");
	printf("请问需要de计算方法是什么?\n");
    printf("     1: 执行将C矩阵分行多线程计算!\n") ;
	printf("     2: 执行将C矩阵分列多线程计算!\n") ;
	printf("     3: 执行将C矩分成小块多线程计算!\n") ;
 	printf("***********************************\n");
    printf( "Enter your choice (1 or 2 or 3): ");
	scanf("%d",&way);
	while(way != 1&&way != 2&&way != 3)
	{
		printf("输入有误，请重新输入\n");
	   scanf("%d",&way);
	}
	if(way ==1)
	{
		int  num;
	   printf("请问要生成的线程数:");
	   scanf("%d",&num);
	   thread_num = (int)num;
		RowRowFunc();
	}
	else if(way == 2)
	{
		int num;
	   printf("请问要生成的线程数:");
	   scanf("%d",&num);
	   thread_num = (int)num;
		ColColFunc();
	}
	else if(way ==3)
	{
		int num_x,num_y;
		printf("要生成的线程数sum=X*Y(X,Y为大于1的正整数)(sum是总线程数):\n");
		printf("X列数=");
		scanf("%d",&num_x);
		printf("Y行数=");
		scanf("%d",&num_y);
		thread_numCol = (int)num_x;
		thread_numRow = (int)num_y;
		RowColFunc();
	}

	printf("开始时间：%d\n",StartTime);
	printf("结束时间:%d\n",EndTime);
	RunTime = EndTime-StartTime;
	printf("该方法运行总时间是:%d\n",RunTime);

	int save;
	printf("***********************************\n");
	printf("需要将得到的矩阵保存到文件么?\n");
    printf("     1: SURE!\n") ;
	printf("     2: NO!\n") ;
 	printf("***********************************\n");
    printf( "Enter your choice (1 or 2 ): ");
	scanf("%d",&save);
	while(save != 1&&save != 2)
	{
		printf("输入有误，请重新输入\n");
	   scanf("%d",&way);
	}
	if(save ==1)
	{
		ret = save_answer();
		if(ret != 0)
		{
			printf("save file error.\n",ret);
		   getchar();
		   exit(1);
		}
	}
	else
	{
		printf("执行全部结束");
	}
	getchar();
	return 0;
}
