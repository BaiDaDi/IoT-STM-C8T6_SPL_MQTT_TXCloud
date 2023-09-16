/***************************** (C) COPYRIGHT ************************************
* File Name          : Usart.h
* Author             : 星光嵌入式
* Version            : V1.0
* Date               : 10/12/2017
* Description        : 串口驱动
* Note               : 版权所有  严禁盗版
********************************************************************************
* 实验平台:星光猛龙STM32F103ZET6开发板 
* 淘宝店铺: https://shop148702745.taobao.com/
* 技术论坛: www.feifanembed.com
* QQ群:542830257
********************************************************************************/
#ifndef __USART_H_
#define __USART_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

#define UART_LSR_RDR  (1<<5)    //接收完成标志
#define UART_LSR_THRE (1<<6)    //发送完成标志

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
	void (*U1TXOver)(void);                       //UART1 发送完成
	void (*U1RXOperation)(unsigned char RxData);  //UART1 接收回调
	void (*U2TXOver)(void);                   	  //UART2 发送完成
	void (*U2RXOperation)(unsigned char RxData);  //UART2 接收回调
	void (*U3TXOver)(void);                   	  //UART3 发送完成
	void (*U3RXOperation)(unsigned char RxData);  //UART3 接收回调
}_USART_ISR_;
extern _USART_ISR_	Usart_ISR;

/*******************************************************************************
* Function Name  : USART1_Init
* Description    : 串口初始化
* Input          : BPS   波特率
				   TXFunction   发送回调
				   RxFunction   接收回调
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Init(unsigned int BPS,void (*TXFunction)(void),void (*RxFunction)(unsigned char RX_Data));

/*******************************************************************************
* Function Name  : USART1_Send
* Description    : 串口发送
* Input          : buff  待发送数据    len      发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Send(uint8_t *buff, uint16_t len);

/*******************************************************************************
* Function Name  : USART1_SendString
* Description    : 串口1发送一串字符串
* Input          : str  待发送字符串 
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_SendString(char *str);

/*******************************************************************************
* Function Name  : USART1_Send_Poll
* Description    : 串口1发送一组数据
* Input          : buff  待发送数据   Size  发送大小
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART1_Send_Poll(uint8_t *buff, uint16_t Size);

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
void USART2_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data));

/*******************************************************************************
* Function Name  : USART2_Send
* Description    : 串口2发送
* Input          : buff  待发送数据    len      发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_Send(uint8_t *buff, uint16_t len);

/*******************************************************************************
* Function Name  : USART2_SendString
* Description    : 串口2发送一串字符串
* Input          : str  待发送字符串 
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_SendString(char *str);

/*******************************************************************************
* Function Name  : USART2_Send_Poll
* Description    : 串口2发送一组数据
* Input          : buff  待发送数据   Size  发送大小
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART2_Send_Poll(uint8_t *buff, uint16_t Size);

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
void USART3_Init(unsigned int BPS, void Tx_Over(void), void (*RxFunction)(unsigned char RX_Data));

/*******************************************************************************
* Function Name  : USART3_Send
* Description    : 串口3发送
* Input          : buff  待发送数据    len      发送数据长度
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_Send(uint8_t *buff, uint16_t len);

/*******************************************************************************
* Function Name  : USART3_SendString
* Description    : 串口3发送一串字符串
* Input          : str  待发送字符串 
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_SendString(char *str);

/*******************************************************************************
* Function Name  : USART3_Send_Poll
* Description    : 串口3发送一串字符串
* Input          : buff  待发送数据   Size  发送大小
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void USART3_Send_Poll(uint8_t *buff, uint16_t Size);

#endif

