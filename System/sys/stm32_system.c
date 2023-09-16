
#include "stm32_system.h"

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
void SystemClock_Init(uint32_t Pll, uint32_t HSE_PreDiv)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	HAL_StatusTypeDef ret = HAL_OK;

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.HSEPredivValue  = HSE_PreDiv;  
	RCC_OscInitStruct.PLL.PLLMUL      = Pll;
	
	ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1; 

	ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
	if(ret != HAL_OK)
	{
		while(1) { ; }
	}  
}

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
void NVIC_Config(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t priority)
{
	NVIC_SetPriorityGrouping(PriorityGroup); //�����жϷ���
	NVIC_SetPriority(IRQn, priority);	     //�����ж����ȼ�
	NVIC_EnableIRQ(IRQn);	                 //ʹ���ж�
}

//ϵͳ��λ   
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
}

/*******************************************************************************
* Function Name  : System_Init
* Description    : ϵͳ��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void System_Init(void)
{	
	HAL_Init();             //����������SYSTICK,ʱ��Ϊ1ms,�⺯��Ҫ�õ�,����,���ǻ����¶���
	SystemClock_Init(RCC_PLL_MUL9,RCC_HSE_PREDIV_DIV1);   //8MHz����
}

