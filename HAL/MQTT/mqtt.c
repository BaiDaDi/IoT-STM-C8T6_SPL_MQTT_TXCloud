#include "mqtt.h"
#include "stdlib.h"
#include "string.h"

_MQTT_INFO mqtt_info;

/*******************************************************************************
* Function Name  : Mqtt_Init
* Description    : MQTT初始化
* Input          : keep_time   保活时间
				   Data_Send   发送回调
				   Data_Recv   接收回调
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
* Description    : MQTT连接报文
* Input          : client_id   客户端ID
				   usr_name   用户名
				   passward   密码
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
	
	//数据缓冲区指针
	buff = mqtt_info.TX_Buff;
	
	client_id_len = strlen(client_id);
	usr_name_len = strlen(usr_name);
	passward_len = strlen(passward);
	
	Fixed_len = 1;                                                        //连接报文中，固定报头长度暂时先=1
	Variable_len = 10;                                                    //连接报文中，可变报头长度=10
	Payload_len = 2 + client_id_len + 2 + usr_name_len + 2 + passward_len; //连接报文中，负载长度      
	Remaining_len = Variable_len + Payload_len;                           //剩余长度=可变报头长度+负载长度
	
	*buff++ = MSG_MQTT_CONN;                       //固定报头第1个字节 ：固定0x10	
	
	do{                                      //循环处理固定报头中的剩余长度字节，字节量根据剩余字节的真实长度变化
		temp = Remaining_len%128;            //剩余长度取余128
		Remaining_len = Remaining_len/128;   //剩余长度取整128
		if(Remaining_len>0)               	
			temp |= 0x80;                    //按协议要求位7置位          
		*buff++ = temp;         //剩余长度字节记录一个数据
		Fixed_len++;	                     //固定报头总长度+1    
	}while(Remaining_len>0);                 //如果Remaining_len>0的话，再次进入循环
	
	*buff++ = 0x00;    //可变报头第1个字节 ：固定0x00	            
	*buff++ = 0x04;    //可变报头第2个字节 ：固定0x04
	*buff++ = 0x4D;	   //可变报头第3个字节 ：固定0x4D--M
	*buff++ = 0x51;	   //可变报头第4个字节 ：固定0x51--Q
	*buff++ = 0x54;	   //可变报头第5个字节 ：固定0x54--T
	*buff++ = 0x54;	   //可变报头第6个字节 ：固定0x54--T
	*buff++ = 0x04;	   //可变报头第7个字节 ：固定0x04,协议级别
	*buff++ = 0xC2;	   //可变报头第8个字节 ：使能用户名和密码校验，不使用遗嘱，不保留会话
	*buff++ = mqtt_info.keep_live_time/256;    //可变报头第9个字节 ：保活时间高字节
	*buff++ = mqtt_info.keep_live_time%256;    //可变报头第10个字节：保活时间高字节
	
	//客户端ID
	*buff++ = client_id_len/256;             //客户端ID长度高字节
	*buff++ = client_id_len%256;             //客户端ID长度低字节
	memcpy(buff,client_id,client_id_len);    //复制客户端ID
	buff += client_id_len;
	
	//用户名     
	*buff++ = usr_name_len/256; 		     //用户名长度高字节
	*buff++ = usr_name_len%256; 			 //用户名长度低字节
	memcpy(buff,usr_name,usr_name_len);      //复制用户名	
	buff += usr_name_len;
	
	//密码
	*buff++ = passward_len/256;			    //密码长度高字节
	*buff++ = passward_len%256;			    //密码长度低字节
	memcpy(buff,passward,passward_len);     //复制过来密码字串
	buff += passward_len;

	//发送数据
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,Fixed_len + Variable_len + Payload_len);
}

/*******************************************************************************
* Function Name  : Mqtt_DisConnect
* Description    : MQTT断开连接
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_DisConnect(void)
{
	uint8_t *buff;
	
	//数据缓冲区指针
	buff = mqtt_info.TX_Buff;
	
	*buff++ = MSG_MQTT_DISCONN;			    //功能码
	*buff++ = 0x00;			    

	//发送数据
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,2);
}

/*******************************************************************************
* Function Name  : Mqtt_Ping
* Description    : MQTT心跳
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Ping(void)
{
	uint8_t *buff;
	
	//数据缓冲区指针
	buff = mqtt_info.TX_Buff;
	
	*buff++ = MSG_MQTT_PING;			    //功能码
	*buff++ = 0x00;			    

	//发送数据
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,2);
}

/*******************************************************************************
* Function Name  : Mqtt_PingAck
* Description    : MQTT心跳应答,服务端调用
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_PingAck(void)
{
	uint8_t *buff;
	
	//数据缓冲区指针
	buff = mqtt_info.TX_Buff;
	
	*buff++ = MSG_MQTT_PING_ACK;			    //功能码
	*buff++ = 0x00;			   

	//发送数据
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,2);
}

/*******************************************************************************
* Function Name  : Mqtt_Subscribe
* Description    : SUBSCRIBE订阅topic报文 
* Input          : topic_name：订阅topic报文名称  PackID  报文标识符(包号)  
                   QoS：订阅等级        
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_Subscribe(char *topic_name, uint16_t PackID, uint8_t QoS)
{	
	int temp,Remaining_len;
	u16 topic_len,pack_len,Variable_len,Payload_len;
	uint8_t *buff;
	
	//数据缓冲区指针
	buff = mqtt_info.TX_Buff;
	//主题长度
	topic_len = strlen(topic_name);
	Variable_len = 2;                           //SUBSCRIBE报文中,报文标识符长度,固定为2
	Payload_len = 2 + strlen(topic_name) + 1;   //计算有效负荷长度 = 2字节(topic_name长度)+ topic_name字符串的长度 + 1字节服务等级
	Remaining_len = Variable_len + Payload_len; //剩余长度
	
	*buff++ = MSG_MQTT_SUBSCRIBE;                  	     //第1个字节 ：固定0x82        

	do{                                      //循环处理固定报头中的剩余长度字节，字节量根据剩余字节的真实长度变化
		temp = Remaining_len%128;            //剩余长度取余128
		Remaining_len = Remaining_len/128;   //剩余长度取整128
		if(Remaining_len>0)               	
			temp |= 0x80;                    //按协议要求位7置位          
		*buff++ = temp;         //剩余长度字节记录一个数据  
	}while(Remaining_len>0);                 //如果Remaining_len>0的话，再次进入循环
	
	*buff++ = PackID/256;                    //报文标识符高字节,包号
	*buff++ = PackID%256;		             //报文标识符低字节,包号
	*buff++ = topic_len/256;                 //topic_name长度高字节
	*buff++ = topic_len%256;		         //topic_name长度低字节
	memcpy(buff,topic_name,topic_len);       //复制topic_name	
	buff += topic_len;
	
	*buff++ = QoS;                           //最后1个字节：订阅等级
	
	pack_len = buff - mqtt_info.TX_Buff;
	
	//发送数据
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,pack_len);
}

/*******************************************************************************
* Function Name  : Mqtt_UnSubscribe
* Description    : 取消订阅topic报文 
* Input          : topic_name：订阅topic报文名称  PackID  报文标识符(包号)      
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Mqtt_UnSubscribe(char *topic_name, uint16_t PackID)
{	
	int temp,Remaining_len;
	u16 topic_len,pack_len,Variable_len,Payload_len;
	uint8_t *buff;
	
	//数据缓冲区指针
	buff = mqtt_info.TX_Buff;
	//主题长度
	topic_len = strlen(topic_name);
	Variable_len = 2;                           //SUBSCRIBE报文中,报文标识符长度,固定为2
	Payload_len = 2 + strlen(topic_name);   //计算有效负荷长度 = 2字节(topic_name长度)+ topic_name字符串的长度
	Remaining_len = Variable_len + Payload_len; //剩余长度
	
	*buff++ = MSG_MQTT_UNSUBSCRIBE;                  	     //第1个字节 ：固定0xA2        

	do{                                      //循环处理固定报头中的剩余长度字节，字节量根据剩余字节的真实长度变化
		temp = Remaining_len%128;            //剩余长度取余128
		Remaining_len = Remaining_len/128;   //剩余长度取整128
		if(Remaining_len>0)               	
			temp |= 0x80;                    //按协议要求位7置位          
		*buff++ = temp;         //剩余长度字节记录一个数据  
	}while(Remaining_len>0);                 //如果Remaining_len>0的话，再次进入循环
	
	*buff++ = PackID/256;                    //报文标识符高字节,包号
	*buff++ = PackID%256;		             //报文标识符低字节,包号
	*buff++ = topic_len/256;                 //topic_name长度高字节
	*buff++ = topic_len%256;		         //topic_name长度低字节
	memcpy(buff,topic_name,topic_len);       //复制topic_name	
	buff += topic_len;
	
	pack_len = buff - mqtt_info.TX_Buff;
	
	//发送数据
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,pack_len);
}

/*******************************************************************************
* Function Name  : Mqtt_Publish
* Description    : 向topic发布报文 
* Input          : topic_name：订阅topic报文名称  PackID  报文标识符(包号)  
                   QoS：等级    data 有效数据   data_len  有效数据长度
* Output         : None
* Return         : None
* Note			 : PackID只有当QoS为1或2时有效
*******************************************************************************/
void Mqtt_Publish(char *topic_name, uint16_t PackID, uint8_t QoS, uint8_t *data, uint16_t data_len)
{	
	int temp,Remaining_len;
	u16 topic_len,pack_len,Payload_len;
	uint8_t *buff;
	
	//数据缓冲区指针
	buff = mqtt_info.TX_Buff;
	//主题长度
	topic_len = strlen(topic_name);
	Payload_len = 2 + strlen(topic_name) + data_len;   //计算有效负荷长度 = 2字节(topic_name长度)+ topic_name字符串的长度 + 数据长度
	Remaining_len = Payload_len; //剩余长度
	
	if(QoS == 0)
	{
		*buff++ = MSG_MQTT_PUBLISH_QS0;                  	     //第1个字节 ：固定0x30 
	}
	else if(QoS == 1)
	{
		Remaining_len += 2;                      //剩余长度,2为报文标识符长度
		*buff++ = MSG_MQTT_PUBLISH_QS1;                  	     //第1个字节 ：固定0x32 
	}
	else if(QoS == 2)
	{
		Remaining_len += 2;                      //剩余长度,2为报文标识符长度
		*buff++ = MSG_MQTT_PUBLISH_QS2;                  	     //第1个字节 ：固定0x34 
	}

	do{                                      //循环处理固定报头中的剩余长度字节，字节量根据剩余字节的真实长度变化
		temp = Remaining_len%128;            //剩余长度取余128
		Remaining_len = Remaining_len/128;   //剩余长度取整128
		if(Remaining_len>0)               	
			temp |= 0x80;                    //按协议要求位7置位          
		*buff++ = temp;         //剩余长度字节记录一个数据  
	}while(Remaining_len>0);                 //如果Remaining_len>0的话，再次进入循环
	
	*buff++ = topic_len/256;                 //topic_name长度高字节
	*buff++ = topic_len%256;		         //topic_name长度低字节
	memcpy(buff,topic_name,topic_len);       //复制topic_name	
	buff += topic_len;
	
	if(QoS != 0)  
	{
		*buff++ = PackID/256;                    //报文标识符高字节,包号
		*buff++ = PackID%256;		             //报文标识符低字节,包号
	}
	
	//复制用户数据
	memcpy(buff,data,data_len); 
	buff += data_len;
	
	pack_len = buff - mqtt_info.TX_Buff;
	
	//发送数据
	if(mqtt_info.mqtt_Data_Send != NULL)
		mqtt_info.mqtt_Data_Send(mqtt_info.TX_Buff,pack_len);
}

/*******************************************************************************
* Function Name  : Mqtt_RecvHandle
* Description    : MQTT接收数据解析
* Input          : buff   数据缓冲区
* Output         : None
* Return         : None
* Note			 : 0  成功,其它 失败
*******************************************************************************/
uint8_t Mqtt_RecvHandle(uint8_t *buff)
{
	uint8_t *p;
	uint8_t encodeByte = 0;
	uint32_t multiplier = 1, Remaining_len = 0;
	uint8_t QS_level = 0;
	
	p = buff;
	
	//解析接收数据
	if((*p != 0x30)&&(*p != 0x32)&&(*p != 0x34))   //不是发布报文头
		return 1;
	
	if(*p != 0x30) QS_level = 1;    //标记qs等级不为0
	
	p++;
	//提取剩余数据长度
	do{
		encodeByte = *p++;
		Remaining_len += (encodeByte & 0x7F) * multiplier;
		multiplier *= 128;
		
		if(multiplier > 128*128*128) //超出剩余长度最大4个字节的要求,错误
			return 2;
	}while((encodeByte & 0x80) != 0);
	
	//提取主题数据长度
	mqtt_info.rx_topic_len = *p++;
	mqtt_info.rx_topic_len = mqtt_info.rx_topic_len * 256 + *p++;
	//提取主题
	memcpy(mqtt_info.RX_Topic,p,mqtt_info.rx_topic_len);
	p += mqtt_info.rx_topic_len;
	
	if(QS_level != 0)  //跳过报文标识符
		p += 2;   
	
	//提取用户年数据
	if(mqtt_info.mqtt_Data_Recv != NULL)
		mqtt_info.mqtt_Data_Recv(p,Remaining_len - mqtt_info.rx_topic_len - 2);
	
	return 0;
}
