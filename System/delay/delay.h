
#ifndef __DELAY_H_
#define __DELAY_H_ 

#include "stm32f1xx.h"
#include "stm32_types.h"		   

/*******************************************************************************
* Function Name  : delay_us
* Description    : 微秒延时函数，延时nus
* Input          : nus:微秒延时的个数
* Output         : None
* Return         : None
* Note			 : 72MHz下us延时函数
*******************************************************************************/
void delay_us(u32 nus);

/*******************************************************************************
* Function Name  : delay_ms
* Description    : 毫秒延时函数，延时nms
* Input          : nms:毫秒延时的个数
* Output         : None
* Return         : None
* Note			 : 72MHz下ms延时函数
*******************************************************************************/
void delay_ms(u32 ms);

/*******************************************************************************
* Function Name  : delay_ns
* Description    : ns级延时函数
* Input          : None
* Output         : None
* Return         : None
* Note			 : 72MHz下ns延时函数
*******************************************************************************/
void delay_ns(void);


#endif /* __DELAY_H_ */

