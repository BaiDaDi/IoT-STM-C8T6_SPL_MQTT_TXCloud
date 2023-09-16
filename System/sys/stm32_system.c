
#include "stm32_system.h"

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
void NVIC_Config(IRQn_Type IRQn, uint32_t PriorityGroup, uint32_t priority)
{
	NVIC_SetPriorityGrouping(PriorityGroup); //设置中断分组
	NVIC_SetPriority(IRQn, priority);	     //设置中断优先级
	NVIC_EnableIRQ(IRQn);	                 //使能中断
}

//系统软复位   
void Sys_Soft_Reset(void)
{   
	SCB->AIRCR =0X05FA0000|(u32)0x04;	  
}

/*******************************************************************************
* Function Name  : System_Init
* Description    : 系统初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void System_Init(void)
{	
	HAL_Init();             //其中配置了SYSTICK,时间为1ms,库函数要用到,这里,我们会重新定义
	SystemClock_Init(RCC_PLL_MUL9,RCC_HSE_PREDIV_DIV1);   //8MHz输入
}

