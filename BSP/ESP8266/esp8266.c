#include "esp8266.h"
#include "Usart.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"

uint16_t esp_rx_index = 0,esp_recv_len,esp_recv_complete;
uint8_t esp_recv_timeout = 0;              //接收超时
uint8_t esp_recv_buff[RECV_MAX_LEN];
uint8_t esp_connect_ok = ESP_STA_DISCONNECTED;

uint8_t esp_airkiss_use = 0;    //微信配网使用标志,1--使用  
uint8_t esp_airkiss_start = 1;  //启动配网标志,1--启动配网

/*******************************************************************************
* Function Name  : ESP_RX_ISR
* Description    : 串口接收处理函数,在接收中断中调用
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_RX_ISR(u8 RX_Data)
{
	if(esp_rx_index < RECV_MAX_LEN)
	{
		esp_recv_buff[esp_rx_index++] = RX_Data;
	}
	
	esp_recv_timeout = 0;      //超时计数器清零
}

/*******************************************************************************
* Function Name  : ESP_Init
* Description    : WIFI模块初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;  
	
	//使能GPIO端口时钟
	ESP_RST_GPIO_ClK_ENABLE();
	
	//KEY 输出
	GPIO_InitStruct.Pin = ESP_RST_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  		//输出
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ESP_RST_GPIO_PORT, &GPIO_InitStruct);
	
	//串口初始化
	USART2_Init(115200, NULL, ESP_RX_ISR);
	
	esp_recv_timeout = 30;
	esp_rx_index = 0;
}

/*******************************************************************************
* Function Name  : ESP_Send
* Description    : WIFI发送
* Input          : buff  待发送数据    len      发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Send(uint8_t *buff, uint16_t len)
{
	USART2_Send_Poll(buff,len);
}

/*******************************************************************************
* Function Name  : ESP_UartSend
* Description    : WIFI串口AT指令发送,用户移植接口
* Input          : s 待发送字符串
* Output         : None
* Return         : None
* Note			 : 不能发送0x00
*******************************************************************************/
void ESP_UartSend(unsigned char *s) 
{
  	USART2_SendString((char *)s);
}

/*******************************************************************************
* Function Name  : ESP_RecvBuff_Clear
* Description    : 清空接收缓存
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_RecvBuff_Clear(void)
{
	u16 i;
	
	esp_rx_index = 0;
	for(i = 0; i < sizeof(esp_recv_buff);i++)
		esp_recv_buff[i] = 0;
}

/*******************************************************************************
* Function Name  : ESP_str_find
* Description    : 查找匹配字符串
* Input          : buff   查找缓存    str  要查找匹配的字符串
* Output         : None
* Return         : 0  找到匹配   1  未找到匹配
* Note			 : None
*******************************************************************************/
u8 ESP_str_find(u8 *buff, u8 *str)
{
	u8 *p,*q;
	
	p=buff;
	q=str;
	while(*p != '\0')
	{
		if(*p == *q)
		{
			q++;
			if(*q == '\0')   //匹配结束
			{
				return 0;
			}
		}
		else
		{
//			if(q != str)   //表示已经匹配字符
//			{
//				if(*q != '\0')  //没有匹配结束
//				{
//					return 1;  //返回错误
//				}
//			}
			q=str;
		}
		p++;
	}
	return 1;
}

/*******************************************************************************
* Function Name  : ESP_strstr
* Description    : 查找匹配字符串
* Input          : buff   查找缓存    str  要查找匹配的字符串  buff_len  查找缓存长度
* Output         : None
* Return         : 0  找到匹配   1  未找到匹配
* Note			 : None
*******************************************************************************/
u8 ESP_strstr(u8 *buff, u8 *str, u16 buff_len)
{
	u8 *p,*q;
	u16 i;
	
	p=buff;
	q=str;
	//while(*p != '\0')
	for(i = 0; i < buff_len; i++)
	{
		if(*p == *q)
		{
			q++;
			if(*q == '\0')   //匹配结束
			{
				return 0;
			}
		}
		else
		{
//			if(q != str)   //表示已经匹配字符
//			{
//				if(*q != '\0')  //没有匹配结束
//				{
//					return 1;  //返回错误
//				}
//			}
			q=str;
		}
		p++;
	}
	return 1;
}

/*******************************************************************************
* Function Name  : ESP_Cmd_Send
* Description    : WIFI模块命令发送
* Input          : cmd  命令字符串   ack  应答字符串  timeout  超时时间,单位毫秒
* Output         : None
* Return         : ESP_ACK_OK  收到应答    ESP_ACK_ERR   无应答
* Note			 : None
*******************************************************************************/
u8 ESP_Cmd_Send(u8 *cmd, u8 *ack, u16 timeout)
{
	u8 res = ESP_ACK_ERR;
	
	ESP_RecvBuff_Clear();             //清空应答缓存
	
	delay_ms(10);
	ESP_UartSend(cmd);         //发送命令

	//等待应答
	if(ack == NULL) 
	{
		res = ESP_ACK_OK;
		goto CMD_END;   //无应答
	}
	else
	{
		while(--timeout)
		{
			delay_ms(1);    //延时1mS,等待接收完成
			if(ESP_str_find(esp_recv_buff,(u8 *)ack) == ESP_ACK_OK)  //收到应答
			{
				res = ESP_ACK_OK;
				break;    //跳出循环
			}
			else
			{
				res = ESP_ACK_ERR;
			}
		}
	}
	
CMD_END:
	return res;
}
/*******************************************************************************
* Function Name  : ESP_HardReset
* Description    : 硬件复位WIFI模块
* Input          : timeout 超时时间
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
u8 ESP_HardReset(u16 timeout)
{
	u8 res = ESP_ACK_ERR;
	
	//硬件复位
	ESP_RST_LOW();
	delay_ms(500);
	ESP_RST_HIGH();
	
	while(--timeout)
	{
		delay_ms(1);    //延时1mS,等待接收完成
		if(ESP_strstr(esp_recv_buff,(u8 *)"ready",esp_rx_index) == ESP_ACK_OK)  //收到应答
		{
			res = ESP_ACK_OK;
			break;    //跳出循环
		}
		else
		{
			res = ESP_ACK_ERR;
		}
	}

	return res;
}

/*******************************************************************************
* Function Name  : ESP_Smartconfig_Check
* Description    : WIFI模块,配网检测
* Input          : timeout 超时时间
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
u8 ESP_Smartconfig_Check(u16 timeout)
{
	u8 res = ESP_ACK_ERR;
	
	while(--timeout)
	{
		delay_ms(1);    //延时1mS,等待接收完成
		if(ESP_strstr(esp_recv_buff,(u8 *)"smartconfig connected wifi",esp_rx_index) == ESP_ACK_OK)  //收到应答
		{
			res = ESP_ACK_OK;
			break;    //跳出循环
		}
		else
		{
			res = ESP_ACK_ERR;
		}
	}

	return res;
}

const char *wifi_mode[2]={"TCP","UDP"};//连接模式
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
uint8_t ESP_Client_ConnectServer(u8 mode, u8 *addr_dns, u8 *port, u8 *ap_ssid, u8 *ap_password)
{	
	char buff[100];
	
	esp_connect_ok = ESP_STA_DISCONNECTED;   //断开网络
	
	//硬件复位
	if(ESP_HardReset(5000) != ESP_ACK_OK)
	{
		ESP_RecvBuff_Clear();             //清空应答缓存
		return 1;
	}
	
	printf("RESET OK\r\n");
	if(ESP_Cmd_Send((u8 *)"ATE0\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 2;
	printf("ATE0 OK\r\n");
	//设置模式STA
	if(ESP_Cmd_Send((u8 *)"AT+CWMODE=1\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 3;
	printf("AT+CWMODE OK\r\n");
    
    if(esp_airkiss_use == 0)
	{
		//加入路由器
		memset(buff,0,100); //清零
		sprintf((char*)buff,"AT+CWJAP=\"%s\",\"%s\"\r\n",ap_ssid,ap_password);
		if(ESP_Cmd_Send((u8 *)buff,(u8 *)"WIFI GOT IP\r\n\r\nOK",20000) != ESP_ACK_OK) return 4;
		printf("Join AP OK\r\n");
	}
	else
	{
		if(esp_airkiss_start == 1)
		{
			esp_airkiss_start = 0;  //清除配网启动标志
			//设置自动连接
			if(ESP_Cmd_Send((u8 *)"AT+CWAUTOCONN=1\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 9;  
			printf("CWAUTOCONN OK\r\n");
			//启动配网
			printf("Airkiss Start\r\n");
			if(ESP_Cmd_Send((u8 *)"AT+CWSTARTSMART\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 10;  
			//等待配网完成,超时1分钟
			printf("Waitting.......\r\n");
			if(ESP_Smartconfig_Check(60000) == ESP_ACK_OK)
			{
				printf("Airkiss Success\r\n");
			}
			else
			{
				printf("Airkiss Fail\r\n");
			}
			//停止配网
			if(ESP_Cmd_Send((u8 *)"AT+CWSTOPSMART\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 11;
			printf("Airkiss Stop\r\n");
		}
		else   //不启动配网时,要查询WIFI是否连接上路由器
		{
//			if(ESP_Cmd_Send((u8 *)buff,(u8 *)"WIFI GOT IP\r\n\r\nOK",20000) != ESP_ACK_OK) return 4;
//			printf("Join AP OK\r\n");
			
			delay_ms(3000);   //延时3秒等待模块自动连接上路由
		}
	}
    
	//获取IP
	if(ESP_Cmd_Send((u8 *)"AT+CIFSR\r\n",(u8 *)"OK",500) != ESP_ACK_OK) return 5;
	printf((const char *)esp_recv_buff);
	//开启透传,5S超时
	if(ESP_Cmd_Send((u8 *)"AT+CIPMODE=1\r\n",(u8 *)"OK",5000) != ESP_ACK_OK) return 6;
	printf("AT+CIPMODE OK\r\n");
	//连接服务器,超时10S
	memset(buff,0,100); //清零
	sprintf((char*)buff,"AT+CIPSTART=\"%s\",\"%s\",%s\r\n",wifi_mode[mode],addr_dns,port);
	if(ESP_Cmd_Send((u8 *)buff,(u8 *)"OK",10000) != ESP_ACK_OK) return 7;
	printf("NET Connect OK\r\n");
	
	//进入透传,5S超时
	if(ESP_Cmd_Send((u8 *)"AT+CIPSEND\r\n",(u8 *)"\r\nOK\r\n\r\n>",5000) != ESP_ACK_OK) return 8;
	printf("AT+CIPSEND OK\r\n");
	
	esp_recv_timeout = 30;
	esp_rx_index = 0;
	ESP_RecvBuff_Clear();
	
	esp_connect_ok = ESP_STA_CONNECTED;  //标记连接上网络
	
	return 0;
}

/*******************************************************************************
* Function Name  : ESP_Data_Send_FixedLen
* Description    : ESP数据发送,指定长度方式
* Input          : databuff: 待发送数据缓存    len: 发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Data_Send_FixedLen(u8 *databuff, u16 len)
{
	if(esp_connect_ok != ESP_STA_CONNECTED) return;

	 //发送数据
	ESP_Send(databuff,len);
}

