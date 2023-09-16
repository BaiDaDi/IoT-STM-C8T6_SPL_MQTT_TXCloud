/***************************** (C) COPYRIGHT ************************************
* File Name          : Systick.h
* Author             : �ǹ�Ƕ��ʽ
* Version            : V1.0
* Date               : 08/14/2016
* Description        : ϵͳ����ʱ������Դ�ļ�
* Note               : ��Ȩ����  �Ͻ�����
********************************************************************************
* ʵ��ƽ̨:�ǹ�STM32F103������ 
* �Ա�����: https://shop148702745.taobao.com/
* ������̳: www.feifanembed.com
* QQȺ:542830257
********************************************************************************/
#ifndef __SYSTICK_H_
#define __SYSTICK_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

typedef struct 
{ 
    void (*Operation)(void);  //�ж������еĺ���
}_SysTick_Info;

extern _SysTick_Info SysTick_Info;

/*******************************************************************************
* Function Name  : SysTick_Init
* Description    : SysTick��ʼ��
* Input          : ISR: �жϻص�
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void SysTick_Init(void (*ISR)(void));


#endif /* __SYSTICK_H_ */



