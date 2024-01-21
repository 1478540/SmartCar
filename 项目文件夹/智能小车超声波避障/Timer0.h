#ifndef __TIMER0_H__
#define __TIMER0_H__

/*
	@brief  关于定时器T0的1ms定时配置初始化
	@param   无
	@retval 无
*/
void Timer0Init();


/*
	@brief  关于定时器T0的 Timer0中断处理函数
			这串代码不包装成函数，可以根据实际需要修改并复制到主函数中
	@param  无
	@retval 无
*/
//void Timer0_Rountine(void) interrupt 1{
//	static unsigned int Count=0;
//	
//	Count++;
//	
//	TL0 = 0x18;		
//	TH0 = 0xFC;		
//	
//	if(Count>=1000){
//		
//		//TODO
//
//		Count=0;
//		
//	}
//}


#endif