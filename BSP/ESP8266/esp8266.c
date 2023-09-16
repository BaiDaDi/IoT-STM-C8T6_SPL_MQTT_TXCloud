#include "esp8266.h"
#include "Usart.h"
#include "delay.h"
#include "string.h"
#include "stdio.h"

uint16_t esp_rx_index = 0,esp_recv_len,esp_recv_complete;
uint8_t esp_recv_timeout = 0;              //���ճ�ʱ
uint8_t esp_recv_buff[RECV_MAX_LEN];
uint8_t esp_connect_ok = ESP_STA_DISCONNECTED;

uint8_t esp_airkiss_use = 0;    //΢������ʹ�ñ�־,1--ʹ��  
uint8_t esp_airkiss_start = 1;  //����������־,1--��������

/*******************************************************************************
* Function Name  : ESP_RX_ISR
* Description    : ���ڽ��մ�����,�ڽ����ж��е���
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
	
	esp_recv_timeout = 0;      //��ʱ����������
}

/*******************************************************************************
* Function Name  : ESP_Init
* Description    : WIFIģ���ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;  
	
	//ʹ��GPIO�˿�ʱ��
	ESP_RST_GPIO_ClK_ENABLE();
	
	//KEY ���
	GPIO_InitStruct.Pin = ESP_RST_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  		//���
	GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(ESP_RST_GPIO_PORT, &GPIO_InitStruct);
	
	//���ڳ�ʼ��
	USART2_Init(115200, NULL, ESP_RX_ISR);
	
	esp_recv_timeout = 30;
	esp_rx_index = 0;
}

/*******************************************************************************
* Function Name  : ESP_Send
* Description    : WIFI����
* Input          : buff  ����������    len      �������ݳ���
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
* Description    : WIFI����ATָ���,�û���ֲ�ӿ�
* Input          : s �������ַ���
* Output         : None
* Return         : None
* Note			 : ���ܷ���0x00
*******************************************************************************/
void ESP_UartSend(unsigned char *s) 
{
  	USART2_SendString((char *)s);
}

/*******************************************************************************
* Function Name  : ESP_RecvBuff_Clear
* Description    : ��ս��ջ���
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
* Description    : ����ƥ���ַ���
* Input          : buff   ���һ���    str  Ҫ����ƥ����ַ���
* Output         : None
* Return         : 0  �ҵ�ƥ��   1  δ�ҵ�ƥ��
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
			if(*q == '\0')   //ƥ�����
			{
				return 0;
			}
		}
		else
		{
//			if(q != str)   //��ʾ�Ѿ�ƥ���ַ�
//			{
//				if(*q != '\0')  //û��ƥ�����
//				{
//					return 1;  //���ش���
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
* Description    : ����ƥ���ַ���
* Input          : buff   ���һ���    str  Ҫ����ƥ����ַ���  buff_len  ���һ��泤��
* Output         : None
* Return         : 0  �ҵ�ƥ��   1  δ�ҵ�ƥ��
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
			if(*q == '\0')   //ƥ�����
			{
				return 0;
			}
		}
		else
		{
//			if(q != str)   //��ʾ�Ѿ�ƥ���ַ�
//			{
//				if(*q != '\0')  //û��ƥ�����
//				{
//					return 1;  //���ش���
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
* Description    : WIFIģ�������
* Input          : cmd  �����ַ���   ack  Ӧ���ַ���  timeout  ��ʱʱ��,��λ����
* Output         : None
* Return         : ESP_ACK_OK  �յ�Ӧ��    ESP_ACK_ERR   ��Ӧ��
* Note			 : None
*******************************************************************************/
u8 ESP_Cmd_Send(u8 *cmd, u8 *ack, u16 timeout)
{
	u8 res = ESP_ACK_ERR;
	
	ESP_RecvBuff_Clear();             //���Ӧ�𻺴�
	
	delay_ms(10);
	ESP_UartSend(cmd);         //��������

	//�ȴ�Ӧ��
	if(ack == NULL) 
	{
		res = ESP_ACK_OK;
		goto CMD_END;   //��Ӧ��
	}
	else
	{
		while(--timeout)
		{
			delay_ms(1);    //��ʱ1mS,�ȴ��������
			if(ESP_str_find(esp_recv_buff,(u8 *)ack) == ESP_ACK_OK)  //�յ�Ӧ��
			{
				res = ESP_ACK_OK;
				break;    //����ѭ��
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
* Description    : Ӳ����λWIFIģ��
* Input          : timeout ��ʱʱ��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
u8 ESP_HardReset(u16 timeout)
{
	u8 res = ESP_ACK_ERR;
	
	//Ӳ����λ
	ESP_RST_LOW();
	delay_ms(500);
	ESP_RST_HIGH();
	
	while(--timeout)
	{
		delay_ms(1);    //��ʱ1mS,�ȴ��������
		if(ESP_strstr(esp_recv_buff,(u8 *)"ready",esp_rx_index) == ESP_ACK_OK)  //�յ�Ӧ��
		{
			res = ESP_ACK_OK;
			break;    //����ѭ��
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
* Description    : WIFIģ��,�������
* Input          : timeout ��ʱʱ��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
u8 ESP_Smartconfig_Check(u16 timeout)
{
	u8 res = ESP_ACK_ERR;
	
	while(--timeout)
	{
		delay_ms(1);    //��ʱ1mS,�ȴ��������
		if(ESP_strstr(esp_recv_buff,(u8 *)"smartconfig connected wifi",esp_rx_index) == ESP_ACK_OK)  //�յ�Ӧ��
		{
			res = ESP_ACK_OK;
			break;    //����ѭ��
		}
		else
		{
			res = ESP_ACK_ERR;
		}
	}

	return res;
}

const char *wifi_mode[2]={"TCP","UDP"};//����ģʽ
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
uint8_t ESP_Client_ConnectServer(u8 mode, u8 *addr_dns, u8 *port, u8 *ap_ssid, u8 *ap_password)
{	
	char buff[100];
	
	esp_connect_ok = ESP_STA_DISCONNECTED;   //�Ͽ�����
	
	//Ӳ����λ
	if(ESP_HardReset(5000) != ESP_ACK_OK)
	{
		ESP_RecvBuff_Clear();             //���Ӧ�𻺴�
		return 1;
	}
	
	printf("RESET OK\r\n");
	if(ESP_Cmd_Send((u8 *)"ATE0\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 2;
	printf("ATE0 OK\r\n");
	//����ģʽSTA
	if(ESP_Cmd_Send((u8 *)"AT+CWMODE=1\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 3;
	printf("AT+CWMODE OK\r\n");
    
    if(esp_airkiss_use == 0)
	{
		//����·����
		memset(buff,0,100); //����
		sprintf((char*)buff,"AT+CWJAP=\"%s\",\"%s\"\r\n",ap_ssid,ap_password);
		if(ESP_Cmd_Send((u8 *)buff,(u8 *)"WIFI GOT IP\r\n\r\nOK",20000) != ESP_ACK_OK) return 4;
		printf("Join AP OK\r\n");
	}
	else
	{
		if(esp_airkiss_start == 1)
		{
			esp_airkiss_start = 0;  //�������������־
			//�����Զ�����
			if(ESP_Cmd_Send((u8 *)"AT+CWAUTOCONN=1\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 9;  
			printf("CWAUTOCONN OK\r\n");
			//��������
			printf("Airkiss Start\r\n");
			if(ESP_Cmd_Send((u8 *)"AT+CWSTARTSMART\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 10;  
			//�ȴ��������,��ʱ1����
			printf("Waitting.......\r\n");
			if(ESP_Smartconfig_Check(60000) == ESP_ACK_OK)
			{
				printf("Airkiss Success\r\n");
			}
			else
			{
				printf("Airkiss Fail\r\n");
			}
			//ֹͣ����
			if(ESP_Cmd_Send((u8 *)"AT+CWSTOPSMART\r\n",(u8 *)"OK",200) != ESP_ACK_OK) return 11;
			printf("Airkiss Stop\r\n");
		}
		else   //����������ʱ,Ҫ��ѯWIFI�Ƿ�������·����
		{
//			if(ESP_Cmd_Send((u8 *)buff,(u8 *)"WIFI GOT IP\r\n\r\nOK",20000) != ESP_ACK_OK) return 4;
//			printf("Join AP OK\r\n");
			
			delay_ms(3000);   //��ʱ3��ȴ�ģ���Զ�������·��
		}
	}
    
	//��ȡIP
	if(ESP_Cmd_Send((u8 *)"AT+CIFSR\r\n",(u8 *)"OK",500) != ESP_ACK_OK) return 5;
	printf((const char *)esp_recv_buff);
	//����͸��,5S��ʱ
	if(ESP_Cmd_Send((u8 *)"AT+CIPMODE=1\r\n",(u8 *)"OK",5000) != ESP_ACK_OK) return 6;
	printf("AT+CIPMODE OK\r\n");
	//���ӷ�����,��ʱ10S
	memset(buff,0,100); //����
	sprintf((char*)buff,"AT+CIPSTART=\"%s\",\"%s\",%s\r\n",wifi_mode[mode],addr_dns,port);
	if(ESP_Cmd_Send((u8 *)buff,(u8 *)"OK",10000) != ESP_ACK_OK) return 7;
	printf("NET Connect OK\r\n");
	
	//����͸��,5S��ʱ
	if(ESP_Cmd_Send((u8 *)"AT+CIPSEND\r\n",(u8 *)"\r\nOK\r\n\r\n>",5000) != ESP_ACK_OK) return 8;
	printf("AT+CIPSEND OK\r\n");
	
	esp_recv_timeout = 30;
	esp_rx_index = 0;
	ESP_RecvBuff_Clear();
	
	esp_connect_ok = ESP_STA_CONNECTED;  //�������������
	
	return 0;
}

/*******************************************************************************
* Function Name  : ESP_Data_Send_FixedLen
* Description    : ESP���ݷ���,ָ�����ȷ�ʽ
* Input          : databuff: ���������ݻ���    len: �������ݳ���
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_Data_Send_FixedLen(u8 *databuff, u16 len)
{
	if(esp_connect_ok != ESP_STA_CONNECTED) return;

	 //��������
	ESP_Send(databuff,len);
}

