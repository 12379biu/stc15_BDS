#include <stc15.H>

/*定时器0初始化*/
void TimerInit(void)   //1毫秒@11.0592MHz

{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x66;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;		//清除TF0标志
	TR0 = 0;
	ET0=1;
	EA=1;
	PT0=0;
}

/**
  * @brief  定时器1初始化，1毫秒@11.0592MHz
  * @param  无
  * @retval 无
  */
void Timer1Init(void)   
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TMOD |= 0x10;			//设置定时器模式
	TL1 = 0x66;				//设置定时初始值
	TH1 = 0xFC;				//设置定时初始值
	TF1 = 0;		//清除TF0标志
	TR1 = 1;		//定时器0开始计时
	ET1=1;
	EA=1;
	PT1=0;
}

void EX0Init(void)	//外部中断0初始化
{
	IE0 = 0;
	EX0 = 1;
	PX0 = 1; 
	IT0 = 0;  //上升沿下降沿都触发
	EA = 1;	
}

/**
  * @brief  定时器0启动停止控制
  * @param  Flag 启动停止标志，1为启动，0为停止
  * @retval 无
  */
void Timer0_Run(unsigned char Flag)
{
	TR0=Flag;
}

/*定时器中断函数模板
void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
		
	}
}
*/
