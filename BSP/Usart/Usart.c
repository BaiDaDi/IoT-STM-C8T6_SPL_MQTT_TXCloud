#include "Usart.h"
#include "stdio.h"

_USART_ISR_	Usart_ISR;
UART_HandleTypeDef Uart1_Handle;
UART_HandleTypeDef Uart2_Handle;
UART_HandleTypeDef Uart3_Handle;

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : ����1�жϷ�����
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_IRQHandler(void)
{	
	if(USART1_SR&UART_LSR_RDR)			//���յ�����
	{	  
 		if(Usart_ISR.U1RXOperation != NULL) Usart_ISR.U1RXOperation(RECV_BUF1); 		
	} 
	
	HAL_UART_IRQHandler(&Uart1_Handle);
}

/*******************************************************************************
* Function Name  : USART1_Init
* Description    : ����1��ʼ��
* Input          : BPS   ������
				   Tx_Over      ������ɻص�
				   RxFunction   ���ջص�
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data))
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStruct;
	
	Usart_ISR.U1TXOver = Tx_Over;          //������ɻص�
	Usart_ISR.U1RXOperation = RxFunction;  //�������ݺ���
	
	__GPIOA_CLK_ENABLE();      //ʹ��GPIOAʱ��
	__USART1_CLK_ENABLE();     //ʹ��USART1ʱ��
	
	//USART1�˿�����	
	GPIO_InitStruct.Pin = GPIO_PIN_9; //UASRT1��ӦIO��PA9 TX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;//GPIO_MODE_AF_OD;        //�����������ģʽ
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  //����
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //����
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        //��ʼ��PA9,PA10
	
	GPIO_InitStruct.Pin = GPIO_PIN_10; //UASRT1��ӦIO��PA10  RX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;   //����Ϊ����
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        //��ʼ��PA10  RX

   //USART1 ��ʼ������
	Uart1_Handle.Instance          = USART1;                //����1
	Uart1_Handle.Init.BaudRate     = BPS;               //����������
	Uart1_Handle.Init.WordLength   = UART_WORDLENGTH_8B;    //8λ
	Uart1_Handle.Init.StopBits     = UART_STOPBITS_1;       //1��ֹͣλ
	Uart1_Handle.Init.Parity       = UART_PARITY_NONE;      //��У��
	Uart1_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;   //��Ӳ����
	Uart1_Handle.Init.Mode         = UART_MODE_TX_RX;       //���ڷ���,����ʹ��
	Uart1_Handle.Init.OverSampling = UART_OVERSAMPLING_16;  //16��������  
	
	HAL_UART_Init(&Uart1_Handle);   //��ʼ������

    __HAL_UART_ENABLE_IT(&Uart1_Handle, UART_IT_RXNE);   //ʹ�ܽ����ж�
	
	//USART1 NVIC ����
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART1_IRQn);
}

/*******************************************************************************
* Function Name  : USART1_Send
* Description    : ����1����
* Input          : buff  ����������    len      �������ݳ���
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
* Description    : ����1����һ���ֽ�,��ѯ��ʽ
* Input          : ch  ����������   
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_SendByte(char ch)
{	
	while((USART1_SR&UART_LSR_THRE)==0);	//�ȴ��������  
	SEND_BUF1 = (u8) ch;
}

/*******************************************************************************
* Function Name  : USART1_SendString
* Description    : ����1����һ���ַ���
* Input          : str  �������ַ��� 
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
* Description    : ����1����һ������
* Input          : buff  ����������   Size  ���ʹ�С
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

//�ض���fputc����,�ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{    	
	while((USART1_SR&UART_LSR_THRE)==0);	//�ȴ��������  
	SEND_BUF1 = (u8) ch;
	
	return ch;
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : ����2�жϷ�����
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
	if(USART2_SR&UART_LSR_RDR)			//���յ�����
	{	  
 		if(Usart_ISR.U2RXOperation != NULL) Usart_ISR.U2RXOperation(RECV_BUF2); 		
	} 
	
	HAL_UART_IRQHandler(&Uart2_Handle);
}

/*******************************************************************************
* Function Name  : USART2_Init
* Description    : ����2��ʼ��
* Input          : BPS   ������
				   Tx_Over      ������ɻص�
				   RxFunction   ���ջص�
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data))
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStruct;
	
	Usart_ISR.U2TXOver = Tx_Over;   	   //������ɻص�
	Usart_ISR.U2RXOperation = RxFunction;  //�������ݺ���
	
	__GPIOA_CLK_ENABLE();      //ʹ��GPIOAʱ��
	__USART2_CLK_ENABLE();     //ʹ��USART2ʱ��
	
	//USART2�˿�����	
	GPIO_InitStruct.Pin = GPIO_PIN_2; 			   //UASRT2��ӦIO��PA2 TX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        //��������ģʽ
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  //����
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //����
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        //��ʼ��PA2
	
	GPIO_InitStruct.Pin = GPIO_PIN_3; 			   //UASRT2��ӦIO��PA3  RX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;     //����Ϊ����
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);        //��ʼ��PA3

   //USART2 ��ʼ������
	Uart2_Handle.Instance          = USART2;                //����2
	Uart2_Handle.Init.BaudRate     = BPS;               //����������
	Uart2_Handle.Init.WordLength   = UART_WORDLENGTH_8B;    //8λ
	Uart2_Handle.Init.StopBits     = UART_STOPBITS_1;       //1��ֹͣλ
	Uart2_Handle.Init.Parity       = UART_PARITY_NONE;      //��У��
	Uart2_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;   //��Ӳ����
	Uart2_Handle.Init.Mode         = UART_MODE_TX_RX;       //���ڷ���,����ʹ��
	Uart2_Handle.Init.OverSampling = UART_OVERSAMPLING_16;  //16��������  
	
	HAL_UART_Init(&Uart2_Handle);   //��ʼ������

	__HAL_UART_ENABLE_IT(&Uart2_Handle, UART_IT_RXNE);   //ʹ�ܽ����ж�   

	//USART2 NVIC ����	
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

/*******************************************************************************
* Function Name  : USART2_Send
* Description    : ����2����
* Input          : buff  ����������    len      �������ݳ���
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
* Description    : ����2����һ���ֽ�,��ѯ��ʽ
* Input          : ch  ����������   
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_SendByte(char ch)
{	
	while((USART2_SR&UART_LSR_THRE)==0);	//�ȴ��������  
	SEND_BUF2 = (u8) ch;
}

/*******************************************************************************
* Function Name  : USART2_SendString
* Description    : ����2����һ���ַ���
* Input          : str  �������ַ��� 
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
* Description    : ����2����һ������
* Input          : buff  ����������   Size  ���ʹ�С
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
* Description    : ����3�жϷ�����
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART3_SR&UART_LSR_RDR)			//���յ�����
	{	  
 		if(Usart_ISR.U3RXOperation != NULL) Usart_ISR.U3RXOperation(RECV_BUF3); 		
	} 
	
	HAL_UART_IRQHandler(&Uart3_Handle);
}

/*******************************************************************************
* Function Name  : USART3_Init
* Description    : ����3��ʼ��
* Input          : BPS   ������
				   Tx_Over      ������ɻص�
				   RxFunction   ���ջص�
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data))
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStruct;

	Usart_ISR.U3TXOver = Tx_Over;   //������ɻص�
	Usart_ISR.U3RXOperation = RxFunction;  //�������ݺ���
	
	__GPIOB_CLK_ENABLE();      //ʹ��GPIOBʱ��
	__USART3_CLK_ENABLE();     //ʹ��USART3ʱ��
	
	//USART3�˿�����	
	GPIO_InitStruct.Pin = GPIO_PIN_10; 			   //UASRT3��ӦIO��PB10 TX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;        //��������ģʽ
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  //����
	GPIO_InitStruct.Pull = GPIO_PULLUP;            //����
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);        //��ʼ��PB10
	
	GPIO_InitStruct.Pin = GPIO_PIN_11; 			   //UASRT3��ӦIO��PB11  RX
	GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;     //����Ϊ����
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);        //��ʼ��PB11

   //USART3 ��ʼ������
	Uart3_Handle.Instance          = USART3;                //����3
	Uart3_Handle.Init.BaudRate     = BPS;               //����������
	Uart3_Handle.Init.WordLength   = UART_WORDLENGTH_8B;    //8λ
	Uart3_Handle.Init.StopBits     = UART_STOPBITS_1;       //1��ֹͣλ
	Uart3_Handle.Init.Parity       = UART_PARITY_NONE;      //��У��
	Uart3_Handle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;   //��Ӳ����
	Uart3_Handle.Init.Mode         = UART_MODE_TX_RX;       //���ڷ���,����ʹ��
	Uart3_Handle.Init.OverSampling = UART_OVERSAMPLING_16;  //16��������  
	
	HAL_UART_Init(&Uart3_Handle);   //��ʼ������

    __HAL_UART_ENABLE_IT(&Uart3_Handle, UART_IT_RXNE);   //ʹ�ܽ����ж�

	//USART3 NVIC ����	
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
}

/*******************************************************************************
* Function Name  : USART3_Send
* Description    : ����3����
* Input          : buff  ����������    len      �������ݳ���
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
* Description    : ����3����һ���ֽ�,��ѯ��ʽ
* Input          : ch  ����������   
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_SendByte(char ch)
{	
	while((USART3_SR&UART_LSR_THRE)==0);	//�ȴ��������  
	SEND_BUF3 = (u8) ch;
}

/*******************************************************************************
* Function Name  : USART3_SendString
* Description    : ����3����һ���ַ���
* Input          : str  �������ַ��� 
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
* Description    : ����3����һ���ַ���
* Input          : buff  ����������   Size  ���ʹ�С
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
* Description    : ������ɻص�,��HAL_UART_IRQHandler�е���
* Input          : huart  ���ھ��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance == USART1)    //����1
	{
		if(Usart_ISR.U1TXOver != NULL) Usart_ISR.U1TXOver();  
	}
	else if(huart->Instance == USART2)    //����2
	{
		if(Usart_ISR.U2TXOver != NULL) Usart_ISR.U2TXOver();
	}
	else if(huart->Instance == USART3)    //����3
	{
		if(Usart_ISR.U3TXOver != NULL) Usart_ISR.U3TXOver();
	}
}
