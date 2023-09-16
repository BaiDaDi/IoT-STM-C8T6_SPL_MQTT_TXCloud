/***************************** (C) COPYRIGHT ************************************
* File Name          : mqtt.h
* Author             : 星光嵌入式
* Version            : V1.0
* Date               : 06/01/2019
* Description        : mqtt协议驱动程序
* Note               : 版权所有  严禁盗版
********************************************************************************
* 实验平台: 星光STM32开发板
* 淘宝店铺: https://shop148702745.taobao.com/
* 技术论坛: www.feifanembed.com
* QQ群:542830257
********************************************************************************/
#ifndef __MQTT_H_
#define __MQTT_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h" 

//功能码定义
enum
{
	MSG_MQTT_CONN = 0x10,                      //连接
    MSG_MQTT_CONN_ACK = 0x20,                  //连接应答
	MSG_MQTT_PING = 0xC0,                      //心跳
	MSG_MQTT_PING_ACK = 0xD0,                  //心跳应答
	MSG_MQTT_DISCONN = 0xE0,                   //断开连接
	MSG_MQTT_SUBSCRIBE = 0x82,                 //订阅报文
	MSG_MQTT_UNSUBSCRIBE = 0xA2,               //取消订阅
	MSG_MQTT_PUBLISH_QS0 = 0x30,               //发布信息,等级0
	MSG_MQTT_PUBLISH_QS1 = 0x32,               //发布信息,等级1
	MSG_MQTT_PUBLISH_QS2 = 0x34,               //发布信息,等级2
};

#define  MQTT_MAX_TOPIC_LEN   256
#define  MQTT_MAX_TRX_LEN     1024

typedef struct
{
	uint16_t keep_live_time;   //连接保活时间
	
	//收发缓冲区
	uint8_t TX_Buff[MQTT_MAX_TRX_LEN];
	//uint8_t RX_Buff[MQTT_MAX_TRX_LEN];
	uint8_t RX_Topic[MQTT_MAX_TOPIC_LEN];
	uint16_t rx_topic_len;
	
	//mqtt发送回调
	void (*mqtt_Data_Send)(u8 *databuff, u16 len);
	//mqtt接收回调
	void (*mqtt_Data_Recv)(u8 *databuff, u16 len);
	
}_MQTT_INFO;

extern _MQTT_INFO mqtt_info;

/*******************************************************************************
* Function Name  : Mqtt_Init
* Description    : MQTT初始化
* Input          : keep_time   保活时间
				   Data_Send   发送回调
				   Data_Recv   接收回调
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Init(uint16_t keep_time,void (*Data_Send)(u8 *databuff, u16 len),void (*Data_Recv)(u8 *databuff, u16 len));

/*******************************************************************************
* Function Name  : Mqtt_Connect
* Description    : MQTT连接报文
* Input          : client_id   客户端ID
				   usr_name   用户名
				   passward   密码
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Connect(char *client_id, char *usr_name, char *passward);

/*******************************************************************************
* Function Name  : Mqtt_DisConnect
* Description    : MQTT断开连接
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_DisConnect(void);

/*******************************************************************************
* Function Name  : Mqtt_Ping
* Description    : MQTT心跳
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Ping(void);

/*******************************************************************************
* Function Name  : Mqtt_PingAck
* Description    : MQTT心跳应答,服务端调用
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_PingAck(void);

/*******************************************************************************
* Function Name  : Mqtt_Subscribe
* Description    : SUBSCRIBE订阅topic报文 
* Input          : topic_name：订阅topic报文名称  PackID  报文标识符(包号)  
                   QoS：订阅等级        
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Subscribe(char *topic_name, uint16_t PackID, uint8_t QoS);

/*******************************************************************************
* Function Name  : Mqtt_UnSubscribe
* Description    : 取消订阅topic报文 
* Input          : topic_name：订阅topic报文名称  PackID  报文标识符(包号)      
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_UnSubscribe(char *topic_name, uint16_t PackID);

/*******************************************************************************
* Function Name  : Mqtt_Publish
* Description    : 向topic发布报文 
* Input          : topic_name：订阅topic报文名称  PackID  报文标识符(包号)  
                   QoS：等级    data 有效数据   data_len  有效数据长度
* Output         : None
* Return         : None
* Note			 : PackID只有当QoS为1或2时有效
*******************************************************************************/
void Mqtt_Publish(char *topic_name, uint16_t PackID, uint8_t QoS, uint8_t *data, uint16_t data_len);

/*******************************************************************************
* Function Name  : Mqtt_RecvHandle
* Description    : MQTT接收数据解析
* Input          : buff   数据缓冲区
* Output         : None
* Return         : None
* Note			 : 0  成功,其它 失败
*******************************************************************************/
uint8_t Mqtt_RecvHandle(uint8_t *buff);

#endif
