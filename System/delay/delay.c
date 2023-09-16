
#include "delay.h"

/*******************************************************************************
* Function Name  : delay_us
* Description    : ΢����ʱ��������ʱnus
* Input          : nus:΢����ʱ�ĸ���
* Output         : None
* Return         : None
* Note			 : 72MHz��us��ʱ����
*******************************************************************************/
void delay_us(u32 nus)    
{ 
	u32 ticks;
	u32 told,tnow,tcnt = 0;
	u32 reload = SysTick->LOAD;				//LOAD��ֵ	  
	
	ticks = nus*72; 						//��Ҫ�Ľ����� 
	told = SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow = SysTick->VAL;	
		if(tnow != told)
		{	    
			if(tnow < told)tcnt += told - tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if(tcnt >= ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}
}

/*******************************************************************************
* Function Name  : delay_ms
* Description    : ������ʱ��������ʱnms
* Input          : nms:������ʱ�ĸ���
* Output         : None
* Return         : None
* Note			 : 72MHz��ms��ʱ����
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
* Description    : ns����ʱ����
* Input          : None
* Output         : None
* Return         : None
* Note			 : 72MHz��ns��ʱ����
*******************************************************************************/
void delay_ns(void)
{
    u8 j; 
	for(j = 0; j < 1; j++);	
}

