#include "Systick.h"

_SysTick_Info SysTick_Info;

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : SysTick中断服务函数
* Input          : None
* Output         : None
* Return         : None
* Note			 : 1ms中断
*******************************************************************************/
void SysTick_Handler (void)		
{           
	HAL_IncTick();
	if(SysTick_Info.Operation != NULL) SysTick_Info.Operation();
	HAL_SYSTICK_IRQHandler();
}

/*******************************************************************************
* Function Name  : SysTick_Init
* Description    : SysTick初始化
* Input          : ISR: 中断回调
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void SysTick_Init(void (*ISR)(void))
{
	SysTick_Info.Operation = ISR;
	//Initialize System Tick with 1ms time interval	
	SysTick_Config(HAL_RCC_GetHCLKFreq()/1000);
}

/*******************************************************************************
* 函数  : HAL_SYSTICK_Callback
* 描述  : HAL库中SysTick中断回调,可根据需要添加代码
* 输入  : None
* 输出  : None
* 返回  : None
* 注意	: None
*******************************************************************************/
void HAL_SYSTICK_Callback(void)
{
	
}







