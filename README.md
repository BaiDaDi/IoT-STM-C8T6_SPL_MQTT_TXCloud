# IoT-STM-C8T6_SPL_MQTT_TXCloud

一种使用标准库架构的STM32F103C8T6通过ESP8266模块以MQTT协议连接腾讯云的解决方案

注：只适合上传dht11，下传点灯这样的简单操作，因为是hal库，所以移植其他的功能不方便，原封不动不改最好

 
参考CSDN：
	https://blog.csdn.net/weixin_43352501/article/details/124304941?ops_request_misc=&request_id=&biz_id=102&utm_term=stm32f103c8t6+8266+%E8%85%BE%E8%AE%AF%E4%BA%91&utm_medium=distribute.pc_search_result.none-task-blog-2~blog~sobaiduweb~default-9-124304941.nonecase


		本工程实例现象：
			实现时间或者其他传感器函数在腾讯云的微信小程序上进行显示；
			微信小程序的控制按钮可以控制开发板上的灯进行闪烁；
		工程适用范围：测试时适用的STM32F103C8T6
		库版本：HAL库函数
		注意：	LED灯 RGB IO分配//注，C8T6只有pc13可控led
  
			ESP-01连接
			TX	PA3//uart2
			RX	PA2
   			EN      3.3V
			RST	PB11（在ESP8266.h中进行宏定义改变）//c8t6可用
			VCC	3.3V
			
			串口打印信息
			USB转TTL
			RXD PA9//uart1
			TXD	PA10
				
		如果进行移植：需要改动的地方，首先如果硬件连接和上述一样，那不需要进行改动；
		在云端连接方面修改#include "tencent_mqtt.h"中的内容
		
		//腾讯云三元组
		#define DEV_NAME "demo"
		#define PRODUCT_KEY "Q7K0Z1GQPK"
		#define DEV_SECRET "UH8+Zozj5RjqNWdoo233KA=="

		//腾讯云主题
		#define SUBSCRIBE_TOPIC  "$thing/down/property/Q7K0Z1GQPK/demo" //订阅主题,腾讯云IOT定义好的
		#define PUBLISH_TOPIC   "$thing/up/property/Q7K0Z1GQPK/demo"    //发布主题,腾讯云IOT定义好的
		
		1.将
		
		#define DEV_NAME "esp12f"
		#define PRODUCT_KEY "35IO4ZV5DK"
		#define DEV_SECRET "5dB6V1qtzS/TrNTBVf0bbw=="
		数据修改为自己账号的三元组数据
		
		2.再将腾讯云主题中的主题内容进行对应修改自己的即可
		
		3.ESP_Client_ConnectServer(0,(u8 *)Server_Addr,"1883","neo5","123123123"); 
		将下方187行处代码，	neo5是笔者手机的热点名称	123123123是笔者手机热点的密码

