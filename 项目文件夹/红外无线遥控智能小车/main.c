#include <reg52.h> 
#include "QXA51.h"



sbit IR  = P3^2;     //��������������ݽӿ�	�ⲿ�ж�O�����

unsigned char IRtime; 		
unsigned char IRcord[4];    
unsigned char IRdata[33];   
bit IRpro_ok, IRok;  

void init()	  
{
	TMOD = 0x02; //��ʱ��0������ʽ2��8λ�Զ���װ
	TH0 = 0x00;  //��8λװ��0��ô��ʱ�����һ�ε�ʱ����256����������
	TL0 = 0x00;
	EA = 1;      
	ET0 = 1;	   
	TR0 = 1;   

	IT0 = 1;	   //�����ⲿ�ж�0Ϊ���ش�����ʽ����һ���½��ش���һ��
	EX0 = 1;	   //�����ⲿ�ж�0
}

void time0() interrupt 1   
{
	IRtime++; 			   //�������1��Ϊ278us
}

void int0() interrupt 0	  		
{
	static unsigned char i;	 			//	������̬�������������������ڻ���ִ�е�ʱ�򲻻ᶪʧ��ֵ��i���ڰ�33�θߵ�ƽ�ĳ���ʱ�����IRdata
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
		 	IRok = 1;				 //��ʾ���������
			i = 0; 				 
		}
	}
	else		  
	{
		IRtime = 0; 				 
		startflag = 1;			 
	}
}

void IRcordpro()   				 //��ȡ����33������������ݽ���
{
	unsigned char i, j, k, cord, value;	/*i���ڴ���4���ֽڣ�j���ڴ���һ���ֽ���ÿһλ��k����33�������е���һλ
	cord����ȡ�������ʱ���ж��Ƿ����1������ʱ��*/
	k = 1; 						//�ӵ�һλ����ʼȡ����������������
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 8; j++)
		{
			cord = IRdata[k];	    //���������cord
			if(cord > 5)	 	
			value = value | 0x80;	
			if(j < 7)
			{
				value = value >> 1;	//valueλ�������ν���8λ���ݡ�
			}
			k++;				//ÿִ��һ������λ��1
		}
		IRcord[i] = value;	   //ÿ������һ���ֽڰ�������IRcord�����С�
		value = 0; 		
	}
	IRpro_ok = 1;				   //������4���ֽں�IRpro ok��1��ʾ����������	
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
	init();	//ִ�г�ʼ����ʱ��0���ⲿ�ж�0
	EN1 = EN2 = 1;//ʹ�����ҵ��
	
	while(1)	
	{
		if(IRok)                    
		{   
			IRcordpro();//������������4���ֽڵ�����
			IRok = 0;	
			if(IRpro_ok) //�ж��Ƿ�������  
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