#include <REGX52.H>
#include "QXa51.h"
#include "Delay.h"
#include "Timer0.h"
#include "IndividualKey.h"

unsigned char pwm_left_value=150;//左轮占空比，占空比越高速度越慢。0~170
unsigned char pwm_right_value=150;//右轮占空比，占空比越高速度越慢。0~170
unsigned char pwm_t=0;

unsigned int KeyType=0;


void forward(){
	left_motor_go;
	right_motor_go;
}

void left_hand()
{
	left_motor_stop; 
	right_motor_go;
}

void right_hand()
{
	right_motor_stop;
	left_motor_go;    
}

void backward()
{
	right_motor_back;
	left_motor_back;    
}

void Timer0_Rountine(void) interrupt 1{

	pwm_t++;
	
	//最大占空比设置为256
	if(pwm_t==255){
		pwm_t=EN1=EN2=0;
	}
	
	if(pwm_t==pwm_left_value){
		EN1=1;
	}
	
	if(pwm_t==pwm_right_value){
		EN2=1;
	}
}



void main(){
	
	while(1){
		KeyType=scanIndividualKey();
		if(KeyType==2){
			beep=0;
			DelayMs(200);
			beep=1;
			break;
		}
	}
	
	DelayMs(1000);
	Timer0Init();
	while(1){
		
		if(left_track_led==1&&right_track_led==1){
			forward();
		}
		else{
			if(left_track_led==1&&right_track_led==0){
				left_hand();
			}
			if(left_track_led==0&&right_track_led==1){
				right_hand();
			}
			if(left_track_led==0&&right_track_led==0){
				backward();
			}
		}
	}
	
}