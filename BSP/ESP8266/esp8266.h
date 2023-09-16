
#ifndef __ESP8266_H_
#define __ESP8266_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

//Ӧ��
enum
{
	ESP_ACK_OK = 0,
	ESP_ACK_ERR = 1,
};

//��ɫ
enum
{
	ESP_STA_ROLE = 1,   //�ͻ���ģʽ
	ESP_AP_ROLE = 2,    //·��ģʽ
	ESP_STA_AP_ROLE = 3,    //�ͻ���+·��ģʽ
};

enum
{
	ESP_STA_DISCONNECTED = 0,   //����Ͽ�
	ESP_STA_CONNECTED = 1,      //��������
};


#define ESP_RST_GPIO_ClK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE() 
#define ESP_RST_GPIO_PORT               GPIOB
#define ESP_RST_PIN                     GPIO_PIN_11
#define ESP_RST_HIGH()				 	HAL_GPIO_WritePin(ESP_RST_GPIO_PORT, ESP_RST_PIN, GPIO_PIN_SET)
#define ESP_RST_LOW()					HAL_GPIO_WritePin(ESP_RST_GPIO_PORT, ESP_RST_PIN, GPIO_PIN_RESET)

//���������ջ��泤��
#define RECV_MAX_LEN   1024

extern uint16_t esp_rx_index,esp_recv_len,esp_recv_complete;
extern uint8_t esp_recv_timeout;              //���ճ�ʱ
extern uint8_t esp_recv_buff[RECV_MAX_LEN];
extern uint8_t esp_connect_ok;

extern uint8_t esp_airkiss_use;    //΢������ʹ�ñ�־,1--ʹ��  
extern uint8_t esp_airkiss_start;  //����������־,1--��������

/*******************************************************************************
* Function Name  : ESP_Init
* Description    : WIFIģ���ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Init(void);

/*******************************************************************************
* Function Name  : ESP_Cmd_Send
* Description    : WIFIģ�������
* Input          : cmd  �����ַ���   ack  Ӧ���ַ���  timeout  ��ʱʱ��,��λ����
* Output         : None
* Return         : ESP_ACK_OK  �յ�Ӧ��    ESP_ACK_ERR   ��Ӧ��
* Note			 : None
*******************************************************************************/
u8 ESP_Cmd_Send(u8 *cmd, u8 *ack, u16 timeout);

/*******************************************************************************
* Function Name  : HC05_Send
* Description    : hc05����
* Input          : buff  ����������    len      �������ݳ���
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Send(uint8_t *buff, uint16_t len);

/*******************************************************************************
* Function Name  : ESP_RecvBuff_Clear
* Description    : ��ս��ջ���
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_RecvBuff_Clear(void);

/*******************************************************************************
* Function Name  : ESP_Client_ConnectServer
* Description    : WIFI�ͻ�������,����·��͸��
* Input          : mode: 0 TCP  1 UDP   
				   addr_dns: IP��ַ������
				   port: �˿ں�
				   ap_ssid: ·��������
				   ap_password:·��������
* Output         : None
* Return         : ת�����ֵ
* Note			 : None
*******************************************************************************/
uint8_t ESP_Client_ConnectServer(u8 mode, u8 *addr_dns, u8 *port, u8 *ap_ssid, u8 *ap_password);

/*******************************************************************************
* Function Name  : ESP_Data_Send_FixedLen
* Description    : ESP���ݷ���,ָ�����ȷ�ʽ
* Input          : databuff: ���������ݻ���    len: �������ݳ���
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Data_Send_FixedLen(u8 *databuff, u16 len);

#endif
