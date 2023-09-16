
#ifndef __DHT110_H_
#define __DHT110_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

//DQ  PB5
#define DHT_DQ_PORT     GPIOB
#define DHT_DQ_PIN	    GPIO_PIN_5
#define DHT_DQ_PORT_CLK_ENABLE()   __GPIOB_CLK_ENABLE()

#define DHT_BIT_HIGH()  HAL_GPIO_WritePin(DHT_DQ_PORT, DHT_DQ_PIN, GPIO_PIN_SET)	
#define DHT_BIT_LOW()   HAL_GPIO_WritePin(DHT_DQ_PORT, DHT_DQ_PIN, GPIO_PIN_RESET)	
#define DHT_BIT_IN()    HAL_GPIO_ReadPin(DHT_DQ_PORT, DHT_DQ_PIN)

//��ʪ�Ƚṹ��
typedef struct
{
	u8  humi_int;		//ʪ�ȵ���������
	u8  humi_deci;	 	//ʪ�ȵ�С������
	u8  temp_int;	 	//�¶ȵ���������
	u8  temp_deci;	 	//�¶ȵ�С������
	u8  check_sum;	 	//У���		                 
}DHT11_Data_TypeDef;

/*******************************************************************************
* Function Name  : DHT11_Get_TemperAndHumi
* Description    : ��ȡ��ʪ��
* Input          : ��ȡ������ʪ��ֵ
* Output         : None
* Return         : 0 У�����   1  ��ȡ�ɹ�  0xFF  ��λʧ��
* Note			 : None
*******************************************************************************/
u8 DHT11_Get_TemperAndHumi(DHT11_Data_TypeDef *DHT11_Data);

/*******************************************************************************
* Function Name  : DHT11_Init
* Description    : ��ʼ��DHT11,���DHT11�Ƿ����
* Input          : None
* Output         : None
* Return         : ����0��ʾ��λ�ɹ�,����0xFF��ʾʧ��
* Note			 : None
*******************************************************************************/
u8 DHT11_Init(void);

#endif /* __DHT11_H_ */
