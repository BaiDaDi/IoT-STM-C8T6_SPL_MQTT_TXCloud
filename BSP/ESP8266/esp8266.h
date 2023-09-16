
#ifndef __ESP8266_H_
#define __ESP8266_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

//应答
enum
{
	ESP_ACK_OK = 0,
	ESP_ACK_ERR = 1,
};

//角色
enum
{
	ESP_STA_ROLE = 1,   //客户端模式
	ESP_AP_ROLE = 2,    //路由模式
	ESP_STA_AP_ROLE = 3,    //客户端+路由模式
};

enum
{
	ESP_STA_DISCONNECTED = 0,   //网络断开
	ESP_STA_CONNECTED = 1,      //网络连接
};


#define ESP_RST_GPIO_ClK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE() 
#define ESP_RST_GPIO_PORT               GPIOB
#define ESP_RST_PIN                     GPIO_PIN_11
#define ESP_RST_HIGH()				 	HAL_GPIO_WritePin(ESP_RST_GPIO_PORT, ESP_RST_PIN, GPIO_PIN_SET)
#define ESP_RST_LOW()					HAL_GPIO_WritePin(ESP_RST_GPIO_PORT, ESP_RST_PIN, GPIO_PIN_RESET)

//定义最大接收缓存长度
#define RECV_MAX_LEN   1024

extern uint16_t esp_rx_index,esp_recv_len,esp_recv_complete;
extern uint8_t esp_recv_timeout;              //接收超时
extern uint8_t esp_recv_buff[RECV_MAX_LEN];
extern uint8_t esp_connect_ok;

extern uint8_t esp_airkiss_use;    //微信配网使用标志,1--使用  
extern uint8_t esp_airkiss_start;  //启动配网标志,1--启动配网

/*******************************************************************************
* Function Name  : ESP_Init
* Description    : WIFI模块初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Init(void);

/*******************************************************************************
* Function Name  : ESP_Cmd_Send
* Description    : WIFI模块命令发送
* Input          : cmd  命令字符串   ack  应答字符串  timeout  超时时间,单位毫秒
* Output         : None
* Return         : ESP_ACK_OK  收到应答    ESP_ACK_ERR   无应答
* Note			 : None
*******************************************************************************/
u8 ESP_Cmd_Send(u8 *cmd, u8 *ack, u16 timeout);

/*******************************************************************************
* Function Name  : HC05_Send
* Description    : hc05发送
* Input          : buff  待发送数据    len      发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Send(uint8_t *buff, uint16_t len);

/*******************************************************************************
* Function Name  : ESP_RecvBuff_Clear
* Description    : 清空接收缓存
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_RecvBuff_Clear(void);

/*******************************************************************************
* Function Name  : ESP_Client_ConnectServer
* Description    : WIFI客户端连接,单链路非透传
* Input          : mode: 0 TCP  1 UDP   
				   addr_dns: IP地址或域名
				   port: 端口号
				   ap_ssid: 路由器名称
				   ap_password:路由器密码
* Output         : None
* Return         : 转换后的值
* Note			 : None
*******************************************************************************/
uint8_t ESP_Client_ConnectServer(u8 mode, u8 *addr_dns, u8 *port, u8 *ap_ssid, u8 *ap_password);

/*******************************************************************************
* Function Name  : ESP_Data_Send_FixedLen
* Description    : ESP数据发送,指定长度方式
* Input          : databuff: 待发送数据缓存    len: 发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Data_Send_FixedLen(u8 *databuff, u16 len);

#endif
