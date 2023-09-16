
#ifndef __STM_SYSTEM_H_
#define __STM_SYSTEM_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

/*****************���þ���*********************/
//#define CLOCK_8MHz              //8MHz����
//#define CLOCK_12MHz	            //12MHz����
#define CLOCK_16MHz				//16MHz����

/*******************************************************************************
* Function Name  : SystemClock_Init
* Description    : STM32F1ʱ������
* Input          : Pll: ���뱶Ƶϵ��
				   HSE_PreDiv: HSE��Ƶϵ��
* Output         : None
* Return         : None
* Note			 : ����ʹ����õ�8M��12M��16MHz�����������,�������뾧��Ƶ��
				   ��Ҫ��������,ϵͳʱ��Ϊ����Ϊ72MHz,SYSCLK = 72MHz
				   HCLK = 72MHz
				   Fs:��������(4-16MHz)(HSE)
				   ���뾧��   ��Ƶϵ��(Pll)   HSE��Ƶ 
				   8MHz       RCC_PLL_MUL9	  RCC_HSE_PREDIV_DIV1
				   12MHz      RCC_PLL_MUL6	  RCC_HSE_PREDIV_DIV1
				   16MHz      RCC_PLL_MUL9	  RCC_HSE_PREDIV_DIV2
*******************************************************************************/
void SystemClock_Init(uint32_t Pll, uint32_t HSE_PreDiv);

/*******************************************************************************
* Function Name  : MPU_Config
* Description    : Configure the MPU attributes as Write Through for SRAM1/2
* Input          : None
* Output         : None
* Return         : None
* Note			 : The Base Address is 0x20010000 since this memory interface is the AXI.
         	       The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
*******************************************************************************/
void MPU_Config(void);

/*******************************************************************************
* Function Name  : CPU_CACHE_Enable
* Description    : STM32F7��L1-Cacheʹ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void CPU_CACHE_Enable(void);

/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : NVIC�ж�����
* Input          : IRQn:�жϺ�
				   PriorityGroup�����ȼ�����
				   priority�����ȼ�
* Output         : None
* Return         : None
* Note			 : ���ú���������core_cm7.h��, ע�������жϵ����ȼ�������һ����
				   ���ȼ�����ʱ������е��жϵ�,�˺�������������жϵģ�������
				   ϵͳ�ж�,SysTick
*******************************************************************************/
void NVIC_Config(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t priority);

//ϵͳ��λ   
void Sys_Soft_Reset(void);

/*******************************************************************************
* Function Name  : System_Init
* Description    : ϵͳ��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void System_Init(void);


#endif /* __LPC_SYSTEM_H_ */
