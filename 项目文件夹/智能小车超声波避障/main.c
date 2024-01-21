#include <reg52.h> //51头文件
#include <intrins.h>   //包含nop等系统函数
#include "QXA51.h"//QX-A51智能小车配置文件
#include "IndividualKey.h"
#include "Delay.h"

sbit RX = P2^0;//ECHO超声波模块回响端
sbit TX = P2^1;//TRIG超声波模块触发端

unsigned int KeyType;//检测按下的按钮


unsigned char pwm_left_val = 100;//左电机占空比值 取值范围0-170，0最快
unsigned char pwm_right_val = 110;//右电机占空比值取值范围0-170 ,0最快

unsigned char pwm_t;//周期
unsigned int  time = 0;//传输时间
unsigned long S = 0;//距离
bit      flag = 0;//超出测量范围标志位



/*定时器0中断*/
void timer0() interrupt 1	//T0中断用来计数器溢出,超过测距范围
{
	flag=1;							 //中断溢出标志			 
}

void  StartModule() 		         //启动超声波模块
{
	  TX=1;			                     //启动一次模块
    DelayUs(20);
	  TX=0;
}


void timer1() interrupt 3
{
	pwm_t++;//周期计时加
	if(pwm_t == 255)
		pwm_t = EN1 = EN2 = 0;
	if(pwm_left_val == pwm_t)//左电机占空比	
		EN1 = 1;		
	if(pwm_right_val == pwm_t)//右电机占空比
		EN2 = 1;			 
}


/*小车前进*/
void forward()
{
	left_motor_go; //左电机前进
	right_motor_go; //右电机前进
}

/*PWM控制使能 小车后退*/
void backward()
{
	left_motor_back; //左电机后退
	right_motor_back; //右电机后退	
}

/*小车停止*/
void stop()
{
	right_motor_stop;//右电机停止
	left_motor_stop; //左电机停止	
}

/*PWM控制使能 小车高速左转*/
void left_rapidly()
{
	left_motor_back;
	right_motor_go;	
}

/*定时器1中断输出PWM信号*/


/*判断S2是否被按下*/
void keyscan()
{
	for(;;)	//死循环
	{
		KeyType=scanIndividualKey();
		if(KeyType == 2)// 实时检测S2按键是否被按下
		{
			beep=0;
			DelayMs(200);
			beep=1;
			break;		//退出FOR死循环
		}
	}	
}



/*计算超声波所测距离*/
void Conut(void)
{
	 while(!RX);		//当RX（ECHO信号回响）为零时等待
	 TR0=1;			    //开启计数
	 while(RX);			//当RX为1计数并等待
	 TR0=0;				//关闭计数
	
	time=TH0*256+TL0;
	TH0=0;
	TL0=0;
	
	S=(float)(time*1.085)*0.17;     //算出来是MM
	if((S>=7000)||flag==1) //超出测量范围
	{	 
		flag=0;
	}
}

/*超声波避障*/
void	Avoid()
{
	if(S < 400)//设置避障距离 ，单位毫米	刹车距离
	{
//		beep = 0;//使能蜂鸣器
		stop();//停车
		backward();//后退
		DelayMs(50);//后退时间越长、距离越远。后退是为了留出车辆转向的空间
		do{
			left_rapidly();//高速左转
			DelayMs(60);//时间越长 转向角度越大，与实际行驶环境有关
			stop();//停车
			DelayMs(100);//时间越长 停止时间越久长
			StartModule();	//启动模块测距，再次判断是否
			Conut();			//计算距离
			
			}while(S < 280);//判断前面障碍物距离
//		beep = 1;//关闭蜂鸣器
	}
	else
	{
		forward();//前进
	}	
}

void AllTimerInit(){
	TMOD |= 0x20;//定时器1工作模式2,8位自动重装。用于产生PWM
	TMOD |= 0x01;//定时器0工作模块1,16位定时模式。T0用测ECH0脉冲长度
	TH1 = 220; //
	TL1 = 220; //100HZ T1
	TH0	= 0;
  TL0	= 0;//T0,16位定时计数用于记录ECHO高电平时间         
  ET1	= 1;//允许T1中断
	ET0 = 1;//允许T0中断
	TR1 = 1;//启动定时器1
	EA  = 1;//启动总中断
}

void main()
{

	keyscan();//等待按下S2启动小车
	DelayMs(1000);//延时1秒
	AllTimerInit();
	while(1)
	{
		 StartModule();	//启动模块测距
		 
	   Conut();			//计算距离
		 Avoid();			//避障
		 DelayMs(65);			//测试周期不低于60MS		  
	}
}