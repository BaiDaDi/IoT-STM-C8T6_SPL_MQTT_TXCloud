
#ifndef __DELAY_H_
#define __DELAY_H_ 

#include "stm32f1xx.h"
#include "stm32_types.h"		   

/*******************************************************************************
* Function Name  : delay_us
* Description    : ΢����ʱ��������ʱnus
* Input          : nus:΢����ʱ�ĸ���
* Output         : None
* Return         : None
* Note			 : 72MHz��us��ʱ����
*******************************************************************************/
void delay_us(u32 nus);

/*******************************************************************************
* Function Name  : delay_ms
* Description    : ������ʱ��������ʱnms
* Input          : nms:������ʱ�ĸ���
* Output         : None
* Return         : None
* Note			 : 72MHz��ms��ʱ����
*******************************************************************************/
void delay_ms(u32 ms);

/*******************************************************************************
* Function Name  : delay_ns
* Description    : ns����ʱ����
* Input          : None
* Output         : None
* Return         : None
* Note			 : 72MHz��ns��ʱ����
*******************************************************************************/
void delay_ns(void);


#endif /* __DELAY_H_ */

