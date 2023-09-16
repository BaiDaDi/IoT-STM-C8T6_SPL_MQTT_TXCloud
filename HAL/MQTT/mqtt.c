#include "mqtt.h"
#include "stdlib.h"
#include "string.h"

_MQTT_INFO mqtt_info;

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
void Mqtt_Init(uint16_t keep_time,void (*Data_Send)(u8 *databuff, u16 len),void (*Data_Recv)(u8 *databuff, u16 len))
{
	mqtt_info.keep_live_time = keep_time;
	mqtt_info.mqtt_Data_Send = Data_Send;
	mqtt_info.mqtt_Data_Recv = Data_Recv;
}

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
void Mqtt_Connect(char *client_id, char *usr_name, char *passward)
{
	int temp,Remaining_len;
	u16 Fixed_len,Variable_len,Payload_len;
	u16 client_id_len,usr_name_len,passward_len;
	uint8_t *buff;
	
	//���ݻ�����ָ��
	buff = mqtt_info.TX_Buff;
	
	client_id_len = strlen(client_id);
	usr_name_len = strlen(usr_name);
	passward_len = strlen(passward);
	
	Fixed_len = 1;                                                        //���ӱ����У��̶���ͷ������ʱ��=1
	Variable_len = 10;                                                    //���ӱ����У��ɱ䱨ͷ����=10
	Payload_len = 2 + client_id_len + 2 + usr_name_len + 2 + passward_len; //���ӱ����У����س���      
	Remaining_len = Variable_len + Payload_len;                           //ʣ�೤��=�ɱ䱨ͷ����+���س���
	
	*buff++ = MSG_MQTT_CONN;                       //�̶���ͷ��1���ֽ� ���̶�0x10	
	
	do{                                      //ѭ������̶���ͷ�е�ʣ�೤���ֽڣ��ֽ�������ʣ���ֽڵ���ʵ���ȱ仯
		temp = Remaining_len%128;            //ʣ�೤��ȡ��128
		Remaining_len = Remaining_len/128;   //ʣ�೤��ȡ��128
		if(Remaining_len>0)               	
			temp |= 0x80;                    //��Э��Ҫ��λ7��λ          
		*buff++ = temp;         //ʣ�೤���ֽڼ�¼һ������
		Fixed_len++;	                     //�̶���ͷ�ܳ���+1    
	}while(Remaining_len>0);                 //���Remaining_len>0�Ļ����ٴν���ѭ��
	
	*buff++ = 0x00;    //�ɱ䱨ͷ��1���ֽ� ���̶�0x00	            
	*buff++ = 0x04;    //�ɱ䱨ͷ��2���ֽ� ���̶�0x04
	*buff++ = 0x4D;	   //�ɱ䱨ͷ��3���ֽ� ���̶�0x4D--M
	*buff++ = 0x51;	   //�ɱ䱨ͷ��4���ֽ� ���̶�0x51--Q
	*buff++ = 0x54;	   //�ɱ䱨ͷ��5���ֽ� ���̶�0x54--T
	*buff++ = 0x54;	   //�ɱ䱨ͷ��6���ֽ� ���̶�0x54--T
	*buff++ = 0x04;	   //�ɱ䱨ͷ��7���ֽ� ���̶�0x04,Э�鼶��
	*buff++ = 0xC2;	   //�ɱ䱨ͷ��8���ֽ� ��ʹ���û���������У�飬��ʹ���������������Ự
	*buff++ = mqtt_info.keep_live_time/256;    //�ɱ䱨ͷ��9���ֽ� ������ʱ����ֽ�
	*buff++ = mqtt_info.keep_live_time%256;    //�ɱ䱨ͷ��10���ֽڣ�����ʱ����ֽ�
	
	//�ͻ���ID
	*buff++ = client_id_len/256;             //�ͻ���ID���ȸ��ֽ�
	*buff++ = client_id_len%256;             //�ͻ���ID���ȵ��ֽ�
	memcpy(buff,client_id,client_id_len);    //���ƿͻ���ID
	buff += client_id_len;
	
	//�û���     
	*buff++ = usr_name_len/256; 		     //�û������ȸ��ֽ�
	*buff++ = usr_name_len%256; 			 //�û������ȵ��ֽ�
	memcpy(buff,usr_name,usr_name_len);      //�����û���	
	buff += usr_name_len;
	
	//����
	*buff++ = passward_len/256;			    //���볤�ȸ��ֽ�
	*buff++ = passward_len%256;			    //���볤�ȵ��ֽ�
	memcpy(buff,passward,passward_len);     //���ƹ��������ִ�
	buff += passward_len;

	//��������
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,Fixed_len + Variable_len + Payload_len);
}

/*******************************************************************************
* Function Name  : Mqtt_DisConnect
* Description    : MQTT�Ͽ�����
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_DisConnect(void)
{
	uint8_t *buff;
	
	//���ݻ�����ָ��
	buff = mqtt_info.TX_Buff;
	
	*buff++ = MSG_MQTT_DISCONN;			    //������
	*buff++ = 0x00;			    

	//��������
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,2);
}

/*******************************************************************************
* Function Name  : Mqtt_Ping
* Description    : MQTT����
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Ping(void)
{
	uint8_t *buff;
	
	//���ݻ�����ָ��
	buff = mqtt_info.TX_Buff;
	
	*buff++ = MSG_MQTT_PING;			    //������
	*buff++ = 0x00;			    

	//��������
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,2);
}

/*******************************************************************************
* Function Name  : Mqtt_PingAck
* Description    : MQTT����Ӧ��,����˵���
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_PingAck(void)
{
	uint8_t *buff;
	
	//���ݻ�����ָ��
	buff = mqtt_info.TX_Buff;
	
	*buff++ = MSG_MQTT_PING_ACK;			    //������
	*buff++ = 0x00;			   

	//��������
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,2);
}

/*******************************************************************************
* Function Name  : Mqtt_Subscribe
* Description    : SUBSCRIBE����topic���� 
* Input          : topic_name������topic��������  PackID  ���ı�ʶ��(����)  
                   QoS�����ĵȼ�        
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Subscribe(char *topic_name, uint16_t PackID, uint8_t QoS)
{	
	int temp,Remaining_len;
	u16 topic_len,pack_len,Variable_len,Payload_len;
	uint8_t *buff;
	
	//���ݻ�����ָ��
	buff = mqtt_info.TX_Buff;
	//���ⳤ��
	topic_len = strlen(topic_name);
	Variable_len = 2;                           //SUBSCRIBE������,���ı�ʶ������,�̶�Ϊ2
	Payload_len = 2 + strlen(topic_name) + 1;   //������Ч���ɳ��� = 2�ֽ�(topic_name����)+ topic_name�ַ����ĳ��� + 1�ֽڷ���ȼ�
	Remaining_len = Variable_len + Payload_len; //ʣ�೤��
	
	*buff++ = MSG_MQTT_SUBSCRIBE;                  	     //��1���ֽ� ���̶�0x82        

	do{                                      //ѭ������̶���ͷ�е�ʣ�೤���ֽڣ��ֽ�������ʣ���ֽڵ���ʵ���ȱ仯
		temp = Remaining_len%128;            //ʣ�೤��ȡ��128
		Remaining_len = Remaining_len/128;   //ʣ�೤��ȡ��128
		if(Remaining_len>0)               	
			temp |= 0x80;                    //��Э��Ҫ��λ7��λ          
		*buff++ = temp;         //ʣ�೤���ֽڼ�¼һ������  
	}while(Remaining_len>0);                 //���Remaining_len>0�Ļ����ٴν���ѭ��
	
	*buff++ = PackID/256;                    //���ı�ʶ�����ֽ�,����
	*buff++ = PackID%256;		             //���ı�ʶ�����ֽ�,����
	*buff++ = topic_len/256;                 //topic_name���ȸ��ֽ�
	*buff++ = topic_len%256;		         //topic_name���ȵ��ֽ�
	memcpy(buff,topic_name,topic_len);       //����topic_name	
	buff += topic_len;
	
	*buff++ = QoS;                           //���1���ֽڣ����ĵȼ�
	
	pack_len = buff - mqtt_info.TX_Buff;
	
	//��������
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,pack_len);
}

/*******************************************************************************
* Function Name  : Mqtt_UnSubscribe
* Description    : ȡ������topic���� 
* Input          : topic_name������topic��������  PackID  ���ı�ʶ��(����)      
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_UnSubscribe(char *topic_name, uint16_t PackID)
{	
	int temp,Remaining_len;
	u16 topic_len,pack_len,Variable_len,Payload_len;
	uint8_t *buff;
	
	//���ݻ�����ָ��
	buff = mqtt_info.TX_Buff;
	//���ⳤ��
	topic_len = strlen(topic_name);
	Variable_len = 2;                           //SUBSCRIBE������,���ı�ʶ������,�̶�Ϊ2
	Payload_len = 2 + strlen(topic_name);   //������Ч���ɳ��� = 2�ֽ�(topic_name����)+ topic_name�ַ����ĳ���
	Remaining_len = Variable_len + Payload_len; //ʣ�೤��
	
	*buff++ = MSG_MQTT_UNSUBSCRIBE;                  	     //��1���ֽ� ���̶�0xA2        

	do{                                      //ѭ������̶���ͷ�е�ʣ�೤���ֽڣ��ֽ�������ʣ���ֽڵ���ʵ���ȱ仯
		temp = Remaining_len%128;            //ʣ�೤��ȡ��128
		Remaining_len = Remaining_len/128;   //ʣ�೤��ȡ��128
		if(Remaining_len>0)               	
			temp |= 0x80;                    //��Э��Ҫ��λ7��λ          
		*buff++ = temp;         //ʣ�೤���ֽڼ�¼һ������  
	}while(Remaining_len>0);                 //���Remaining_len>0�Ļ����ٴν���ѭ��
	
	*buff++ = PackID/256;                    //���ı�ʶ�����ֽ�,����
	*buff++ = PackID%256;		             //���ı�ʶ�����ֽ�,����
	*buff++ = topic_len/256;                 //topic_name���ȸ��ֽ�
	*buff++ = topic_len%256;		         //topic_name���ȵ��ֽ�
	memcpy(buff,topic_name,topic_len);       //����topic_name	
	buff += topic_len;
	
	pack_len = buff - mqtt_info.TX_Buff;
	
	//��������
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,pack_len);
}

/*******************************************************************************
* Function Name  : Mqtt_Publish
* Description    : ��topic�������� 
* Input          : topic_name������topic��������  PackID  ���ı�ʶ��(����)  
                   QoS���ȼ�    data ��Ч����   data_len  ��Ч���ݳ���
* Output         : None
* Return         : None
* Note			 : PackIDֻ�е�QoSΪ1��2ʱ��Ч
*******************************************************************************/
void Mqtt_Publish(char *topic_name, uint16_t PackID, uint8_t QoS, uint8_t *data, uint16_t data_len)
{	
	int temp,Remaining_len;
	u16 topic_len,pack_len,Payload_len;
	uint8_t *buff;
	
	//���ݻ�����ָ��
	buff = mqtt_info.TX_Buff;
	//���ⳤ��
	topic_len = strlen(topic_name);
	Payload_len = 2 + strlen(topic_name) + data_len;   //������Ч���ɳ��� = 2�ֽ�(topic_name����)+ topic_name�ַ����ĳ��� + ���ݳ���
	Remaining_len = Payload_len; //ʣ�೤��
	
	if(QoS == 0)
	{
		*buff++ = MSG_MQTT_PUBLISH_QS0;                  	     //��1���ֽ� ���̶�0x30 
	}
	else if(QoS == 1)
	{
		Remaining_len += 2;                      //ʣ�೤��,2Ϊ���ı�ʶ������
		*buff++ = MSG_MQTT_PUBLISH_QS1;                  	     //��1���ֽ� ���̶�0x32 
	}
	else if(QoS == 2)
	{
		Remaining_len += 2;                      //ʣ�೤��,2Ϊ���ı�ʶ������
		*buff++ = MSG_MQTT_PUBLISH_QS2;                  	     //��1���ֽ� ���̶�0x34 
	}

	do{                                      //ѭ������̶���ͷ�е�ʣ�೤���ֽڣ��ֽ�������ʣ���ֽڵ���ʵ���ȱ仯
		temp = Remaining_len%128;            //ʣ�೤��ȡ��128
		Remaining_len = Remaining_len/128;   //ʣ�೤��ȡ��128
		if(Remaining_len>0)               	
			temp |= 0x80;                    //��Э��Ҫ��λ7��λ          
		*buff++ = temp;         //ʣ�೤���ֽڼ�¼һ������  
	}while(Remaining_len>0);                 //���Remaining_len>0�Ļ����ٴν���ѭ��
	
	*buff++ = topic_len/256;                 //topic_name���ȸ��ֽ�
	*buff++ = topic_len%256;		         //topic_name���ȵ��ֽ�
	memcpy(buff,topic_name,topic_len);       //����topic_name	
	buff += topic_len;
	
	if(QoS != 0)  
	{
		*buff++ = PackID/256;                    //���ı�ʶ�����ֽ�,����
		*buff++ = PackID%256;		             //���ı�ʶ�����ֽ�,����
	}
	
	//�����û�����
	memcpy(buff,data,data_len); 
	buff += data_len;
	
	pack_len = buff - mqtt_info.TX_Buff;
	
	//��������
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,pack_len);
}

/*******************************************************************************
* Function Name  : Mqtt_RecvHandle
* Description    : MQTT�������ݽ���
* Input          : buff   ���ݻ�����
* Output         : None
* Return         : None
* Note			 : 0  �ɹ�,���� ʧ��
*******************************************************************************/
uint8_t Mqtt_RecvHandle(uint8_t *buff)
{
	uint8_t *p;
	uint8_t encodeByte = 0;
	uint32_t multiplier = 1, Remaining_len = 0;
	uint8_t QS_level = 0;
	
	p = buff;
	
	//������������
	if((*p != 0x30)&&(*p != 0x32)&&(*p != 0x34))   //���Ƿ�������ͷ
		return 1;
	
	if(*p != 0x30) QS_level = 1;    //���qs�ȼ���Ϊ0
	
	p++;
	//��ȡʣ�����ݳ���
	do{
		encodeByte = *p++;
		Remaining_len += (encodeByte & 0x7F) * multiplier;
		multiplier *= 128;
		
		if(multiplier > 128*128*128) //����ʣ�೤�����4���ֽڵ�Ҫ��,����
			return 2;
	}while((encodeByte & 0x80) != 0);
	
	//��ȡ�������ݳ���
	mqtt_info.rx_topic_len = *p++;
	mqtt_info.rx_topic_len = mqtt_info.rx_topic_len * 256 + *p++;
	//��ȡ����
	memcpy(mqtt_info.RX_Topic,p,mqtt_info.rx_topic_len);
	p += mqtt_info.rx_topic_len;
	
	if(QS_level != 0)  //�������ı�ʶ��
		p += 2;   
	
	//��ȡ�û�������
	if(mqtt_info.mqtt_Data_Recv != NULL)
		mqtt_info.mqtt_Data_Recv(p,Remaining_len - mqtt_info.rx_topic_len - 2);
	
	return 0;
}
