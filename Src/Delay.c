#include <stc15.H>
void Delay(unsigned int xms)
{
	unsigned char i, j;
	while(xms--)
	{
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}


