#include "main.h"

sbit buzzer = P2^7;
sbit BPC_PON = P5^4;



void STC15_IO_OutputConfig(void)
{
    P2M0 &= ~0xfc; P2M1 &= ~0xfc;   //按键  蜂鸣器 准双向
    P3M0 = (P3M0 & ~0x10) | 0x60; P3M1 &= ~0x70;       // P3.6/P3.5推挽，P3.4准双向 
    P1M0 = 0xe0; P1M1 = 0x00; 	   //OLED1602
	P0M0 = 0xff; P0M1 = 0x00; 
	P5M0 &= ~0x10; P5M1 &= ~0x10;  //P54准双向，使能

}


void main()
{
	Delay(100);
	STC15_IO_OutputConfig();
	OLED_Init();
	BPC_Init();
	DS1302_Init();
	Timer1Init();
	DS1302_Chong();
	OLED_ShowString(1,1,"20  -  -");//静态字符初始化显示
	OLED_ShowString(2,1,"  :  :  ");
	
//	DS1302_SetTime();//设置时间
	
	while(1)
	{
		KeyNum=Key();//读取键码
		if(KeyNum != 0)
		{
			buzzer = 0;
			Delay(100);
			buzzer = 1;
		}

		if(KeyNum==1)//按键1按下  手动调时
		{
			if(MODE==0){MODE=1;TimeSetSelect=0;}//功能切换
			else if(MODE==1){MODE=0;DS1302_SetTime();}
		}
		switch(MODE)//根据不同的功能执行不同的函数
		{
			case 0:TimeShow();break;
			case 1:TimeSet();break;
		}
		
		
		/*手动授时*/
		if(KeyNum==5)
		{
			BPC_Flag = 0;BPC_PON=0;
		}
		/*每小时自动接收信号，持续十分钟10分钟*/
		BPC_zidong();
		BPC_DataHandle();//数据转化
		BPC_Write_DS1302();//将转化好的数据写入 DS1302_Time[]
		
	}
}


