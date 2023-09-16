
#ifndef __STM_SYSTEM_H_
#define __STM_SYSTEM_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

/*****************配置晶振*********************/
//#define CLOCK_8MHz              //8MHz晶振
//#define CLOCK_12MHz	            //12MHz晶振
#define CLOCK_16MHz				//16MHz晶振

/*******************************************************************************
* Function Name  : SystemClock_Init
* Description    : STM32F1时钟配置
* Input          : Pll: 输入倍频系数
				   HSE_PreDiv: HSE分频系数
* Output         : None
* Return         : None
* Note			 : 这里使用最常用的8M、12M、16MHz晶振进行配置,其它输入晶振频率
				   需要另行配置,系统时钟为这里为72MHz,SYSCLK = 72MHz
				   HCLK = 72MHz
				   Fs:晶振输入(4-16MHz)(HSE)
				   输入晶振   倍频系数(Pll)   HSE分频 
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
* Description    : STM32F7的L1-Cache使能
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void CPU_CACHE_Enable(void);

/*******************************************************************************
* Function Name  : NVIC_Config
* Description    : NVIC中断配置
* Input          : IRQn:中断号
				   PriorityGroup：优先级分组
				   priority：优先级
* Output         : None
* Return         : None
* Note			 : 调用函数定义在core_cm7.h中, 注意所有中断的优先级分组是一样的
				   优先级分组时针对所有的中断的,此函数是针对外设中断的，不包括
				   系统中断,SysTick
*******************************************************************************/
void NVIC_Config(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t priority);

//系统软复位   
void Sys_Soft_Reset(void);

/*******************************************************************************
* Function Name  : System_Init
* Description    : 系统初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void System_Init(void);


#endif /* __LPC_SYSTEM_H_ */
