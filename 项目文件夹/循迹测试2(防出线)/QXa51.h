#ifndef __QXA51_H__
#define __QXA51_H__


//EN1是左电机使能端，EN2是右电机使能端
//当IN1=0 IN2=1时左电机正转，反之逆转；IN1==IN2时不动
//当IN3=1 IN4=0时右电机正转，反之逆转；IN3==IN4时不动
sbit IN1=P1^2;
sbit IN2=P1^3;
sbit EN1=P1^4;
sbit EN2=P1^5;
sbit IN3=P1^6;
sbit IN4=P1^7;


//对小车的各种操作进行宏定义
#define left_motor_en				EN1=1
#define left_motor_stop		EN1=0
#define right_motor_en			EN2=1
#define right_motor_stop		EN2=0
#define left_motor_go				IN1=0,IN2=1
#define left_motor_back			IN1=1,IN2=0
#define right_motor_go			IN3=1,IN4=0
#define right_motor_back		IN3=0,IN4=1


//小车蜂鸣器定义
sbit beep=P2^3;


//小车独立按键定义
sbit key_s2=P3^0;
sbit key_s3=P3^1;
sbit key_s4=P3^2;
sbit key_s5=P3^3;



//小车红外线循迹LED灯定义
//注意，红外线LED灯引脚何小车独立按键s4和s5有冲突
sbit left_track_led=P3^3;
sbit right_track_led=P3^2;

#endif