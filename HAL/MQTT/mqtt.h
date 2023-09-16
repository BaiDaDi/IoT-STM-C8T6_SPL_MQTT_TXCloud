/***************************** (C) COPYRIGHT ************************************
* File Name          : mqtt.h
* Author             : �ǹ�Ƕ��ʽ
* Version            : V1.0
* Date               : 06/01/2019
* Description        : mqttЭ����������
* Note               : ��Ȩ����  �Ͻ�����
********************************************************************************
* ʵ��ƽ̨: �ǹ�STM32������
* �Ա�����: https://shop148702745.taobao.com/
* ������̳: www.feifanembed.com
* QQȺ:542830257
********************************************************************************/
#ifndef __MQTT_H_
#define __MQTT_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h" 

//�����붨��
enum
{
	MSG_MQTT_CONN = 0x10,                      //����
    MSG_MQTT_CONN_ACK = 0x20,                  //����Ӧ��
	MSG_MQTT_PING = 0xC0,                      //����
	MSG_MQTT_PING_ACK = 0xD0,                  //����Ӧ��
	MSG_MQTT_DISCONN = 0xE0,                   //�Ͽ�����
	MSG_MQTT_SUBSCRIBE = 0x82,                 //���ı���
	MSG_MQTT_UNSUBSCRIBE = 0xA2,               //ȡ������
	MSG_MQTT_PUBLISH_QS0 = 0x30,               //������Ϣ,�ȼ�0
	MSG_MQTT_PUBLISH_QS1 = 0x32,               //������Ϣ,�ȼ�1
	MSG_MQTT_PUBLISH_QS2 = 0x34,               //������Ϣ,�ȼ�2
};

#define  MQTT_MAX_TOPIC_LEN   256
#define  MQTT_MAX_TRX_LEN     1024

typedef struct
{
	uint16_t keep_live_time;   //���ӱ���ʱ��
	
	//�շ�������
	uint8_t TX_Buff[MQTT_MAX_TRX_LEN];
	//uint8_t RX_Buff[MQTT_MAX_TRX_LEN];
	uint8_t RX_Topic[MQTT_MAX_TOPIC_LEN];
	uint16_t rx_topic_len;
	
	//mqtt���ͻص�
	void (*mqtt_Data_Send)(u8 *databuff, u16 len);
	//mqtt���ջص�
	void (*mqtt_Data_Recv)(u8 *databuff, u16 len);
	
}_MQTT_INFO;

extern _MQTT_INFO mqtt_info;

/*******************************************************************************
* Function Name  : Mqtt_Init
* Description    : MQTT��ʼ��
* Input          : keep_time   ����ʱ��
				   Data_Send   ���ͻص�
				   Data_Recv   ���ջص�
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Init(uint16_t keep_time,void (*Data_Send)(u8 *databuff, u16 len),void (*Data_Recv)(u8 *databuff, u16 len));

/*******************************************************************************
* Function Name  : Mqtt_Connect
* Description    : MQTT���ӱ���
* Input          : client_id   �ͻ���ID
				   usr_name   �û���
				   passward   ����
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Connect(char *client_id, char *usr_name, char *passward);

/*******************************************************************************
* Function Name  : Mqtt_DisConnect
* Description    : MQTT�Ͽ�����
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_DisConnect(void);

/*******************************************************************************
* Function Name  : Mqtt_Ping
* Description    : MQTT����
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Ping(void);

/*******************************************************************************
* Function Name  : Mqtt_PingAck
* Description    : MQTT����Ӧ��,����˵���
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_PingAck(void);

/*******************************************************************************
* Function Name  : Mqtt_Subscribe
* Description    : SUBSCRIBE����topic���� 
* Input          : topic_name������topic��������  PackID  ���ı�ʶ��(����)  
                   QoS�����ĵȼ�        
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Subscribe(char *topic_name, uint16_t PackID, uint8_t QoS);

/*******************************************************************************
* Function Name  : Mqtt_UnSubscribe
* Description    : ȡ������topic���� 
* Input          : topic_name������topic��������  PackID  ���ı�ʶ��(����)      
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_UnSubscribe(char *topic_name, uint16_t PackID);

/*******************************************************************************
* Function Name  : Mqtt_Publish
* Description    : ��topic�������� 
* Input          : topic_name������topic��������  PackID  ���ı�ʶ��(����)  
                   QoS���ȼ�    data ��Ч����   data_len  ��Ч���ݳ���
* Output         : None
* Return         : None
* Note			 : PackIDֻ�е�QoSΪ1��2ʱ��Ч
*******************************************************************************/
void Mqtt_Publish(char *topic_name, uint16_t PackID, uint8_t QoS, uint8_t *data, uint16_t data_len);

/*******************************************************************************
* Function Name  : Mqtt_RecvHandle
* Description    : MQTT�������ݽ���
* Input          : buff   ���ݻ�����
* Output         : None
* Return         : None
* Note			 : 0  �ɹ�,���� ʧ��
*******************************************************************************/
uint8_t Mqtt_RecvHandle(uint8_t *buff);

#endif
