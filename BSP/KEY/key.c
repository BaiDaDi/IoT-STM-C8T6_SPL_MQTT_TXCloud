#include "key.h"
#include "delay.h"

_KEY_ Key_Info; 

/*******************************************************************************
* Function Name  : Key_Info_Init
* Description    : 按键结构体信息初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Key_Info_Init(void)
{
	Key_Info.Num = 0;
	Key_Info.State = Key_UP;		//按键处于抬起状态
	Key_Info.Num_Last = 0;	
	Key_Info.Key_Delay_Cnt = 0;		//去抖计数器清零
}

/*******************************************************************************
* Function Name  : Key_Init
* Description    : 按键IO初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : FIODIR的每一位对应一个IO口的输入输出配置,1为输出 0为输入
*******************************************************************************/
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  
	__GPIOA_CLK_ENABLE(); //使能GPIOA时钟
	__GPIOC_CLK_ENABLE(); //使能GPIOA时钟
  
    //配置PA0 PA4 PA5为输入,按键
    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;  
    GPIO_InitStruct.Mode = GPIO_PULLUP;           //输入
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;               //下拉  
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;          //高速 
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;  
    GPIO_InitStruct.Pull = GPIO_PULLUP;               //上拉  
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	Key_Info_Init();		//按键信息初始化
}

/*******************************************************************************
* Function Name  : Independent_Key_Scan
* Description    : 独立式按键扫描
* Input          : None
* Output         : None
* Return         : 键值,为0表示无键按下
* Note			 : 只获取键值,不做任何去抖处理
*******************************************************************************/
u8 Independent_Key_Scan(void)
{
//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)   //KEY1按键按下
//	{
//		return KEY_ONE;	
//	}
	
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET)   //KEY2按键按下
	{
		return KEY_TWO;	
	}
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)   //KEY3按键按下
	{
		return KEY_THREE;	
	}
	
	return 0;
}  

/*******************************************************************************
* Function Name  : Key_Read
* Description    : 按键读取,不支持多个按键同时按下
* Input          : None
* Output         : None
* Return         : None
* Note			 : 此函数支持按键按下触发和按键按下抬起触发,需要配置宏
				   KEY_TRIGGER_MODE选择触发方式
*******************************************************************************/
void Key_Read(void)
{
	u8 Key_CurrNum;		//当前按键值
	
	//delay_ms(1);        //每隔1ms扫描一次按键
	
	Key_CurrNum = 0;	//每次读之前将上次读到的值清除

	Key_CurrNum = Independent_Key_Scan();	//获取按键	

	if((Key_Info.Shake_LastNum == Key_CurrNum)&&(Key_CurrNum != 0))	//同一个按键按下
	{
		Key_Info.Key_Delay_Cnt++; 			//去抖计数
	#if KEY_TRIGGER_MODE == DOWN_TRIGGER	//按键按下触发
		if(Key_Info.Key_Delay_Cnt > KEY_SHAKE_DELAY)	//按键有效，这里可以作按键按下触发一次
		{
			Key_Info.Key_Delay_Cnt = KEY_SHAKE_DELAY + 1;			//清零计数器
			Key_Info.State = Key_KEEP;			//按键保持
		}
		
		if(Key_Info.Key_Delay_Cnt == KEY_SHAKE_DELAY)
		{
			Key_Info.Num = Key_Info.Shake_LastNum;	//按键值清零
			Key_Info.State = Key_DOWN;			    //按键按下
		}
	#endif	
	}
	else		//按键松开，这里可以作按键松开触发，可以根据按键按下的长短进行不同的触发，即长按、短按松开触发
	{
	#if KEY_TRIGGER_MODE == DOWN_TRIGGER	//按键按下触发
		Key_Info.Key_Delay_Cnt = 0;		//清零计数器
		Key_Info.Num = 0;				//按键值清零
		Key_Info.State = Key_UP; 		//按键抬起
		goto KEY_END;

	#elif KEY_TRIGGER_MODE == UP_TRIGGER	//按键抬起触发
		if(Key_Info.Key_Delay_Cnt < KEY_SHAKE_DELAY)	//按键无效
		{
			Key_Info.Key_Delay_Cnt = 0;		//清零计数器
			Key_Info.Num = 0;				//按键值清零
			Key_Info.State = Key_UP; 		//按键抬起
			goto KEY_END;
		}
		else	//按键有效
		{
			Key_Info.Key_Delay_Cnt = 0;			//清零计数器
			Key_Info.Num = Key_Info.Shake_LastNum;	//记录按键值
			Key_Info.State = Key_UP;
			goto KEY_END;
		}
	#endif
	}

KEY_END:
	Key_Info.Shake_LastNum = Key_CurrNum;	//记录按键值
}


