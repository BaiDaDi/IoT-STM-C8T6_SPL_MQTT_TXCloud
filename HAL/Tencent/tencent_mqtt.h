#ifndef __ALIYUN_MQTT_H_
#define __ALIYUN_MQTT_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h" 

enum{MQTT_IDLE = 0, MQTT_RUN, MQTT_WAIT};

//��Ѷ����Ԫ��
#define DEV_NAME "esp12f"
#define PRODUCT_KEY "35IO4ZV5DK"
#define DEV_SECRET "5dB6V1qtzS/TrNTBVf0bbw=="


//��Ѷ������
#define SUBSCRIBE_TOPIC  "$thing/down/property/35IO4ZV5DK/esp12f" //��������,��Ѷ��IOT����õ�
#define PUBLISH_TOPIC   "$thing/up/property/35IO4ZV5DK/esp12f"    //��������,��Ѷ��IOT����õ�

//����ģʽ
enum{MANUAL_MODE = 0, AUTO_MODE};

extern char Server_Addr[128];

/*******************************************************************************
* Function Name  : Tencent_Mqtt_Init
* Description    : ��Ѷ��ƽ̨MQTT��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Tencent_Mqtt_Init(void);

/*******************************************************************************
* Function Name  : Tencent_Mqtt_Connect
* Description    : ��Ѷ��ƽ̨MQTT����
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Tencent_Mqtt_Connect(void);


#endif
