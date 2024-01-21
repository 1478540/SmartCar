#include <intrins.h>

void DelayMs(unsigned int x_ms)		//@12.000MHz
{

	unsigned char i, j;
	
	while(x_ms--){
		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
	
}

void DelayUs(unsigned int t_us)    	//10us延时函数 启动超声波模块时使用
{ 
	do{ 
		_nop_(); 
	}while(--t_us); 
}
 