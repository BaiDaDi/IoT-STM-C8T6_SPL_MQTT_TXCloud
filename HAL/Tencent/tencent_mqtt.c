#include "tencent_mqtt.h"
#include "string.h"
#include "stdlib.h"
#include "esp8266.h"
#include "mqtt.h"
#include "utils_hmac.h"
#include "led.h"

/*************************************************************
客户端ID ：  *|securemode=3,signmethod=hmacsha1|                                                  *设备名称              注意替换
 
用户名   ：  *&#                                                                                  *设备名称 #ProductKey  注意替换                 

密码     ：  用DeviceSecret做为秘钥对clientId*deviceName*productKey#进行hmacsha1加密后的结果      *设备名称 #ProductKey  注意替换
*************************************************************/

char ClientID[128];                                          //客户端ID缓冲区
char Username[128];                                          //用户名缓冲区
char Passward[128];                                          //密码缓冲区

char Server_Addr[128];     //服务器域名缓存

extern uint8_t ctrl_ack;

/*******************************************************************************
* Function Name  : Tencent_Mqtt_RecvHandle
* Description    : 腾讯IOT云服务器 MQTT接收数据解析
* Input          : buff   数据缓冲区   len  长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Tencent_Mqtt_RecvHandle(uint8_t *buff, uint16_t len)
{	
	if(strstr((char *)buff,"\"method\":\"control\"") != 0)        //控制下发指令
	{
		if(strstr((char *)buff,"\"LED1\":1") != 0)        //开
		{
			LED_Open(LED_G);
			printf("\r\n灯打开\r\n");
			ctrl_ack = 1;    //标记控制应答
		}
		else if(strstr((char *)buff,"\"LED1\":0") != 0)   //关
		{
			LED_Close(LED_G);
			printf("\r\n灯关闭\r\n");
			ctrl_ack = 1;    //标记控制应答
		}
	}
}

//平年的月份日期表
u8 const mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/*******************************************************************************
* Function Name  : IsLeapYear
* Description    : 计算是否是闰年
* Input          : nYear: 年份
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
* Description    : 根据时间日期,计算出秒数
* Input          : 日期时间
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
u32 DateTime_To_Sec(u16 Year, u8 Mon, u8 Mday, u8 Hour, u8 Min, u8 Sec) 
{	
	u16 i;
	u32 seccount = 0;
	
	//年份设置2000-2099
	if(Year > 2099)return 0;	   
	for(i = 1970; i < Year; i++)	//把所有年份的秒钟相加
	{
		if(IsLeapYear(i))seccount += 31622400;//闰年的秒钟数
		else seccount += 31536000;			  //平年的秒钟数
	}
	Mon -= 1;
	for(i = 0; i < Mon; i++)	   //把前面月份的秒钟数相加
	{
		seccount += (u32)mon_table[i]*86400;//月份秒钟数相加
		if(IsLeapYear(Year)&&i==1)seccount += 86400;//闰年2月份增加一天的秒钟数	   
	}
	seccount += (u32)(Mday-1)*86400;//把前面日期的秒钟数相加 
	seccount += (u32)Hour*3600;	//小时秒钟数
    seccount += (u32)Min*60;	 	//分钟秒钟数
	seccount += Sec;				//最后的秒钟加上去	

	return seccount;
}

/*******************************************************************************
* Function Name  : Tencent_Mqtt_Init
* Description    : 腾讯云平台MQTT初始化
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
	
	//保活时间100S,WIFI执行发送
	Mqtt_Init(100,ESP_Data_Send_FixedLen,Tencent_Mqtt_RecvHandle);
	
	/////////////////////////初始化参数////////////////////
	//客户端ID
	sprintf(ClientID,"%s%s",PRODUCT_KEY,DEV_NAME);
	//用户名
	seccount = DateTime_To_Sec(2030,12,31,23,59,59);   //设置签名有效期2030年12月31号
	sprintf(Username,"%s%s;12010126;D3VXV;%d",PRODUCT_KEY,DEV_NAME,seccount);
	//密码 
	//以DeviceSecret为秘钥对temp中的明文，进行hmacsha1加密，结果就是密码，并保存到缓冲区中
	raw_len = base64_decode(DEV_SECRET,raw_key);   //先对秘钥进行base64解码
	utils_hmac_sha1(Username,strlen(Username),Passward,(const char *)raw_key,raw_len);   //再用解码出来的结果对username进行加密
	strcat(Passward,";hmacsha1");
	
	//构建服务器域名
	sprintf(Server_Addr,"%s.iotcloud.tencentdevices.com",PRODUCT_KEY);
}

/*******************************************************************************
* Function Name  : Tencent_Mqtt_Connect
* Description    : 腾讯云平台MQTT连接
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Tencent_Mqtt_Connect(void)
{
	//连接OneNet
	Mqtt_Connect(ClientID,Username,Passward);
}

