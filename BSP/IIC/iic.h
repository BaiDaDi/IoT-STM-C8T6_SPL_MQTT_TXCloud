#ifndef __IIC_H_
#define __IIC_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

#ifndef BSP_I2C_SPEED
 #define BSP_I2C_SPEED                          400000
#endif /* BSP_I2C_SPEED */

#define I2C1_SCL_PIN                     GPIO_PIN_6
#define I2C1_SCL_SDA_GPIO_PORT           GPIOB
#define I2C1_SDA_PIN                     GPIO_PIN_7

#define I2C2_SCL_PIN                     GPIO_PIN_10
#define I2C2_SCL_SDA_GPIO_PORT           GPIOB
#define I2C2_SDA_PIN                     GPIO_PIN_11

/*******************************************************************************
* Function Name  : I2C1_Init
* Description    : PB6 PB7��ʼ����I2C1
* Input          : i2c_handler : I2C handler
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void I2C1_Init(I2C_HandleTypeDef *i2c_handler);

/*******************************************************************************
* Function Name  : I2C1_ReadMultiple
* Description    : I2C������
* Input          : i2c_handler : I2C handler
				   Addr: ������ַ
				   Reg:  �����Ĵ�����ַ
				   MemAddSize: һ�η������ݴ�С8λ/16λ
							   I2C_MEMADD_SIZE_8BIT  8λ
							   I2C_MEMADD_SIZE_16BIT 16λ
				   Buffer:  ��д�������
				   Length:  д�볤��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C1_ReadMultiple(I2C_HandleTypeDef *i2c_handler,
                                           uint8_t Addr,
                                           uint16_t Reg,
                                           uint16_t MemAddSize,
                                           uint8_t *Buffer,
                                           uint16_t Length);
										   
/*******************************************************************************
* Function Name  : I2C1_WriteMultiple
* Description    : I2Cд����
* Input          : i2c_handler : I2C handler
				   Addr: ������ַ
				   Reg:  �����Ĵ�����ַ
				   MemAddSize: һ�η������ݴ�С8λ/16λ
							   I2C_MEMADD_SIZE_8BIT  8λ
							   I2C_MEMADD_SIZE_16BIT 16λ
				   Buffer:  ��д�������
				   Length:  д�볤��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C1_WriteMultiple(I2C_HandleTypeDef *i2c_handler,
                                            uint8_t Addr,
                                            uint16_t Reg,
                                            uint16_t MemAddSize,
                                            uint8_t *Buffer,
                                            uint16_t Length);
											
/*******************************************************************************
* Function Name  : I2C1_ReadMaster
* Description    : I2C����������
* Input          : i2c_handler : I2C handler
				   Addr: ������ַ
				   Buffer:  ��д�������
				   Length:  д�볤��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C1_ReadMaster(I2C_HandleTypeDef *i2c_handler,
                                           uint8_t Addr,
                                           uint8_t *Buffer,
                                           uint16_t Length);
										   
/*******************************************************************************
* Function Name  : I2C1_WriteMaster
* Description    : I2C����д����
* Input          : i2c_handler : I2C handler
				   Addr: ������ַ
				   Buffer:  ��д�������
				   Length:  д�볤��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C1_WriteMaster(I2C_HandleTypeDef *i2c_handler,
                                            uint8_t Addr,
                                            uint8_t *Buffer,
                                            uint16_t Length);
											
/*******************************************************************************
* Function Name  : I2C1_IsDeviceReady
* Description    : I2C�ж������Ƿ�׼������
* Input          : i2c_handler : I2C handler
				   DevAddress: ������ַ
				   Reg:  �����Ĵ�����ַ
				   Trials: ���������
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C1_IsDeviceReady(I2C_HandleTypeDef *i2c_handler, uint16_t DevAddress, uint32_t Trials);

/*******************************************************************************
* Function Name  : I2C2_Init
* Description    : PB10 PB11��ʼ����I2C2
* Input          : i2c_handler : I2C handler
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void I2C2_Init(I2C_HandleTypeDef *i2c_handler);

/*******************************************************************************
* Function Name  : I2C2_ReadMultiple
* Description    : I2C������
* Input          : i2c_handler : I2C handler
				   Addr: ������ַ
				   Reg:  �����Ĵ�����ַ
				   MemAddSize: һ�η������ݴ�С8λ/16λ
							   I2C_MEMADD_SIZE_8BIT  8λ
							   I2C_MEMADD_SIZE_16BIT 16λ
				   Buffer:  ��д�������
				   Length:  д�볤��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C2_ReadMultiple(I2C_HandleTypeDef *i2c_handler,
                                           uint8_t Addr,
                                           uint16_t Reg,
                                           uint16_t MemAddSize,
                                           uint8_t *Buffer,
                                           uint16_t Length);
										   
/*******************************************************************************
* Function Name  : I2C2_WriteMultiple
* Description    : I2Cд����
* Input          : i2c_handler : I2C handler
				   Addr: ������ַ
				   Reg:  �����Ĵ�����ַ
				   MemAddSize: һ�η������ݴ�С8λ/16λ
							   I2C_MEMADD_SIZE_8BIT  8λ
							   I2C_MEMADD_SIZE_16BIT 16λ
				   Buffer:  ��д�������
				   Length:  д�볤��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C2_WriteMultiple(I2C_HandleTypeDef *i2c_handler,
                                            uint8_t Addr,
                                            uint16_t Reg,
                                            uint16_t MemAddSize,
                                            uint8_t *Buffer,
                                            uint16_t Length);
											
/*******************************************************************************
* Function Name  : I2C2_ReadMaster
* Description    : I2C����������
* Input          : i2c_handler : I2C handler
				   Addr: ������ַ
				   Buffer:  ��д�������
				   Length:  д�볤��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C2_ReadMaster(I2C_HandleTypeDef *i2c_handler,
                                           uint8_t Addr,
                                           uint8_t *Buffer,
                                           uint16_t Length);
										   
/*******************************************************************************
* Function Name  : I2C2_WriteMaster
* Description    : I2C����д����
* Input          : i2c_handler : I2C handler
				   Addr: ������ַ
				   Buffer:  ��д�������
				   Length:  д�볤��
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C2_WriteMaster(I2C_HandleTypeDef *i2c_handler,
                                            uint8_t Addr,
                                            uint8_t *Buffer,
                                            uint16_t Length);

/*******************************************************************************
* Function Name  : I2C2_IsDeviceReady
* Description    : I2C�ж������Ƿ�׼������
* Input          : i2c_handler : I2C handler
				   DevAddress: ������ַ
				   Reg:  �����Ĵ�����ַ
				   Trials: ���������
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
HAL_StatusTypeDef I2C2_IsDeviceReady(I2C_HandleTypeDef *i2c_handler, uint16_t DevAddress, uint32_t Trials);

#endif
