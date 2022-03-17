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

//typedef struct  //�ļ��ṹ��
//{
//	int _fd;  /* �ļ��� */
//	int _cleft;  /* ��������ʣ�µ��ֽ��� */
//	int _mode;  /* �ļ�����ģʽ */
//	char * _nextc;  /* ��һ���ֽڵ�λ�� */
//	char * _buff;  /* �ļ�������λ�� */
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

void gen()  //������̬��ά����(����)
{
	int i;

	for(i=0;i<LEN;i++)  //ѭ����ʼ��a��ά����
	{
		aMatrix[i] = new int [LEN];
	}

	for(i=0;i<LEN;i++)  //ѭ����ʼ��b��ά����
	{
		bMatrix[i] = new int [LEN];
	}

	for(i=0;i<LEN;i++)  //ѭ����ʼ��c��ά����
	{
		cMatrix[i] = new int [LEN];
	}
	srand((unsigned int)time(NULL));  //������������ĳ�ʼ������,��rand()�������
}

void assign()  //Ϊ2�����󸳳�ֵ
{
	int i,j;
	for(i=0;i<LEN;i++)
	{
		for(j=0;j<LEN;j++)  //Ϊab2����ά���鸳ֵ
		{
			aMatrix[i][j] = rand()%1000;
			bMatrix[i][j] = rand()%1000;
		}
	}
}

int save()  //�����ɵľ��󱣴浽�ļ���ȥ
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

int save_answer()  //�����������浽�ļ���ȥ
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

int read()  //�������ã����ļ��еľ�������ڴ�������к����ķ���ֵ����ȡ�ɹ�����0��ʧ�ܷ���-1
{
	printf("����read����()");
	int i,j,x=0;
	ifstream fin;
	fin.open("E:\\mat\\martix.txt");
	if(!fin)
	{
		printf("��ʧ��\n");
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
	printf("д�����A��B�ɹ�\n");
	return 0;
}

unsigned __stdcall FirstThreadFunc(void * arg)  //�������ã����̼߳����ڴ�����������ĳ˻�
{
	
	ProcParam* param = (ProcParam*)arg;  //��ȡ�����ṹ��	
	/*�Ӳ����л����Ϣ*/
	DWORD id = param->threadid;
	int top = param->range_top;
	int bottom = param->range_bottom;
	int i,j,k;
	int result=0;
	//printf("�߳�%d�Ѿ�������\n",id);
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

unsigned __stdcall SecondThreadFunc(void * arg)  //�������ã����̼߳����ڴ�����������ĳ˻�
{
	ProcParam* param = (ProcParam*)arg;  //��ȡ�����ṹ��	
	/*�Ӳ����л����Ϣ*/
	DWORD id = param->threadid;
	int top = param->range_top;
	int bottom = param->range_bottom;
	int i,j,k;
	int result=0;

	//printf("�߳�%d�Ѿ�������\n",id);
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


void ColColFunc()  //ִ�С����С���������
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
	printf("�����̶߳�ִ�����\n");
}


void RowRowFunc()  //ִ�С����С���������
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
			//   printf("��%d���߳�����ʱ��Ϊ:%d-%d-%d-%d\n",i,ftCreatTime,ExitTime,ftKernelTimeStart,ftUserTimeStart);
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
	//		  printf("��%d���߳�����ʱ��Ϊ:%d-%d-%d-%d\n",i,ftCreatTime,ExitTime,ftKernelTimeStart,ftUserTimeStart);
	//}
	WaitForMultipleObjects(thread_num,hThread,TRUE,INFINITE);
	for(i=0;i<thread_num;i++)
	{
		CloseHandle(hThread[i]);
	}
	EndTime = GetTickCount();
	
	printf("�����̶߳�ִ�����\n");
}

unsigned __stdcall ThirdThreadFunc(void * arg)  //�������ã����̼߳����ڴ�����������ĳ˻�
{
	ProcParam_2* param = (ProcParam_2*)arg;  //��ȡ�����ṹ��	
	/*�Ӳ����л����Ϣ*/
	DWORD id = param->threadid;
	int RowTop = param->range_RowTop;
	int RowBottom = param->range_RowBottom;
	int ColTop = param->range_ColTop;
	int ColBottom = param->range_ColBottom;
	int i,j,k;
	int result=0;


	//printf("�߳�%d�Ѿ�������\n",id);
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



void RowColFunc()  //ִ�С����С���������
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
				   //printf("�ڷ��Ҳ�ǵ��º�����---%d,----%d,----%d,----%d\n",param[sum].range_RowTop,param[sum].range_RowBottom,param[sum].range_ColTop,param[sum].range_ColBottom); //����
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
					//printf("���Ҳ�ǵ��º�����---%d,----%d,----%d,----%d\n",param[sum].range_RowTop,param[sum].range_RowBottom,param[sum].range_ColTop,param[sum].range_ColBottom); //����
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
					//printf("�ڷ��Ҳ���º�����---%d,----%d,----%d,----%d\n",param[sum].range_RowTop,param[sum].range_RowBottom,param[sum].range_ColTop,param[sum].range_ColBottom); //����
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
					//printf("���Ҳ���º�����---%d,----%d,----%d,----%d\n",param[sum].range_RowTop,param[sum].range_RowBottom,param[sum].range_ColTop,param[sum].range_ColBottom); //����
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
	printf("�����̶߳�ִ�����\n");

}


int main()
{
	int start;
	int ret;
	printf("***********************************\n");
	printf("������Ҫ�����µľ���ô?\n");
    printf("     1: YEP!!������һ����!\n") ;
	printf("     2: NOP!!�þɵ�!\n") ;
 	printf("***********************************\n");
    printf( "Enter your choice (1 or 2): ");
	scanf("%d",&start);
	while(start != 1&&start != 2)
	{
		printf("������������������\n");
	   scanf("%d",&start);
	}
    gen();  //Ϊabc3���������ڴ�ռ�

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
		read();  //��ȡ�����ڴ�
	}
	else
	{
		read();
	}


	int way;
	printf("***********************************\n");
	printf("������Ҫde���㷽����ʲô?\n");
    printf("     1: ִ�н�C������ж��̼߳���!\n") ;
	printf("     2: ִ�н�C������ж��̼߳���!\n") ;
	printf("     3: ִ�н�C�طֳ�С����̼߳���!\n") ;
 	printf("***********************************\n");
    printf( "Enter your choice (1 or 2 or 3): ");
	scanf("%d",&way);
	while(way != 1&&way != 2&&way != 3)
	{
		printf("������������������\n");
	   scanf("%d",&way);
	}
	if(way ==1)
	{
		int  num;
	   printf("����Ҫ���ɵ��߳���:");
	   scanf("%d",&num);
	   thread_num = (int)num;
		RowRowFunc();
	}
	else if(way == 2)
	{
		int num;
	   printf("����Ҫ���ɵ��߳���:");
	   scanf("%d",&num);
	   thread_num = (int)num;
		ColColFunc();
	}
	else if(way ==3)
	{
		int num_x,num_y;
		printf("Ҫ���ɵ��߳���sum=X*Y(X,YΪ����1��������)(sum�����߳���):\n");
		printf("X����=");
		scanf("%d",&num_x);
		printf("Y����=");
		scanf("%d",&num_y);
		thread_numCol = (int)num_x;
		thread_numRow = (int)num_y;
		RowColFunc();
	}

	printf("��ʼʱ�䣺%d\n",StartTime);
	printf("����ʱ��:%d\n",EndTime);
	RunTime = EndTime-StartTime;
	printf("�÷���������ʱ����:%d\n",RunTime);

	int save;
	printf("***********************************\n");
	printf("��Ҫ���õ��ľ��󱣴浽�ļ�ô?\n");
    printf("     1: SURE!\n") ;
	printf("     2: NO!\n") ;
 	printf("***********************************\n");
    printf( "Enter your choice (1 or 2 ): ");
	scanf("%d",&save);
	while(save != 1&&save != 2)
	{
		printf("������������������\n");
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
		printf("ִ��ȫ������");
	}
	getchar();
	return 0;
}
