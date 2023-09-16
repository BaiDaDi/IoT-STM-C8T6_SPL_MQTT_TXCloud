#include "tencent_mqtt.h"
#include "string.h"
#include "stdlib.h"
#include "esp8266.h"
#include "mqtt.h"
#include "utils_hmac.h"
#include "led.h"

/*************************************************************
�ͻ���ID ��  *|securemode=3,signmethod=hmacsha1|                                                  *�豸����              ע���滻
 
�û���   ��  *&#                                                                                  *�豸���� #ProductKey  ע���滻                 

����     ��  ��DeviceSecret��Ϊ��Կ��clientId*deviceName*productKey#����hmacsha1���ܺ�Ľ��      *�豸���� #ProductKey  ע���滻
*************************************************************/

char ClientID[128];                                          //�ͻ���ID������
char Username[128];                                          //�û���������
char Passward[128];                                          //���뻺����

char Server_Addr[128];     //��������������

extern uint8_t ctrl_ack;

/*******************************************************************************
* Function Name  : Tencent_Mqtt_RecvHandle
* Description    : ��ѶIOT�Ʒ����� MQTT�������ݽ���
* Input          : buff   ���ݻ�����   len  ����
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Tencent_Mqtt_RecvHandle(uint8_t *buff, uint16_t len)
{	
	if(strstr((char *)buff,"\"method\":\"control\"") != 0)        //�����·�ָ��
	{
		if(strstr((char *)buff,"\"LED1\":1") != 0)        //��
		{
			LED_Open(LED_G);
			printf("\r\n�ƴ�\r\n");
			ctrl_ack = 1;    //��ǿ���Ӧ��
		}
		else if(strstr((char *)buff,"\"LED1\":0") != 0)   //��
		{
			LED_Close(LED_G);
			printf("\r\n�ƹر�\r\n");
			ctrl_ack = 1;    //��ǿ���Ӧ��
		}
	}
}

//ƽ����·����ڱ�
u8 const mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/*******************************************************************************
* Function Name  : IsLeapYear
* Description    : �����Ƿ�������
* Input          : nYear: ���
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
uint8_t IsLeapYear(uint16_t nYear)
{
	if((nYear % 4) != 0) 
	{
		return 0;
	}

	if((nYear % 100) != 0) 
	{
		return 1;
	}

	if((nYear % 400) == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/*******************************************************************************
* Function Name  : DateTime_To_Sec
* Description    : ����ʱ������,���������
* Input          : ����ʱ��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
u32 DateTime_To_Sec(u16 Year, u8 Mon, u8 Mday, u8 Hour, u8 Min, u8 Sec) 
{	
	u16 i;
	u32 seccount = 0;
	
	//�������2000-2099
	if(Year > 2099)return 0;	   
	for(i = 1970; i < Year; i++)	//��������ݵ��������
	{
		if(IsLeapYear(i))seccount += 31622400;//�����������
		else seccount += 31536000;			  //ƽ���������
	}
	Mon -= 1;
	for(i = 0; i < Mon; i++)	   //��ǰ���·ݵ����������
	{
		seccount += (u32)mon_table[i]*86400;//�·����������
		if(IsLeapYear(Year)&&i==1)seccount += 86400;//����2�·�����һ���������	   
	}
	seccount += (u32)(Mday-1)*86400;//��ǰ�����ڵ���������� 
	seccount += (u32)Hour*3600;	//Сʱ������
    seccount += (u32)Min*60;	 	//����������
	seccount += Sec;				//�������Ӽ���ȥ	

	return seccount;
}

/*******************************************************************************
* Function Name  : Tencent_Mqtt_Init
* Description    : ��Ѷ��ƽ̨MQTT��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Tencent_Mqtt_Init(void)
{
	uint8_t raw_key[128];
	int raw_len;
	u32 seccount = 0;
	
	//����ʱ��100S,WIFIִ�з���
	Mqtt_Init(100,ESP_Data_Send_FixedLen,Tencent_Mqtt_RecvHandle);
	
	/////////////////////////��ʼ������////////////////////
	//�ͻ���ID
	sprintf(ClientID,"%s%s",PRODUCT_KEY,DEV_NAME);
	//�û���
	seccount = DateTime_To_Sec(2030,12,31,23,59,59);   //����ǩ����Ч��2030��12��31��
	sprintf(Username,"%s%s;12010126;D3VXV;%d",PRODUCT_KEY,DEV_NAME,seccount);
	//���� 
	//��DeviceSecretΪ��Կ��temp�е����ģ�����hmacsha1���ܣ�����������룬�����浽��������
	raw_len = base64_decode(DEV_SECRET,raw_key);   //�ȶ���Կ����base64����
	utils_hmac_sha1(Username,strlen(Username),Passward,(const char *)raw_key,raw_len);   //���ý�������Ľ����username���м���
	strcat(Passward,";hmacsha1");
	
	//��������������
	sprintf(Server_Addr,"%s.iotcloud.tencentdevices.com",PRODUCT_KEY);
}

/*******************************************************************************
* Function Name  : Tencent_Mqtt_Connect
* Description    : ��Ѷ��ƽ̨MQTT����
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Tencent_Mqtt_Connect(void)
{
	//����OneNet
	Mqtt_Connect(ClientID,Username,Passward);
}

