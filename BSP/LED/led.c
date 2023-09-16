#include "led.h"
#include "delay.h"


/*
ע�⣺	LED�� RGB IO����//ע��C8T6ֻ��pc13�ɿ�led
		
				R	PB5
				G	PB6
				B	PB7
				
*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStruct;

	__GPIOC_CLK_ENABLE();

	//GPIOC4/5��ʼ������
	GPIO_InitStruct.Pin = GPIO_PIN_13;              //LED��ӦIO��PA6 PA7
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    //��ͨ�������ģʽ
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  //����
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //����
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);        //��ʼ��GPIO
	

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void LED_Flash(void)
{	
	HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_6|GPIO_PIN_7);//��˼���ǵ�ƽ��ת//��������
	delay_ms(1000);
}
void LED_Open(u8 led)
{
	switch(led)
	{
		case LED_R://ע��C8T6ֻ��pc13�ɿ�led
			
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);	break;
		
		default: break;
	}
}

void LED_Close(u8 led)
{
	switch(led)
	{
		case LED_R://ע��C8T6ֻ��pc13�ɿ�led
			
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
