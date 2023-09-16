/***************************** (C) COPYRIGHT ************************************
* File Name          : Systick.h
* Author             : 星光嵌入式
* Version            : V1.0
* Date               : 08/14/2016
* Description        : 系统节拍时钟驱动源文件
* Note               : 版权所有  严禁盗版
********************************************************************************
* 实验平台:星光STM32F103开发板 
* 淘宝店铺: https://shop148702745.taobao.com/
* 技术论坛: www.feifanembed.com
* QQ群:542830257
********************************************************************************/
#ifndef __SYSTICK_H_
#define __SYSTICK_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

typedef struct 
{ 
    void (*Operation)(void);  //中断中运行的函数
}_SysTick_Info;

extern _SysTick_Info SysTick_Info;

/*******************************************************************************
* Function Name  : SysTick_Init
* Description    : SysTick初始化
* Input          : ISR: 中断回调
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void SysTick_Init(void (*ISR)(void));


#endif /* __SYSTICK_H_ */



