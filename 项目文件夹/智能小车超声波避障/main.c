#include <reg52.h> //51ͷ�ļ�
#include <intrins.h>   //����nop��ϵͳ����
#include "QXA51.h"//QX-A51����С�������ļ�
#include "IndividualKey.h"
#include "Delay.h"

sbit RX = P2^0;//ECHO������ģ������
sbit TX = P2^1;//TRIG������ģ�鴥����

unsigned int KeyType;//��ⰴ�µİ�ť


unsigned char pwm_left_val = 100;//����ռ�ձ�ֵ ȡֵ��Χ0-170��0���
unsigned char pwm_right_val = 110;//�ҵ��ռ�ձ�ֵȡֵ��Χ0-170 ,0���

unsigned char pwm_t;//����
unsigned int  time = 0;//����ʱ��
unsigned long S = 0;//����
bit      flag = 0;//����������Χ��־λ



/*��ʱ��0�ж�*/
void timer0() interrupt 1	//T0�ж��������������,������෶Χ
{
	flag=1;							 //�ж������־			 
}

void  StartModule() 		         //����������ģ��
{
	  TX=1;			                     //����һ��ģ��
    DelayUs(20);
	  TX=0;
}


void timer1() interrupt 3
{
	pwm_t++;//���ڼ�ʱ��
	if(pwm_t == 255)
		pwm_t = EN1 = EN2 = 0;
	if(pwm_left_val == pwm_t)//����ռ�ձ�	
		EN1 = 1;		
	if(pwm_right_val == pwm_t)//�ҵ��ռ�ձ�
		EN2 = 1;			 
}


/*С��ǰ��*/
void forward()
{
	left_motor_go; //����ǰ��
	right_motor_go; //�ҵ��ǰ��
}

/*PWM����ʹ�� С������*/
void backward()
{
	left_motor_back; //��������
	right_motor_back; //�ҵ������	
}

/*С��ֹͣ*/
void stop()
{
	right_motor_stop;//�ҵ��ֹͣ
	left_motor_stop; //����ֹͣ	
}

/*PWM����ʹ�� С��������ת*/
void left_rapidly()
{
	left_motor_back;
	right_motor_go;	
}

/*��ʱ��1�ж����PWM�ź�*/


/*�ж�S2�Ƿ񱻰���*/
void keyscan()
{
	for(;;)	//��ѭ��
	{
		KeyType=scanIndividualKey();
		if(KeyType == 2)// ʵʱ���S2�����Ƿ񱻰���
		{
			beep=0;
			DelayMs(200);
			beep=1;
			break;		//�˳�FOR��ѭ��
		}
	}	
}



/*���㳬�����������*/
void Conut(void)
{
	 while(!RX);		//��RX��ECHO�źŻ��죩Ϊ��ʱ�ȴ�
	 TR0=1;			    //��������
	 while(RX);			//��RXΪ1�������ȴ�
	 TR0=0;				//�رռ���
	
	time=TH0*256+TL0;
	TH0=0;
	TL0=0;
	
	S=(float)(time*1.085)*0.17;     //�������MM
	if((S>=7000)||flag==1) //����������Χ
	{	 
		flag=0;
	}
}

/*����������*/
void	Avoid()
{
	if(S < 400)//���ñ��Ͼ��� ����λ����	ɲ������
	{
//		beep = 0;//ʹ�ܷ�����
		stop();//ͣ��
		backward();//����
		DelayMs(50);//����ʱ��Խ��������ԽԶ��������Ϊ����������ת��Ŀռ�
		do{
			left_rapidly();//������ת
			DelayMs(60);//ʱ��Խ�� ת��Ƕ�Խ����ʵ����ʻ�����й�
			stop();//ͣ��
			DelayMs(100);//ʱ��Խ�� ֹͣʱ��Խ�ó�
			StartModule();	//����ģ���࣬�ٴ��ж��Ƿ�
			Conut();			//�������
			
			}while(S < 280);//�ж�ǰ���ϰ������
//		beep = 1;//�رշ�����
	}
	else
	{
		forward();//ǰ��
	}	
}

void AllTimerInit(){
	TMOD |= 0x20;//��ʱ��1����ģʽ2,8λ�Զ���װ�����ڲ���PWM
	TMOD |= 0x01;//��ʱ��0����ģ��1,16λ��ʱģʽ��T0�ò�ECH0���峤��
	TH1 = 220; //
	TL1 = 220; //100HZ T1
	TH0	= 0;
  TL0	= 0;//T0,16λ��ʱ�������ڼ�¼ECHO�ߵ�ƽʱ��         
  ET1	= 1;//����T1�ж�
	ET0 = 1;//����T0�ж�
	TR1 = 1;//������ʱ��1
	EA  = 1;//�������ж�
}

void main()
{

	keyscan();//�ȴ�����S2����С��
	DelayMs(1000);//��ʱ1��
	AllTimerInit();
	while(1)
	{
		 StartModule();	//����ģ����
		 
	   Conut();			//�������
		 Avoid();			//����
		 DelayMs(65);			//�������ڲ�����60MS		  
	}
}