#ifndef __DHT11_H
#define __DHT11_H
#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"


#define DQ_PIN 		GPIO_PIN_5 // ����DQ���ź�Ϊ0
#define DQ_PORT 	GPIOB    // ����DQ�������ΪA
#define ONELINE_IN  		HAL_GPIO_ReadPin(DQ_PORT,DQ_PIN) // �궨�����Ŷ�ȡ
#define ONELINE_OUT(PIN) 	HAL_GPIO_WritePin(DQ_PORT,DQ_PIN,PIN) // �궨���������

typedef uint8_t u8;
typedef struct{
    float h;
    float t;
} DHT11_INFO; // DHT11�ṹ�壬���渡�����¶Ⱥ�ʪ��

extern DHT11_INFO dht11;

void DHT11_Init();
void DHT11_Read();
u8 Oneline_Start();

#endif
