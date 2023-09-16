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
* Description    : SysTick�жϻص�
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
		LED_Toggle(LED_R);	//��ʱʱ�� 1S
		sys_update_rate = 1;
		
		//ʱ����ʾ1S��һ
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
		if(esp_recv_timeout == 20) //��Ϊһ֡���ս���
		{
			esp_recv_complete = 1;
		}
	}
}

/*******************************************************************************
* Function Name  : ESP_RecvProcess
* Description    : Uart���մ���
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void ESP_RecvProcess(void)
{	
	if(esp_connect_ok == ESP_STA_CONNECTED)
	{
		if(esp_recv_complete == 1)    //���ճ�ʱ,�������
		{
			esp_recv_complete = 0;
			esp_recv_len = esp_rx_index;
			esp_rx_index = 0;
            
            if(esp_recv_buff[0] == MSG_MQTT_CONN_ACK)    //����Ӧ����
            {
                if(esp_recv_buff[3] == 0x00)   //���ӳɹ�
                {
                    connect_ok = MQTT_RUN;
                    delay_ms(200);
                    //����
                    Mqtt_Subscribe(SUBSCRIBE_TOPIC,123,0);
                    
                    printf("MQTT connect ok\r\n");
                }
            }
            else if(esp_recv_buff[0] == MSG_MQTT_PING_ACK)    //����Ӧ����
            {
                
            }
            else
            {
                //���մ���
                Mqtt_RecvHandle(esp_recv_buff);
            }
			
			//��ӡ��Ϣ
			USART1_Send_Poll(esp_recv_buff,esp_recv_len);
			ESP_RecvBuff_Clear();
		}
	}
}

int main(void)
{			
    System_Init();    //ϵͳʱ�ӳ�ʼ��
	LED_Init();	  //led��ʼ��
	SysTick_Init(systick_isr);
	USART1_Init(115200,NULL,NULL);
	ESP_Init();   //WIFI��ʼ��	B_8
	//��ʼ���豸ID,API_KEY
	Tencent_Mqtt_Init(); 
	
	//DHT11��ʪ��
	DHT11_Init();
	delay_ms(2000);     //�����ʱ18ms
	flag_DHT11 = Oneline_Start();
	printf("DHT11_Init = %x\r\n",flag_DHT11);
	
    while(1)
    {	
		if(esp_connect_ok != ESP_STA_CONNECTED)  //δ��������ʱ,����wifi��������
		{
			printf("WIFI DISCONNECTED\r\n");
			ESP_Client_ConnectServer(0,(u8 *)Server_Addr,(u8 *)"1883",(u8 *)"666",(u8 *)"66669999"); 
            connect_ok = MQTT_IDLE;
		}
		else
		{    
			if(connect_ok == MQTT_IDLE)  //û�н���MQTT����ʱ,����MQTT����
			{
				//����MQTT����
				Tencent_Mqtt_Connect();
				connect_ok = MQTT_WAIT;
			}
		}
		
		ESP_RecvProcess();
		
		//ϵͳ��Ϣ�ɼ�����
		if(sys_update_rate == 1)
		{
			sys_update_rate = 0;			
		}
		
		//���ؿ���Ӧ��
		if(ctrl_ack == 1)
		{
			ctrl_ack = 0;
			urc_update = 0;
			urc_cnt = 0;
			memset(iot_send_buff,0,sizeof(iot_send_buff));
			sprintf((char *)iot_send_buff,"{\"method\":\"report\",\"clientToken\":\"18831247139\",\"params\":\
				{\"led1\":0,\"temp\":%d,\"hum\":%d}}",(int)dht11.t,(int)dht11.h); 
			data_len = strlen((char *)iot_send_buff);
			
			if(connect_ok == MQTT_RUN)   //MQTT�����Ϻ�,���ܷ���������
			{
				//������Ϣ
				Mqtt_Publish(PUBLISH_TOPIC,2,0,iot_send_buff,data_len);
				
				printf("\r\n��������:\r\n");
                printf((const char *)iot_send_buff);    //���������ݴ�ӡ����
                printf("\r\n");
			}
		}
		
		//��Ϣ�ϱ�
		if(urc_update == 1)
		{
			urc_update = 0;
			
			memset(iot_send_buff,0,sizeof(iot_send_buff));
			sprintf((char *)iot_send_buff,"{\"method\":\"report\",\"clientToken\":\"18831247139\",\"params\":\
				{\"led1\":0,\"temp\":%d,\"hum\":%d}}",(int)dht11.t,(int)dht11.h); 
			data_len = strlen((char *)iot_send_buff);
			
			if(connect_ok == MQTT_RUN)   //MQTT�����Ϻ�,���ܷ���������
			{
				//������Ϣ
				Mqtt_Publish(PUBLISH_TOPIC,2,0,iot_send_buff,data_len);
				printf("\r\n��������:\r\n");
                printf((const char *)iot_send_buff);    //���������ݴ�ӡ����
                printf("\r\n");
			}
			
			/********�˴��ɼ�dht11********************************/
			DHT11_Read();
			printf("temp:%f,hum:%f",dht11.t,dht11.h);
			delay_ms(1000);
		}
		
		if(ping_start == 1)
		{
			ping_start = 0;
			
			if(connect_ok == MQTT_RUN)   //MQTT�����Ϻ�,���ܷ���������
				Mqtt_Ping();     //��������,ά������
		}
    }     
}


