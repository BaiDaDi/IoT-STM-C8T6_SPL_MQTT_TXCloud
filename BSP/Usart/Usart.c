#include "Usart.h"
#include "stdio.h"

_USART_ISR_	Usart_ISR;
UART_HandleTypeDef Uart1_Handle;
UART_HandleTypeDef Uart2_Handle;
UART_HandleTypeDef Uart3_Handle;

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : 串口1中断服务函数
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_IRQHandler(void)
{	
	if(USART1_SR&UART_LSR_RDR)			//接收到数据
	{	  
 		if(Usart_ISR.U1RXOperation != NULL) Usart_ISR.U1RXOperation(RECV_BUF1); 		
	} 
	
	HAL_UART_IRQHandler(&Uart1_Handle);
}

/*******************************************************************************
* Function Name  : USART1_Init
* Description    : 串口1初始化
* Input          : BPS   波特率
				   Tx_Over      发送完成回调
				   RxFunction   接收回调
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data))
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStruct;
	
	Usart_ISR.U1TXOver = Tx_Over;          //发送完成回调
	Usart_ISR.U1RXOperation = RxFunction;  //接收数据函数
	
	__GPIOA_CLK_ENABLE();      //使能GPIOA时钟
	__USART1_CLK_ENABLE();     //使能USART1时钟
	
	//USART1端口配置	
	GPIO_InitStruct.Pin = GPIO_PIN_9; //UASRT1对应IO口PA9 TX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//GPIO_MODE_AF_OD;        //复用推挽输出模式
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  //高速
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //上拉
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        //初始化PA9,PA10
	
	GPIO_InitStruct.Pin = GPIO_PIN_10; //UASRT1对应IO口PA10  RX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;   //配置为输入
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        //初始化PA10  RX

   //USART1 初始化设置
	Uart1_Handle.Instance          = USART1;                //串口1
	Uart1_Handle.Init.BaudRate     = BPS;               //波特率设置
	Uart1_Handle.Init.WordLength   = UART_WORDLENGTH_8B;    //8位
	Uart1_Handle.Init.StopBits     = UART_STOPBITS_1;       //1个停止位
	Uart1_Handle.Init.Parity       = UART_PARITY_NONE;      //无校验
	Uart1_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;   //无硬件流
	Uart1_Handle.Init.Mode         = UART_MODE_TX_RX;       //串口发送,接收使能
	Uart1_Handle.Init.OverSampling = UART_OVERSAMPLING_16;  //16倍过采样  
	
	HAL_UART_Init(&Uart1_Handle);   //初始化串口

    __HAL_UART_ENABLE_IT(&Uart1_Handle, UART_IT_RXNE);   //使能接收中断
	
	//USART1 NVIC 配置
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/*******************************************************************************
* Function Name  : USART1_Send
* Description    : 串口1发送
* Input          : buff  待发送数据    len      发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Send(uint8_t *buff, uint16_t len)
{
	HAL_UART_Transmit_IT(&Uart1_Handle, (uint8_t*)buff, len);
}

/*******************************************************************************
* Function Name  : USART1_SendByte
* Description    : 串口1发送一个字节,查询方式
* Input          : ch  待发送数据   
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_SendByte(char ch)
{	
	while((USART1_SR&UART_LSR_THRE)==0);	//等待发送完成  
	SEND_BUF1 = (u8) ch;
}

/*******************************************************************************
* Function Name  : USART1_SendString
* Description    : 串口1发送一串字符串
* Input          : str  待发送字符串 
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_SendString(char *str) 
{
  	while(*str != 0) 
	{
   		USART1_SendByte(*str++);
	}
}

/*******************************************************************************
* Function Name  : USART1_Send_Poll
* Description    : 串口1发送一组数据
* Input          : buff  待发送数据   Size  发送大小
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Send_Poll(uint8_t *buff, uint16_t Size) 
{
	uint16_t i;
	
	for(i = 0; i < Size; i++)
	{
		USART1_SendByte(*buff++);
	}
}

//重定义fputc函数,重定向后可使用printf函数
int fputc(int ch, FILE *f)
{    	
	while((USART1_SR&UART_LSR_THRE)==0);	//等待发送完成  
	SEND_BUF1 = (u8) ch;
	
	return ch;
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : 串口2中断服务函数
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
	if(USART2_SR&UART_LSR_RDR)			//接收到数据
	{	  
 		if(Usart_ISR.U2RXOperation != NULL) Usart_ISR.U2RXOperation(RECV_BUF2); 		
	} 
	
	HAL_UART_IRQHandler(&Uart2_Handle);
}

/*******************************************************************************
* Function Name  : USART2_Init
* Description    : 串口2初始化
* Input          : BPS   波特率
				   Tx_Over      发送完成回调
				   RxFunction   接收回调
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data))
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStruct;
	
	Usart_ISR.U2TXOver = Tx_Over;   	   //发送完成回调
	Usart_ISR.U2RXOperation = RxFunction;  //接收数据函数
	
	__GPIOA_CLK_ENABLE();      //使能GPIOA时钟
	__USART2_CLK_ENABLE();     //使能USART2时钟
	
	//USART2端口配置	
	GPIO_InitStruct.Pin = GPIO_PIN_2; 			   //UASRT2对应IO口PA2 TX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        //复用推挽模式
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  //高速
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //上拉
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        //初始化PA2
	
	GPIO_InitStruct.Pin = GPIO_PIN_3; 			   //UASRT2对应IO口PA3  RX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;     //配置为输入
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        //初始化PA3

   //USART2 初始化设置
	Uart2_Handle.Instance          = USART2;                //串口2
	Uart2_Handle.Init.BaudRate     = BPS;               //波特率设置
	Uart2_Handle.Init.WordLength   = UART_WORDLENGTH_8B;    //8位
	Uart2_Handle.Init.StopBits     = UART_STOPBITS_1;       //1个停止位
	Uart2_Handle.Init.Parity       = UART_PARITY_NONE;      //无校验
	Uart2_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;   //无硬件流
	Uart2_Handle.Init.Mode         = UART_MODE_TX_RX;       //串口发送,接收使能
	Uart2_Handle.Init.OverSampling = UART_OVERSAMPLING_16;  //16倍过采样  
	
	HAL_UART_Init(&Uart2_Handle);   //初始化串口

	__HAL_UART_ENABLE_IT(&Uart2_Handle, UART_IT_RXNE);   //使能接收中断   

	//USART2 NVIC 配置	
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/*******************************************************************************
* Function Name  : USART2_Send
* Description    : 串口2发送
* Input          : buff  待发送数据    len      发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_Send(uint8_t *buff, uint16_t len)
{
	HAL_UART_Transmit_IT(&Uart2_Handle, (uint8_t*)buff, len);
}

/*******************************************************************************
* Function Name  : USART2_SendByte
* Description    : 串口2发送一个字节,查询方式
* Input          : ch  待发送数据   
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_SendByte(char ch)
{	
	while((USART2_SR&UART_LSR_THRE)==0);	//等待发送完成  
	SEND_BUF2 = (u8) ch;
}

/*******************************************************************************
* Function Name  : USART2_SendString
* Description    : 串口2发送一串字符串
* Input          : str  待发送字符串 
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_SendString(char *str) 
{
  	while(*str != 0) 
	{
   		USART2_SendByte(*str++);
	}
}

/*******************************************************************************
* Function Name  : USART2_Send_Poll
* Description    : 串口2发送一组数据
* Input          : buff  待发送数据   Size  发送大小
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_Send_Poll(uint8_t *buff, uint16_t Size) 
{
	uint16_t i;
	
	for(i = 0; i < Size; i++)
	{
		USART2_SendByte(*buff++);
	}
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : 串口3中断服务函数
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART3_SR&UART_LSR_RDR)			//接收到数据
	{	  
 		if(Usart_ISR.U3RXOperation != NULL) Usart_ISR.U3RXOperation(RECV_BUF3); 		
	} 
	
	HAL_UART_IRQHandler(&Uart3_Handle);
}

/*******************************************************************************
* Function Name  : USART3_Init
* Description    : 串口3初始化
* Input          : BPS   波特率
				   Tx_Over      发送完成回调
				   RxFunction   接收回调
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data))
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStruct;

	Usart_ISR.U3TXOver = Tx_Over;   //发送完成回调
	Usart_ISR.U3RXOperation = RxFunction;  //接收数据函数
	
	__GPIOB_CLK_ENABLE();      //使能GPIOB时钟
	__USART3_CLK_ENABLE();     //使能USART3时钟
	
	//USART3端口配置	
	GPIO_InitStruct.Pin = GPIO_PIN_10; 			   //UASRT3对应IO口PB10 TX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        //复用推挽模式
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  //高速
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //上拉
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);        //初始化PB10
	
	GPIO_InitStruct.Pin = GPIO_PIN_11; 			   //UASRT3对应IO口PB11  RX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;     //配置为输入
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);        //初始化PB11

   //USART3 初始化设置
	Uart3_Handle.Instance          = USART3;                //串口3
	Uart3_Handle.Init.BaudRate     = BPS;               //波特率设置
	Uart3_Handle.Init.WordLength   = UART_WORDLENGTH_8B;    //8位
	Uart3_Handle.Init.StopBits     = UART_STOPBITS_1;       //1个停止位
	Uart3_Handle.Init.Parity       = UART_PARITY_NONE;      //无校验
	Uart3_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;   //无硬件流
	Uart3_Handle.Init.Mode         = UART_MODE_TX_RX;       //串口发送,接收使能
	Uart3_Handle.Init.OverSampling = UART_OVERSAMPLING_16;  //16倍过采样  
	
	HAL_UART_Init(&Uart3_Handle);   //初始化串口

    __HAL_UART_ENABLE_IT(&Uart3_Handle, UART_IT_RXNE);   //使能接收中断

	//USART3 NVIC 配置	
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/*******************************************************************************
* Function Name  : USART3_Send
* Description    : 串口3发送
* Input          : buff  待发送数据    len      发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_Send(uint8_t *buff, uint16_t len)
{
	HAL_UART_Transmit_IT(&Uart3_Handle, (uint8_t*)buff, len);
}

/*******************************************************************************
* Function Name  : USART3_SendByte
* Description    : 串口3发送一个字节,查询方式
* Input          : ch  待发送数据   
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_SendByte(char ch)
{	
	while((USART3_SR&UART_LSR_THRE)==0);	//等待发送完成  
	SEND_BUF3 = (u8) ch;
}

/*******************************************************************************
* Function Name  : USART3_SendString
* Description    : 串口3发送一串字符串
* Input          : str  待发送字符串 
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_SendString(char *str) 
{
  	while(*str != 0) 
	{
   		USART3_SendByte(*str++);
	}
}

/*******************************************************************************
* Function Name  : USART3_Send_Poll
* Description    : 串口3发送一串字符串
* Input          : buff  待发送数据   Size  发送大小
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_Send_Poll(uint8_t *buff, uint16_t Size) 
{
	uint16_t i;
	
	for(i = 0; i < Size; i++)
	{
		USART3_SendByte(*buff++);
	}
}

/*******************************************************************************
* Function Name  : HAL_UART_TxCpltCallback
* Description    : 发送完成回调,在HAL_UART_IRQHandler中调用
* Input          : huart  串口句柄
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)    //串口1
	{
		if(Usart_ISR.U1TXOver != NULL) Usart_ISR.U1TXOver();  
	}
	else if(huart->Instance == USART2)    //串口2
	{
		if(Usart_ISR.U2TXOver != NULL) Usart_ISR.U2TXOver();
	}
	else if(huart->Instance == USART3)    //串口3
	{
		if(Usart_ISR.U3TXOver != NULL) Usart_ISR.U3TXOver();
	}
}
