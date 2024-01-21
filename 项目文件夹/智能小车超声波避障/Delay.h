#ifndef __DELAY_H__
#define __DELAY_H__

/*
	@brief  系统延迟函数，根据你传的参数延迟相应的ms
	@param	参数x_ms为几时，系统就延迟多少毫秒
	@retval 无
*/
void DelayMs(unsigned int x_ms);


/*
	@brief  系统延迟函数，根据你传的参数延迟相应的us
	@param	参数t_us为几时，系统就延迟多少微秒
	@retval 无
*/
void DelayUs(unsigned int t_us);   
#endif