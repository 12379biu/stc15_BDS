#include <stc15.h>
#include "Timer0.h"
#include "Delay.h"
#include "DS1302.h"
sbit BPC_OUT = P3^2;
sbit LED = P5^5;
sbit BPC_PON = P5^4;
unsigned int BPC_Time;
unsigned int Count;
unsigned char BPC_State;

unsigned int BPC_Data[25];
unsigned char BPC_pData;

unsigned int HigTime;
unsigned int LowTime;
unsigned char BPC_Flag = 3;//0:�����ж�   1����ʼת��  2��д��DS1302  3:����

unsigned char BPC_Timer[7];
#define MIN_PULSE_TIME 40  // ��С�������ʱ�䣬�ɸ���ʵ���������


// ���˸����ź�
bit is_interference() {
    if (HigTime < MIN_PULSE_TIME || LowTime < MIN_PULSE_TIME) {
        return 1;
    }
    return 0;
}


/**
  * @brief  �ⲿ�жϳ�ʼ��
  * @param  ��
  * @retval ��
  */
void BPC_Init(void)
{
	TimerInit();
	EX0Init();
}


void Timer0_SetCounter(unsigned int Value)
{
	HigTime = Value;
	LowTime = Value;
}


//�����Ľ�����ת����2���ƺ��1�ĸ���
unsigned char NumOf1(unsigned char four_data)
{
  if(four_data==0)
    return 0;
  else if(four_data==1)
    return 1;
  else if(four_data==2)
    return 1;
  else if(four_data==3)
    return 2;
  else return 0;
}



int BPC_Check(void)
{
/*--------------------P1 P2ʱ���� ����У��-----------------------------*/
	int p3_check=0;
	int p4_check=0;
	int num=0;	
	int i = 0;	
	for(i=1;i<10;i++)
	{
	  num+=NumOf1(BPC_Data[i]);  
	}

	if((BPC_Data[10]==0)||(BPC_Data[10]==2))   //ż��У��  
	{
	  if(num%2==0)	//ż��
	    p3_check=1;  
	}
	else if((BPC_Data[10]==1)||(BPC_Data[10]==3))	//����У��
	{
	  if(num%2==1)
	    p3_check=1; 
	}
	
	
	num=0;	
	for(i=11;i<19;i++)
	{
	  num+=NumOf1(BPC_Data[i]);  
	}
	if((BPC_Data[19]==0)||(BPC_Data[19]==2))   //ż��У��  
	{
	  if(num%2==0)	//ż��
	    p4_check=1;  
	}
	else if((BPC_Data[19]==1)||(BPC_Data[19]==3))	//����У��
	{
	  if(num%2==1)
	    p4_check=1; 
	}
	if(p3_check==1 && p4_check==1)
	{return 1;}
	else
	{return 0;}		
	
}

//�ⲿ�ж�0�жϺ�������Ե�ش���ִ��
void Int0_Routine(void) interrupt 0
{
	
	Delay(1);
	
	LED = 1;            //ָʾ��
	if(BPC_OUT == 1 && BPC_Flag == 0)   //�����ش���
	{
	LED = 0;
	if(BPC_State==0)				//״̬0������״̬
	{
		Timer0_SetCounter(0);	//��ʱ��������0
		Timer0_Run(1);			//��ʱ������
		BPC_State=1;				//��״̬Ϊ1
	}
	
	
	
	else if(BPC_State==1)		//״̬1���ȴ���ʼ�ź�
	{
			if(is_interference())//�Ǹ����ź�
			{  
				BPC_State=1;				
			}
			
			else
			{
				BPC_Time = LowTime;
			}
		
		Timer0_SetCounter(0);	//��ʱ��������0
		//�����ʱΪ1550-1950����֮�䣬Ϊ��ʼλ
		if(BPC_Time>1550 && BPC_Time<1950)
		{
			BPC_pData = 1;
			BPC_State=2;			//��״̬Ϊ2
		}
		else					//���ճ���
		{
			BPC_State=1;			//��״̬Ϊ1
		}
	}
	
	
	
	
	
	else if(BPC_State==2)		//״̬2����������
	{
		if(is_interference())//�Ǹ����ź�
		{
			if(LowTime<MIN_PULSE_TIME)
			{
				HigTime += LowTime;//�͵�ƽ�Ǹ����źţ����ʱ�䱾Ӧ���Ǹߵ�ƽ 
				LowTime = 0;
				return;
			}
			else if(HigTime<MIN_PULSE_TIME)
			{
				LowTime += HigTime;//�ߵ�ƽ�Ǹ����źţ����ʱ�䱾Ӧ���ǵ͵�ƽ 
				HigTime = 0;
				return;
			}
		}
		BPC_Time=HigTime;	//��ȡ��һ���жϵ��˴��жϵ�ʱ��
		Timer0_SetCounter(0);
		//�����ʱΪ100ms������յ�������0
		if(BPC_Time>100-50 && BPC_Time<100+50)
		{
			BPC_Data[BPC_pData]=0;	
			BPC_pData++;			//����λ��ָ������
		}
		else if(BPC_Time>200-50 && BPC_Time<200+50)
		{
			BPC_Data[BPC_pData]=1;	
			BPC_pData++;			//����λ��ָ������
		}
		else if(BPC_Time>300-50 && BPC_Time<300+50)
		{
			BPC_Data[BPC_pData]=2;	
			BPC_pData++;			//����λ��ָ������
		}
		else if(BPC_Time>400-50 && BPC_Time<400+50)
		{
			BPC_Data[BPC_pData]=3;	
			BPC_pData++;			//����λ��ָ������
		}
 
		else					//���ճ���
		{
			BPC_pData=0;			//����λ��ָ����0
			BPC_State=1;			//��״̬Ϊ1
		}

		if(BPC_pData>=20)		//ȣ������յ���19λ����
		{
			BPC_Flag = 1;             //��ʱ���
			Timer0_SetCounter(0);
			BPC_pData=0;			//����λ��ָ����0
			Timer0_Run(0);		//��ʱ��ֹͣ
			BPC_State=0;			//��״̬Ϊ0
		}
	}
}
}

void Int0_IRS(void) interrupt 1
{
	TL0 = 0x18;				//���ö�ʱ��ʼֵ
	TH0 = 0xFC;				//���ö�ʱ��ʼֵ
	if(BPC_OUT == 1){HigTime++;}
	else{LowTime++;}
}


//ʱ�����飬����0~6�ֱ�Ϊ�ꡢ�¡��ա�ʱ���֡��롢���ڣ�����Ϊ�з��ŵı���<0���ж�
//char DS1302_Time[]={19,11,16,12,59,55,6};
//����BPCʱ��
void BPC_DataHandle(void)
{
	
  if(BPC_Flag==1)       //һ֡���ݽ������֮��
  {
	  if(BPC_Check()==1)
	  {
		if(BPC_Data[10]==2 || BPC_Data[10]==3)  //���� 
		{BPC_Timer[3]=4*BPC_Data[3]+BPC_Data[4]+12;   }
		else	//����
		{BPC_Timer[3]=4*BPC_Data[3]+BPC_Data[4]; }    
			BPC_Timer[4]=16*BPC_Data[5]+4*BPC_Data[6]+BPC_Data[7]; 	 //����
		switch(BPC_Data[1])	   //��
		{
			case 0:BPC_Timer[5] = 1;break;
			case 1:BPC_Timer[5] = 21;break;
			case 2:BPC_Timer[5] = 41;break;
		}
		BPC_Timer[0]=16*BPC_Data[16]+4*BPC_Data[17]+BPC_Data[18];   //�� 
		BPC_Timer[1]=4*BPC_Data[14]+BPC_Data[15];   //��
		BPC_Timer[2]=16*BPC_Data[11]+4*BPC_Data[12]+BPC_Data[13];   //�� 
		BPC_Timer[6]=4*BPC_Data[8]+BPC_Data[9];   //���� 
		BPC_Flag=2;
	}
  else
	{
	BPC_State = 1;
	BPC_Flag = 0;
	}
	}
}




/*����д��DS1302*/
void BPC_Write_DS1302(void)
{
	if(BPC_Flag == 2)
	{
		int i = 0;
		BPC_PON=1;//������ɣ�ģ�����
		for(i=0;i<7;i++)
		{
			DS1302_Time[i] = BPC_Timer[i];
		}
		DS1302_Time[5]+=20;
		/*Խ���ж�*/
		if(DS1302_Time[5]>45)
		{
			DS1302_Time[5] = 1;
			DS1302_Time[4]++;
			if(DS1302_Time[4]==60)
			{
				DS1302_Time[4]=0;
				DS1302_Time[3]++;//ʱ
			}
		}
		DS1302_SetTime();
		BPC_Flag = 3;
	}

}

/*ÿСʱ�Զ������źţ�����ʮ����10����*/
void BPC_zidong(void)
{
	if(DS1302_Time[4]>=0 && DS1302_Time[4]<=10)
	{
		BPC_Flag = 0;
		BPC_PON=0;
	}
}




