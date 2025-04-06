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
unsigned char BPC_Flag = 3;//0:允许中断   1：开始转化  2：写入DS1302  3:空闲

unsigned char BPC_Timer[7];
#define MIN_PULSE_TIME 40  // 最小脉冲持续时间，可根据实际情况调整


// 过滤干扰信号
bit is_interference() {
    if (HigTime < MIN_PULSE_TIME || LowTime < MIN_PULSE_TIME) {
        return 1;
    }
    return 0;
}


/**
  * @brief  外部中断初始化
  * @param  无
  * @retval 无
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


//计算四进制数转换成2进制后的1的个数
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
/*--------------------P1 P2时分秒 星期校验-----------------------------*/
	int p3_check=0;
	int p4_check=0;
	int num=0;	
	int i = 0;	
	for(i=1;i<10;i++)
	{
	  num+=NumOf1(BPC_Data[i]);  
	}

	if((BPC_Data[10]==0)||(BPC_Data[10]==2))   //偶数校验  
	{
	  if(num%2==0)	//偶数
	    p3_check=1;  
	}
	else if((BPC_Data[10]==1)||(BPC_Data[10]==3))	//奇数校验
	{
	  if(num%2==1)
	    p3_check=1; 
	}
	
	
	num=0;	
	for(i=11;i<19;i++)
	{
	  num+=NumOf1(BPC_Data[i]);  
	}
	if((BPC_Data[19]==0)||(BPC_Data[19]==2))   //偶数校验  
	{
	  if(num%2==0)	//偶数
	    p4_check=1;  
	}
	else if((BPC_Data[19]==1)||(BPC_Data[19]==3))	//奇数校验
	{
	  if(num%2==1)
	    p4_check=1; 
	}
	if(p3_check==1 && p4_check==1)
	{return 1;}
	else
	{return 0;}		
	
}

//外部中断0中断函数，边缘沿触发执行
void Int0_Routine(void) interrupt 0
{
	
	Delay(1);
	
	LED = 1;            //指示灯
	if(BPC_OUT == 1 && BPC_Flag == 0)   //上升沿触发
	{
	LED = 0;
	if(BPC_State==0)				//状态0，空闲状态
	{
		Timer0_SetCounter(0);	//定时计数器清0
		Timer0_Run(1);			//定时器启动
		BPC_State=1;				//置状态为1
	}
	
	
	
	else if(BPC_State==1)		//状态1，等待开始信号
	{
			if(is_interference())//是干扰信号
			{  
				BPC_State=1;				
			}
			
			else
			{
				BPC_Time = LowTime;
			}
		
		Timer0_SetCounter(0);	//定时计数器清0
		//如果计时为1550-1950毫秒之间，为起始位
		if(BPC_Time>1550 && BPC_Time<1950)
		{
			BPC_pData = 1;
			BPC_State=2;			//置状态为2
		}
		else					//接收出错
		{
			BPC_State=1;			//置状态为1
		}
	}
	
	
	
	
	
	else if(BPC_State==2)		//状态2，接收数据
	{
		if(is_interference())//是干扰信号
		{
			if(LowTime<MIN_PULSE_TIME)
			{
				HigTime += LowTime;//低电平是干扰信号，这段时间本应该是高电平 
				LowTime = 0;
				return;
			}
			else if(HigTime<MIN_PULSE_TIME)
			{
				LowTime += HigTime;//高电平是干扰信号，这段时间本应该是低电平 
				HigTime = 0;
				return;
			}
		}
		BPC_Time=HigTime;	//获取上一次中断到此次中断的时间
		Timer0_SetCounter(0);
		//如果计时为100ms，则接收到了数据0
		if(BPC_Time>100-50 && BPC_Time<100+50)
		{
			BPC_Data[BPC_pData]=0;	
			BPC_pData++;			//数据位置指针自增
		}
		else if(BPC_Time>200-50 && BPC_Time<200+50)
		{
			BPC_Data[BPC_pData]=1;	
			BPC_pData++;			//数据位置指针自增
		}
		else if(BPC_Time>300-50 && BPC_Time<300+50)
		{
			BPC_Data[BPC_pData]=2;	
			BPC_pData++;			//数据位置指针自增
		}
		else if(BPC_Time>400-50 && BPC_Time<400+50)
		{
			BPC_Data[BPC_pData]=3;	
			BPC_pData++;			//数据位置指针自增
		}
 
		else					//接收出错
		{
			BPC_pData=0;			//数据位置指针清0
			BPC_State=1;			//置状态为1
		}

		if(BPC_pData>=20)		//龋果接收到了19位数据
		{
			BPC_Flag = 1;             //对时完成
			Timer0_SetCounter(0);
			BPC_pData=0;			//数据位置指针清0
			Timer0_Run(0);		//定时器停止
			BPC_State=0;			//置状态为0
		}
	}
}
}

void Int0_IRS(void) interrupt 1
{
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	if(BPC_OUT == 1){HigTime++;}
	else{LowTime++;}
}


//时间数组，索引0~6分别为年、月、日、时、分、秒、星期，设置为有符号的便于<0的判断
//char DS1302_Time[]={19,11,16,12,59,55,6};
//解析BPC时间
void BPC_DataHandle(void)
{
	
  if(BPC_Flag==1)       //一帧数据接收完成之后
  {
	  if(BPC_Check()==1)
	  {
		if(BPC_Data[10]==2 || BPC_Data[10]==3)  //下午 
		{BPC_Timer[3]=4*BPC_Data[3]+BPC_Data[4]+12;   }
		else	//上午
		{BPC_Timer[3]=4*BPC_Data[3]+BPC_Data[4]; }    
			BPC_Timer[4]=16*BPC_Data[5]+4*BPC_Data[6]+BPC_Data[7]; 	 //分钟
		switch(BPC_Data[1])	   //秒
		{
			case 0:BPC_Timer[5] = 1;break;
			case 1:BPC_Timer[5] = 21;break;
			case 2:BPC_Timer[5] = 41;break;
		}
		BPC_Timer[0]=16*BPC_Data[16]+4*BPC_Data[17]+BPC_Data[18];   //年 
		BPC_Timer[1]=4*BPC_Data[14]+BPC_Data[15];   //月
		BPC_Timer[2]=16*BPC_Data[11]+4*BPC_Data[12]+BPC_Data[13];   //日 
		BPC_Timer[6]=4*BPC_Data[8]+BPC_Data[9];   //星期 
		BPC_Flag=2;
	}
  else
	{
	BPC_State = 1;
	BPC_Flag = 0;
	}
	}
}




/*数据写入DS1302*/
void BPC_Write_DS1302(void)
{
	if(BPC_Flag == 2)
	{
		int i = 0;
		BPC_PON=1;//接收完成，模块待机
		for(i=0;i<7;i++)
		{
			DS1302_Time[i] = BPC_Timer[i];
		}
		DS1302_Time[5]+=20;
		/*越界判定*/
		if(DS1302_Time[5]>45)
		{
			DS1302_Time[5] = 1;
			DS1302_Time[4]++;
			if(DS1302_Time[4]==60)
			{
				DS1302_Time[4]=0;
				DS1302_Time[3]++;//时
			}
		}
		DS1302_SetTime();
		BPC_Flag = 3;
	}

}

/*每小时自动接收信号，持续十分钟10分钟*/
void BPC_zidong(void)
{
	if(DS1302_Time[4]>=0 && DS1302_Time[4]<=10)
	{
		BPC_Flag = 0;
		BPC_PON=0;
	}
}




