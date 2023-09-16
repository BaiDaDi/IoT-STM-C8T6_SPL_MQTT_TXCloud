
#include "delay.h"

/*******************************************************************************
* Function Name  : delay_us
* Description    : 微秒延时函数，延时nus
* Input          : nus:微秒延时的个数
* Output         : None
* Return         : None
* Note			 : 72MHz下us延时函数
*******************************************************************************/
void delay_us(u32 nus)    
{ 
	u32 ticks;
	u32 told,tnow,tcnt = 0;
	u32 reload = SysTick->LOAD;				//LOAD的值	  
	
	ticks = nus*72; 						//需要的节拍数 
	told = SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow < told)tcnt += told - tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if(tcnt >= ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	}
}

/*******************************************************************************
* Function Name  : delay_ms
* Description    : 毫秒延时函数，延时nms
* Input          : nms:毫秒延时的个数
* Output         : None
* Return         : None
* Note			 : 72MHz下ms延时函数
*******************************************************************************/
void delay_ms(u32 ms)    
{ 
	u32 i; 
	for(i = 0; i < ms; i++)
	{ 
		delay_us(1000);
	}
}

/*******************************************************************************
* Function Name  : delay_ns
* Description    : ns级延时函数
* Input          : None
* Output         : None
* Return         : None
* Note			 : 72MHz下ns延时函数
*******************************************************************************/
void delay_ns(void)
{
    u8 j; 
	for(j = 0; j < 1; j++);	
}

