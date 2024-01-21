#include <reg52.h> 
#include "QXA51.h"



sbit IR  = P3^2;     //定义红外脉冲数据接口	外部中断O输入口

unsigned char IRtime; 		
unsigned char IRcord[4];    
unsigned char IRdata[33];   
bit IRpro_ok, IRok;  

void init()	  
{
	TMOD = 0x02; //定时器0工作方式2，8位自动重装
	TH0 = 0x00;  //高8位装入0那么定时器溢出一次的时间是256个机器周期
	TL0 = 0x00;
	EA = 1;      
	ET0 = 1;	   
	TR0 = 1;   

	IT0 = 1;	   //设置外部中断0为跳沿触发方式，来一个下降沿触发一次
	EX0 = 1;	   //启动外部中断0
}

void time0() interrupt 1   
{
	IRtime++; 			   //检测脉宽，1次为278us
}

void int0() interrupt 0	  		
{
	static unsigned char i;	 			//	声明静态变量（在跳出函数后在回来执行的时候不会丢失数值）i用于把33次高电平的持续时间存入IRdata
	static bit startflag;		
	if(startflag)	 			
	{
		if( (IRtime < 53) && (IRtime >= 32) ) 
			i = 0;				 
		IRdata[i] = IRtime;  		
		IRtime = 0;				 
		i++; 					 
		if(i == 33) 				
		{
		 	IRok = 1;				 //表示脉宽检测完毕
			i = 0; 				 
		}
	}
	else		  
	{
		IRtime = 0; 				 
		startflag = 1;			 
	}
}

void IRcordpro()   				 //提取它的33次脉宽进行数据解码
{
	unsigned char i, j, k, cord, value;	/*i用于处理4个字节，j用于处理一个字节中每一位，k用于33次脉宽中的哪一位
	cord用于取出脉宽的时间判断是否符合1的脉宽时间*/
	k = 1; 						//从第一位脉宽开始取，丢弃引导码脉宽
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 8; j++)
		{
			cord = IRdata[k];	    //把脉宽存入cord
			if(cord > 5)	 	
			value = value | 0x80;	
			if(j < 7)
			{
				value = value >> 1;	//value位左移依次接收8位数据。
			}
			k++;				//每执行一次脉宽位加1
		}
		IRcord[i] = value;	   //每处理完一个字节把它放入IRcord数组中。
		value = 0; 		
	}
	IRpro_ok = 1;				   //接收完4个字节后IRpro ok置1表示红外解码完成	
}

void forward()
{
	left_motor_go; 
	right_motor_go; 
}

	

void backward()
{
	left_motor_back; 
	right_motor_back; 	
}



void left_run()
{
	left_motor_stop; 
	right_motor_go; 
}



void right_run()
{
	right_motor_stop;
	left_motor_go;    
}



void stop()
{
	right_motor_stop;
	left_motor_stop; 
}

void main()
{
	init();	//执行初始化定时器0和外部中断0
	EN1 = EN2 = 1;//使能左右电机
	
	while(1)	
	{
		if(IRok)                    
		{   
			IRcordpro();//根据脉宽解码出4个字节的数据
			IRok = 0;	
			if(IRpro_ok) //判断是否解码完毕  
			{
		        switch(IRcord[2])
		   		{
				     case 0x18:  forward(); 			
				             break;
				     case 0x52:  backward();  			
				             break;
				     case 0x08:  left_run(); 			 
				             break;
					 case 0x5A:  right_run(); 			 
				             break;
					 case 0x1C:  stop();			    
				             break;
					 default:break;
		   		}
				IRpro_ok = 0;
			}
		}	
	}
}