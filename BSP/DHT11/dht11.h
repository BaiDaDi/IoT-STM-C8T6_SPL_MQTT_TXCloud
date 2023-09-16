#ifndef __DHT11_H
#define __DHT11_H
#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"


#define DQ_PIN 		GPIO_PIN_5 // 设置DQ引脚号为0
#define DQ_PORT 	GPIOB    // 设置DQ引脚类别为A
#define ONELINE_IN  		HAL_GPIO_ReadPin(DQ_PORT,DQ_PIN) // 宏定义引脚读取
#define ONELINE_OUT(PIN) 	HAL_GPIO_WritePin(DQ_PORT,DQ_PIN,PIN) // 宏定义引脚输出

typedef uint8_t u8;
typedef struct{
    float h;
    float t;
} DHT11_INFO; // DHT11结构体，储存浮点数温度和湿度

extern DHT11_INFO dht11;

void DHT11_Init();
void DHT11_Read();
u8 Oneline_Start();

#endif
