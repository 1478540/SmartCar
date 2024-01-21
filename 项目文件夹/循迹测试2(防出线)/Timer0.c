#include <REGX52.H>



void Timer0Init(){
	
	TMOD &= 0xF0;		
	TMOD |= 0x02;		
	//启动8位重装模式
	
	TL0 = 220;		
	TH0 = 220;		
	//目标是设定频率为100HZ，占空比最大值为256
	//经过计算后得出，需要定时器计数36次后溢出
	//也就是说，定时器每次的溢出时间为一份，一共256份为一组，一组的时间总和是0.01ms，即频率100HZ	
	
	TF0 = 0;		
	TR0 = 1;		
	
	ET0=1;
	EA=1;
	
	PT0=0;
}


