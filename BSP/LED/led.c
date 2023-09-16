#include "led.h"
#include "delay.h"


/*
注意：	LED灯 RGB IO分配//注，C8T6只有pc13可控led
		
				R	PB5
				G	PB6
				B	PB7
				
*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStruct;

	__GPIOC_CLK_ENABLE();

	//GPIOC4/5初始化设置
	GPIO_InitStruct.Pin = GPIO_PIN_13;              //LED对应IO口PA6 PA7
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    //普通推挽输出模式
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  //高速
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //上拉
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);        //初始化GPIO
	

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void LED_Flash(void)
{	
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_6|GPIO_PIN_7);//意思就是电平翻转//？？？？
	delay_ms(1000);
}
void LED_Open(u8 led)
{
	switch(led)
	{
		case LED_R://注，C8T6只有pc13可控led
			
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);	break;
		
		default: break;
	}
}

void LED_Close(u8 led)
{
	switch(led)
	{
		case LED_R://注，C8T6只有pc13可控led
			
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);	break;
		
		default: break;
	}
}

void LED_Toggle(u8 led)
{
	switch(led)
	{
		case LED_R:
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			break;
		default: break;
	}
}
