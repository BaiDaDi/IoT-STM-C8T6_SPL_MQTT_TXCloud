#include "MyIncludes.h"

u16 urc_cnt = 0, ping_cnt = 0;
u8 urc_update = 0, ping_start = 0, connect_ok = 0;
u16 sys_cnt = 0;
u16 sys_update_rate = 0;

uint16_t data_len = 0;
uint8_t iot_send_buff[512];

uint8_t ctrl_ack = 0;   

extern DHT11_INFO dht11;
u8 flag_DHT11 =0;
float humidity = 0, temperature = 0;

u8 hour=23,min=59,sec=0;//23:59:00

/*******************************************************************************
* Function Name  : systick_isr
* Description    : SysTick中断回调
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void systick_isr(void)
{
	if(sys_cnt < 1000) sys_cnt++;
	else
	{
		sys_cnt = 0;
		LED_Toggle(LED_R);	//定时时间 1S
		sys_update_rate = 1;
		
		//时钟显示1S加一
		if(++sec==60){
			sec=0;
			if(++min==60){
				min=0;
				if(++hour==24)
					hour=0;
			}
		}
	}
	
	if(ping_cnt < 10000) ping_cnt++;
	else
	{
		ping_cnt = 0;
		ping_start = 1;
	}
	
	if(urc_cnt < 3000) urc_cnt++;
	else
	{
		urc_cnt = 0;
		urc_update = 1;
	}
	
	if(esp_connect_ok == ESP_STA_CONNECTED)
	{
		if(esp_recv_timeout < 30) esp_recv_timeout++;
		if(esp_recv_timeout == 20) //认为一帧接收结束
		{
			esp_recv_complete = 1;
		}
	}
}

/*******************************************************************************
* Function Name  : ESP_RecvProcess
* Description    : Uart接收处理
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_RecvProcess(void)
{	
	if(esp_connect_ok == ESP_STA_CONNECTED)
	{
		if(esp_recv_complete == 1)    //接收超时,接收完成
		{
			esp_recv_complete = 0;
			esp_recv_len = esp_rx_index;
			esp_rx_index = 0;
            
            if(esp_recv_buff[0] == MSG_MQTT_CONN_ACK)    //连接应答报文
            {
                if(esp_recv_buff[3] == 0x00)   //连接成功
                {
                    connect_ok = MQTT_RUN;
                    delay_ms(200);
                    //订阅
                    Mqtt_Subscribe(SUBSCRIBE_TOPIC,123,0);
                    
                    printf("MQTT connect ok\r\n");
                }
            }
            else if(esp_recv_buff[0] == MSG_MQTT_PING_ACK)    //心跳应答报文
            {
                
            }
            else
            {
                //接收处理
                Mqtt_RecvHandle(esp_recv_buff);
            }
			
			//打印信息
			USART1_Send_Poll(esp_recv_buff,esp_recv_len);
			ESP_RecvBuff_Clear();
		}
	}
}

int main(void)
{			
    System_Init();    //系统时钟初始化
	LED_Init();	  //led初始化
	SysTick_Init(systick_isr);
	USART1_Init(115200,NULL,NULL);
	ESP_Init();   //WIFI初始化	B_8
	//初始化设备ID,API_KEY
	Tencent_Mqtt_Init(); 
	
	//DHT11温湿度
	DHT11_Init();
	delay_ms(2000);     //最低延时18ms
	flag_DHT11 = Oneline_Start();
	printf("DHT11_Init = %x\r\n",flag_DHT11);
	
    while(1)
    {	
		if(esp_connect_ok != ESP_STA_CONNECTED)  //未建立连接时,进行wifi连网流程
		{
			printf("WIFI DISCONNECTED\r\n");
			ESP_Client_ConnectServer(0,(u8 *)Server_Addr,(u8 *)"1883",(u8 *)"666",(u8 *)"66669999"); 
            connect_ok = MQTT_IDLE;
		}
		else
		{    
			if(connect_ok == MQTT_IDLE)  //没有建立MQTT连接时,发起MQTT连接
			{
				//建立MQTT连接
				Tencent_Mqtt_Connect();
				connect_ok = MQTT_WAIT;
			}
		}
		
		ESP_RecvProcess();
		
		//系统信息采集更新
		if(sys_update_rate == 1)
		{
			sys_update_rate = 0;			
		}
		
		//开关控制应答
		if(ctrl_ack == 1)
		{
			ctrl_ack = 0;
			urc_update = 0;
			urc_cnt = 0;
			memset(iot_send_buff,0,sizeof(iot_send_buff));
			sprintf((char *)iot_send_buff,"{\"method\":\"report\",\"clientToken\":\"18831247139\",\"params\":\
				{\"led1\":0,\"temp\":%d,\"hum\":%d}}",(int)dht11.t,(int)dht11.h); 
			data_len = strlen((char *)iot_send_buff);
			
			if(connect_ok == MQTT_RUN)   //MQTT连接上后,才能发送心跳包
			{
				//发布信息
				Mqtt_Publish(PUBLISH_TOPIC,2,0,iot_send_buff,data_len);
				
				printf("\r\n发布数据:\r\n");
                printf((const char *)iot_send_buff);    //发布的数据打印出来
                printf("\r\n");
			}
		}
		
		//信息上报
		if(urc_update == 1)
		{
			urc_update = 0;
			
			memset(iot_send_buff,0,sizeof(iot_send_buff));
			sprintf((char *)iot_send_buff,"{\"method\":\"report\",\"clientToken\":\"18831247139\",\"params\":\
				{\"led1\":0,\"temp\":%d,\"hum\":%d}}",(int)dht11.t,(int)dht11.h); 
			data_len = strlen((char *)iot_send_buff);
			
			if(connect_ok == MQTT_RUN)   //MQTT连接上后,才能发送心跳包
			{
				//发布信息
				Mqtt_Publish(PUBLISH_TOPIC,2,0,iot_send_buff,data_len);
				printf("\r\n发布数据:\r\n");
                printf((const char *)iot_send_buff);    //发布的数据打印出来
                printf("\r\n");
			}
			
			/********此处采集dht11********************************/
			DHT11_Read();
			printf("temp:%f,hum:%f",dht11.t,dht11.h);
			delay_ms(1000);
		}
		
		if(ping_start == 1)
		{
			ping_start = 0;
			
			if(connect_ok == MQTT_RUN)   //MQTT连接上后,才能发送心跳包
				Mqtt_Ping();     //发送心跳,维持连接
		}
    }     
}


