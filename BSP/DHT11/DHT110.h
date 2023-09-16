
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

//温湿度结构体
typedef struct
{
	u8  humi_int;		//湿度的整数部分
	u8  humi_deci;	 	//湿度的小数部分
	u8  temp_int;	 	//温度的整数部分
	u8  temp_deci;	 	//温度的小数部分
	u8  check_sum;	 	//校验和		                 
}DHT11_Data_TypeDef;

/*******************************************************************************
* Function Name  : DHT11_Get_TemperAndHumi
* Description    : 获取温湿度
* Input          : 读取到的温湿度值
* Output         : None
* Return         : 0 校验错误   1  读取成功  0xFF  复位失败
* Note			 : None
*******************************************************************************/
u8 DHT11_Get_TemperAndHumi(DHT11_Data_TypeDef *DHT11_Data);

/*******************************************************************************
* Function Name  : DHT11_Init
* Description    : 初始化DHT11,检测DHT11是否存在
* Input          : None
* Output         : None
* Return         : 返回0表示复位成功,返回0xFF表示失败
* Note			 : None
*******************************************************************************/
u8 DHT11_Init(void);

#endif /* __DHT11_H_ */
