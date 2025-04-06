#include "main.h"


unsigned char KeyNum,MODE,TimeSetSelect,TimeSetFlashFlag;

void TimeShow(void)//ʱ����ʾ����
{
	DS1302_ReadTime();//��ȡʱ��
	OLED_ShowNum(1,3,DS1302_Time[0],2);//��ʾ��
	OLED_ShowNum(1,6,DS1302_Time[1],2);//��ʾ��
	OLED_ShowNum(1,9,DS1302_Time[2],2);//��ʾ��
	OLED_ShowNum(2,12,DS1302_Time[6],1);//��ʾ����
	OLED_ShowNum(2,1,DS1302_Time[3],2);//��ʾʱ
	OLED_ShowNum(2,4,DS1302_Time[4],2);//��ʾ��
	OLED_ShowNum(2,7,DS1302_Time[5],2);//��ʾ��
}

void TimeSet(void)//ʱ�����ù���
{
	if(KeyNum==2)//����2����
	{
		TimeSetSelect++;//����ѡ��λ��1
		TimeSetSelect%=7;//Խ������
	}
	if(KeyNum==3)//����3����
	{
		DS1302_Time[TimeSetSelect]++;//ʱ������λ��ֵ��1
		if(DS1302_Time[0]>99){DS1302_Time[0]=0;}//��Խ���ж�
		if(DS1302_Time[1]>12){DS1302_Time[1]=1;}//��Խ���ж�
		if( DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || 
			DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)//��Խ���ж�
		{
			if(DS1302_Time[2]>31){DS1302_Time[2]=1;}//����
		}
		else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 || DS1302_Time[1]==11)
		{
			if(DS1302_Time[2]>30){DS1302_Time[2]=1;}//С��
		}
		else if(DS1302_Time[1]==2)
		{
			if(DS1302_Time[0]%4==0)
			{
				if(DS1302_Time[2]>29){DS1302_Time[2]=1;}//����2��
			}
			else
			{
				if(DS1302_Time[2]>28){DS1302_Time[2]=1;}//ƽ��2��
			}
		}
		if(DS1302_Time[3]>23){DS1302_Time[3]=0;}//ʱԽ���ж�
		if(DS1302_Time[4]>59){DS1302_Time[4]=0;}//��Խ���ж�
		if(DS1302_Time[5]>59){DS1302_Time[5]=0;}//��Խ���ж�
		if(DS1302_Time[6]>7){DS1302_Time[6]=1;}//����Խ���ж�
	}
	if(KeyNum==4)//����3����
	{
		DS1302_Time[TimeSetSelect]--;//ʱ������λ��ֵ��1
		if(DS1302_Time[0]<0){DS1302_Time[0]=99;}//��Խ���ж�
		if(DS1302_Time[1]<1){DS1302_Time[1]=12;}//��Խ���ж�
		if( DS1302_Time[1]==1 || DS1302_Time[1]==3 || DS1302_Time[1]==5 || DS1302_Time[1]==7 || 
			DS1302_Time[1]==8 || DS1302_Time[1]==10 || DS1302_Time[1]==12)//��Խ���ж�
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=31;}//����
			if(DS1302_Time[2]>31){DS1302_Time[2]=1;}
		}
		else if(DS1302_Time[1]==4 || DS1302_Time[1]==6 || DS1302_Time[1]==9 || DS1302_Time[1]==11)
		{
			if(DS1302_Time[2]<1){DS1302_Time[2]=30;}//С��
			if(DS1302_Time[2]>30){DS1302_Time[2]=1;}
		}
		else if(DS1302_Time[1]==2)
		{
			if(DS1302_Time[0]%4==0)
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=29;}//����2��
				if(DS1302_Time[2]>29){DS1302_Time[2]=1;}
			}
			else
			{
				if(DS1302_Time[2]<1){DS1302_Time[2]=28;}//ƽ��2��
				if(DS1302_Time[2]>28){DS1302_Time[2]=1;}
			}
		}
		if(DS1302_Time[3]<0){DS1302_Time[3]=23;}//ʱԽ���ж�
		if(DS1302_Time[4]<0){DS1302_Time[4]=59;}//��Խ���ж�
		if(DS1302_Time[5]<0){DS1302_Time[5]=59;}//��Խ���ж�
		if(DS1302_Time[6]<1){DS1302_Time[6]=7;}//����Խ���ж�
	}
	//������ʾ������TimeSetSelect��TimeSetFlashFlag�жϿ������˸����
	if(TimeSetSelect==0 && TimeSetFlashFlag==1){OLED_ShowString(1,3,"  ");}
	else {OLED_ShowNum(1,3,DS1302_Time[0],2);}
	if(TimeSetSelect==1 && TimeSetFlashFlag==1){OLED_ShowString(1,6,"  ");}
	else {OLED_ShowNum(1,6,DS1302_Time[1],2);}
	if(TimeSetSelect==2 && TimeSetFlashFlag==1){OLED_ShowString(1,9,"  ");}
	else {OLED_ShowNum(1,9,DS1302_Time[2],2);}
	if(TimeSetSelect==3 && TimeSetFlashFlag==1){OLED_ShowString(2,1,"  ");}
	else {OLED_ShowNum(2,1,DS1302_Time[3],2);}
	if(TimeSetSelect==4 && TimeSetFlashFlag==1){OLED_ShowString(2,4,"  ");}
	else {OLED_ShowNum(2,4,DS1302_Time[4],2);}
	if(TimeSetSelect==5 && TimeSetFlashFlag==1){OLED_ShowString(2,7,"  ");}
	else {OLED_ShowNum(2,7,DS1302_Time[5],2);}
	if(TimeSetSelect==6 && TimeSetFlashFlag==1){OLED_ShowString(2,12," ");}
	else {OLED_ShowNum(2,12,DS1302_Time[6],1);}

}



void Timer1_Routine() interrupt 3
{
	static unsigned int T1Count;
	TL1 = 0x66;				//���ö�ʱ��ʼֵ
	TH1 = 0xFC;				//���ö�ʱ��ʼֵ
	T1Count++;
	if(T1Count>=500)//ÿ500ms����һ��
	{
		T1Count=0;
		TimeSetFlashFlag=!TimeSetFlashFlag;//��˸��־λȡ��
	}
}
