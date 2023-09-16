#include "DHT110.h"
#include "delay.h"

//DQ配置为输出
void DHT_DIR_OUT(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	//GPIOB5初始化设置
	GPIO_InitStruct.Pin = DHT_DQ_PIN;             
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;    //普通推挽输出模式
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;       //高速
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //上拉
	HAL_GPIO_Init(DHT_DQ_PORT, &GPIO_InitStruct);        //初始化GPIO
}

//DQ配置为输入
void DHT_DIR_IN(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//配置PB5
	GPIO_InitStruct.Pin = DHT_DQ_PIN;  
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;           //输入
    GPIO_InitStruct.Pull = GPIO_PULLUP;               //上拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;       //高速
    HAL_GPIO_Init(DHT_DQ_PORT, &GPIO_InitStruct);
}

/*******************************************************************************
* Function Name  : DHT11_Reset
* Description    : 复位DHT11总线
* Input          : None
* Output         : None
* Return         : 返回0表示复位成功,返回0xFF表示失败
* Note			 : None
*******************************************************************************/
u8 DHT11_Reset(void)
{
	u16 timeout=0;
	
	DHT_DIR_OUT();    //数据线配置为输出
    DHT_BIT_LOW();	  //拉低
	delay_ms(20);     //最低延时18ms
	DHT_BIT_HIGH();	  //拉高,释放总线
	delay_us(30);     //延时20us-40us
	
	DHT_DIR_IN();     //数据线配置为输入,等待从机相应(低电平)	
	//这时DHT11已经做出响应,数据线变为低电平
	while(DHT_BIT_IN())   //等待变为低电平
	{
		delay_us(1);
		if(++timeout > 200) goto Exit;	
	}
	timeout = 0;
	//变为高电平,持续80ms
	while(!DHT_BIT_IN())    //等待变为高电平
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
* Description    : 读一个位,一个读时序过程至少60us
* Input          : None
* Output         : None
* Return         : 返回读到的位(0或1),返回0xFF表示失败
* Note			 : DHT11 以27~28us的高电平表示“0”，以70us高电平表示“1”
*******************************************************************************/
u8 DHT11_RD_Bit(void)
{	
	u8 timeout=0;
	
	DHT_DIR_IN();           //数据线配置为输入
	
	while(DHT_BIT_IN())   //等待变为低电平,每位以50us低电平开始
	{
		delay_us(1);
		if(++timeout > 200) goto Exit;	
	}
	timeout = 0;
	//变为高电平,持续80ms
	while(!DHT_BIT_IN())    //等待变为高电平
	{
		delay_us(1);
		if(++timeout > 200) goto Exit;
	}
	
	delay_us(50);           //延时50us
	
    if(DHT_BIT_IN())
		return 1;
	else
		return 0;
	
Exit:
	return 0xFF;
}

/*******************************************************************************
* Function Name  : DHT11_RD_Byte
* Description    : 读一个字节,高位在前
* Input          : None
* Output         : None
* Return         : 返回读到的数
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
* Description    : 获取温湿度
* Input          : 读取到的温湿度值
* Output         : None
* Return         : 0 校验错误   1  读取成功  0xFF  复位失败
* Note			 : None
*******************************************************************************/
u8 DHT11_Get_TemperAndHumi(DHT11_Data_TypeDef *DHT11_Data)
{	
	if(DHT11_Reset() == 0)
	{
		//开始接收数据
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
* Description    : 初始化DHT11,检测DHT11是否存在
* Input          : None
* Output         : None
* Return         : 返回0表示复位成功,返回0xFF表示失败
* Note			 : None
*******************************************************************************/
u8 DHT11_Init(void)
{	
	GPIO_InitTypeDef GPIO_Initure;
	DHT_DQ_PORT_CLK_ENABLE(); //使能端口时钟
	
    GPIO_Initure.Pin=GPIO_PIN_5;           //PG11
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化
	
	return DHT11_Reset();
}















