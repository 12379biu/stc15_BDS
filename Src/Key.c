#include <stc15.H>
#include "Delay.h"

/**
  * @brief  获取独立按键键码
  * @param  无
  * @retval 按下按键的键码，范围：0~4，无按键按下时返回值为0
  */
unsigned char Key()
{
	unsigned char KeyNumber=0;

	if(P23==0){Delay(20);while(P23==0);Delay(20);KeyNumber=1;}
	if(P24==0){Delay(20);while(P24==0);Delay(20);KeyNumber=2;}
	if(P25==0){Delay(20);while(P25==0);Delay(20);KeyNumber=3;}
	if(P26==0){Delay(20);while(P26==0);Delay(20);KeyNumber=4;}
	if(P22==0){Delay(20);while(P22==0);Delay(20);KeyNumber=5;}
	
	return KeyNumber;
}
