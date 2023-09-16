#include "DHT110.h"
#include "delay.h"

//DQ����Ϊ���
void DHT_DIR_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	//GPIOB5��ʼ������
	GPIO_InitStruct.Pin = DHT_DQ_PIN;             
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    //��ͨ�������ģʽ
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;       //����
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //����
	HAL_GPIO_Init(DHT_DQ_PORT, &GPIO_InitStruct);        //��ʼ��GPIO
}

//DQ����Ϊ����
void DHT_DIR_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//����PB5
	GPIO_InitStruct.Pin = DHT_DQ_PIN;  
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;           //����
    GPIO_InitStruct.Pull = GPIO_PULLUP;               //����
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;       //����
    HAL_GPIO_Init(DHT_DQ_PORT, &GPIO_InitStruct);
}

/*******************************************************************************
* Function Name  : DHT11_Reset
* Description    : ��λDHT11����
* Input          : None
* Output         : None
* Return         : ����0��ʾ��λ�ɹ�,����0xFF��ʾʧ��
* Note			 : None
*******************************************************************************/
u8 DHT11_Reset(void)
{
	u16 timeout=0;
	
	DHT_DIR_OUT();    //����������Ϊ���
    DHT_BIT_LOW();	  //����
	delay_ms(20);     //�����ʱ18ms
	DHT_BIT_HIGH();	  //����,�ͷ�����
	delay_us(30);     //��ʱ20us-40us
	
	DHT_DIR_IN();     //����������Ϊ����,�ȴ��ӻ���Ӧ(�͵�ƽ)	
	//��ʱDHT11�Ѿ�������Ӧ,�����߱�Ϊ�͵�ƽ
	while(DHT_BIT_IN())   //�ȴ���Ϊ�͵�ƽ
	{
		delay_us(1);
		if(++timeout > 200) goto Exit;	
	}
	timeout = 0;
	//��Ϊ�ߵ�ƽ,����80ms
	while(!DHT_BIT_IN())    //�ȴ���Ϊ�ߵ�ƽ
	{
		delay_us(1);
		if(++timeout > 200) goto Exit;
	}
	
	return 0;
	
Exit:
	return 0xFF;
}

/*******************************************************************************
* Function Name  : DHT11_RD_Bit
* Description    : ��һ��λ,һ����ʱ���������60us
* Input          : None
* Output         : None
* Return         : ���ض�����λ(0��1),����0xFF��ʾʧ��
* Note			 : DHT11 ��27~28us�ĸߵ�ƽ��ʾ��0������70us�ߵ�ƽ��ʾ��1��
*******************************************************************************/
u8 DHT11_RD_Bit(void)
{	
	u8 timeout=0;
	
	DHT_DIR_IN();           //����������Ϊ����
	
	while(DHT_BIT_IN())   //�ȴ���Ϊ�͵�ƽ,ÿλ��50us�͵�ƽ��ʼ
	{
		delay_us(1);
		if(++timeout > 200) goto Exit;	
	}
	timeout = 0;
	//��Ϊ�ߵ�ƽ,����80ms
	while(!DHT_BIT_IN())    //�ȴ���Ϊ�ߵ�ƽ
	{
		delay_us(1);
		if(++timeout > 200) goto Exit;
	}
	
	delay_us(50);           //��ʱ50us
	
    if(DHT_BIT_IN())
		return 1;
	else
		return 0;
	
Exit:
	return 0xFF;
}

/*******************************************************************************
* Function Name  : DHT11_RD_Byte
* Description    : ��һ���ֽ�,��λ��ǰ
* Input          : None
* Output         : None
* Return         : ���ض�������
* Note			 : None
*******************************************************************************/
u8 DHT11_RD_Byte(void)
{
	u8 i,dat=0;
	
	for(i = 0; i < 8; i++)
	{
		dat |= (DHT11_RD_Bit()<<(7-i));
	}
	
	return dat;
}

/*******************************************************************************
* Function Name  : DHT11_Get_TemperAndHumi
* Description    : ��ȡ��ʪ��
* Input          : ��ȡ������ʪ��ֵ
* Output         : None
* Return         : 0 У�����   1  ��ȡ�ɹ�  0xFF  ��λʧ��
* Note			 : None
*******************************************************************************/
u8 DHT11_Get_TemperAndHumi(DHT11_Data_TypeDef *DHT11_Data)
{	
	if(DHT11_Reset() == 0)
	{
		//��ʼ��������
		DHT11_Data->humi_int = DHT11_RD_Byte();
		DHT11_Data->humi_deci = DHT11_RD_Byte();
		DHT11_Data->temp_int = DHT11_RD_Byte();
		DHT11_Data->temp_deci = DHT11_RD_Byte();
		DHT11_Data->check_sum = DHT11_RD_Byte();
		
		if(DHT11_Data->check_sum == DHT11_Data->humi_int+DHT11_Data->humi_deci+DHT11_Data->temp_int+DHT11_Data->temp_deci)
			return 1;
		else 
			return 0;
	}
	else
		return 0xFF;
}
   
/*******************************************************************************
* Function Name  : DHT11_Init
* Description    : ��ʼ��DHT11,���DHT11�Ƿ����
* Input          : None
* Output         : None
* Return         : ����0��ʾ��λ�ɹ�,����0xFF��ʾʧ��
* Note			 : None
*******************************************************************************/
u8 DHT11_Init(void)
{	
	GPIO_InitTypeDef GPIO_Initure;
	DHT_DQ_PORT_CLK_ENABLE(); //ʹ�ܶ˿�ʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_5;           //PG11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��
	
	return DHT11_Reset();
}















