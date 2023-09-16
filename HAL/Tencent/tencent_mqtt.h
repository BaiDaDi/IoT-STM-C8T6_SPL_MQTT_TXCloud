#ifndef __ALIYUN_MQTT_H_
#define __ALIYUN_MQTT_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h" 

enum{MQTT_IDLE = 0, MQTT_RUN, MQTT_WAIT};

//腾讯云三元组
#define DEV_NAME "esp12f"
#define PRODUCT_KEY "35IO4ZV5DK"
#define DEV_SECRET "5dB6V1qtzS/TrNTBVf0bbw=="


//腾讯云主题
#define SUBSCRIBE_TOPIC  "$thing/down/property/35IO4ZV5DK/esp12f" //订阅主题,腾讯云IOT定义好的
#define PUBLISH_TOPIC   "$thing/up/property/35IO4ZV5DK/esp12f"    //发布主题,腾讯云IOT定义好的

//工作模式
enum{MANUAL_MODE = 0, AUTO_MODE};

extern char Server_Addr[128];

/*******************************************************************************
* Function Name  : Tencent_Mqtt_Init
* Description    : 腾讯云平台MQTT初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Tencent_Mqtt_Init(void);

/*******************************************************************************
* Function Name  : Tencent_Mqtt_Connect
* Description    : 腾讯云平台MQTT连接
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Tencent_Mqtt_Connect(void);


#endif
