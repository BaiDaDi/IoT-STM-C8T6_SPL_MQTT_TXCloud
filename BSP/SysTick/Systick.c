#include "Systick.h"

_SysTick_Info SysTick_Info;

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : SysTick�жϷ�����
* Input          : None
* Output         : None
* Return         : None
* Note			 : 1ms�ж�
*******************************************************************************/
void SysTick_Handler (void)		
{           
	HAL_IncTick();
	if(SysTick_Info.Operation != NULL) SysTick_Info.Operation();
	HAL_SYSTICK_IRQHandler();
}

/*******************************************************************************
* Function Name  : SysTick_Init
* Description    : SysTick��ʼ��
* Input          : ISR: �жϻص�
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
* ����  : HAL_SYSTICK_Callback
* ����  : HAL����SysTick�жϻص�,�ɸ�����Ҫ��Ӵ���
* ����  : None
* ���  : None
* ����  : None
* ע��	: None
*******************************************************************************/
void HAL_SYSTICK_Callback(void)
{
	
}







