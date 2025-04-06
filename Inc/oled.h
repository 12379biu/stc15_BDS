#ifndef __OLED_H__
#define __OLED_H__

#define uint8_t    unsigned char
#define uint32_t   unsigned int
#define int32_t    signed int 

void OLED_I2C_Init(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

#endif
