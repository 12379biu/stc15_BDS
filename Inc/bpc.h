#ifndef __BPC_H__
#define __BPC_H__

extern unsigned int BPC_Data[25];
extern unsigned char BPC_Timer[7];
extern unsigned char BPC_Flag;
void BPC_Init(void);
void BPC_DataHandle(void);
void BPC_zidong(void);
void BPC_Write_DS1302(void);
#endif
