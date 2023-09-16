/***************************** (C) COPYRIGHT ************************************
* File Name          : Usart.h
* Author             : �ǹ�Ƕ��ʽ
* Version            : V1.0
* Date               : 10/12/2017
* Description        : ��������
* Note               : ��Ȩ����  �Ͻ�����
********************************************************************************
* ʵ��ƽ̨:�ǹ�����STM32F103ZET6������ 
* �Ա�����: https://shop148702745.taobao.com/
* ������̳: www.feifanembed.com
* QQȺ:542830257
********************************************************************************/
#ifndef __USART_H_
#define __USART_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

#define UART_LSR_RDR  (1<<5)    //������ɱ�־
#define UART_LSR_THRE (1<<6)    //������ɱ�־

#define SEND_BUF1 (USART1->DR)
#define RECV_BUF1 (USART1->DR)
#define USART1_SR (USART1->SR)

#define SEND_BUF2 (USART2->DR)
#define RECV_BUF2 (USART2->DR)
#define USART2_SR (USART2->SR)

#define SEND_BUF3 (USART3->DR)
#define RECV_BUF3 (USART3->DR)
#define USART3_SR (USART3->SR)

typedef struct 
{
	void (*U1TXOver)(void);                       //UART1 �������
	void (*U1RXOperation)(unsigned char RxData);  //UART1 ���ջص�
	void (*U2TXOver)(void);                   	  //UART2 �������
	void (*U2RXOperation)(unsigned char RxData);  //UART2 ���ջص�
	void (*U3TXOver)(void);                   	  //UART3 �������
	void (*U3RXOperation)(unsigned char RxData);  //UART3 ���ջص�
}_USART_ISR_;
extern _USART_ISR_	Usart_ISR;

/*******************************************************************************
* Function Name  : USART1_Init
* Description    : ���ڳ�ʼ��
* Input          : BPS   ������
				   TXFunction   ���ͻص�
				   RxFunction   ���ջص�
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Init(unsigned int BPS,void (*TXFunction)(void),void (*RxFunction)(unsigned char RX_Data));

/*******************************************************************************
* Function Name  : USART1_Send
* Description    : ���ڷ���
* Input          : buff  ����������    len      �������ݳ���
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Send(uint8_t *buff, uint16_t len);

/*******************************************************************************
* Function Name  : USART1_SendString
* Description    : ����1����һ���ַ���
* Input          : str  �������ַ��� 
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_SendString(char *str);

/*******************************************************************************
* Function Name  : USART1_Send_Poll
* Description    : ����1����һ������
* Input          : buff  ����������   Size  ���ʹ�С
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Send_Poll(uint8_t *buff, uint16_t Size);

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
void USART2_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data));

/*******************************************************************************
* Function Name  : USART2_Send
* Description    : ����2����
* Input          : buff  ����������    len      �������ݳ���
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_Send(uint8_t *buff, uint16_t len);

/*******************************************************************************
* Function Name  : USART2_SendString
* Description    : ����2����һ���ַ���
* Input          : str  �������ַ��� 
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_SendString(char *str);

/*******************************************************************************
* Function Name  : USART2_Send_Poll
* Description    : ����2����һ������
* Input          : buff  ����������   Size  ���ʹ�С
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_Send_Poll(uint8_t *buff, uint16_t Size);

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
void USART3_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data));

/*******************************************************************************
* Function Name  : USART3_Send
* Description    : ����3����
* Input          : buff  ����������    len      �������ݳ���
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_Send(uint8_t *buff, uint16_t len);

/*******************************************************************************
* Function Name  : USART3_SendString
* Description    : ����3����һ���ַ���
* Input          : str  �������ַ��� 
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_SendString(char *str);

/*******************************************************************************
* Function Name  : USART3_Send_Poll
* Description    : ����3����һ���ַ���
* Input          : buff  ����������   Size  ���ʹ�С
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_Send_Poll(uint8_t *buff, uint16_t Size);

#endif

