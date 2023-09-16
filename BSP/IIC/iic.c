#include "iic.h"

/*
PB6 I2C1_SCL
PB7 I2C1_SDA

PB10 I2C2_SCL
PB11 I2C2_SDA

*/

/*******************************************************************************
* Function Name  : I2C1_Init
* Description    : PB6 PB7��ʼ����I2C1
* Input          : i2c_handler : I2C handler
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void I2C1_Init(I2C_HandleTypeDef *i2c_handler)
{
	GPIO_InitTypeDef          GPIO_InitStruct;

    //ʹ��GPIOBʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();

    //����SCL�ܽ�
    GPIO_InitStruct.Pin = I2C1_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C1_SCL_SDA_GPIO_PORT, &GPIO_InitStruct);
    //����SDA�ܽ�
    GPIO_InitStruct.Pin = I2C1_SDA_PIN;
    HAL_GPIO_Init(I2C1_SCL_SDA_GPIO_PORT, &GPIO_InitStruct);

    //ʹ��I2C1ʱ��
    __HAL_RCC_I2C1_CLK_ENABLE();
    /* Force the I2C peripheral clock reset */
    __HAL_RCC_I2C1_FORCE_RESET();
    /* Release the I2C peripheral clock reset */
    __HAL_RCC_I2C1_RELEASE_RESET();
    //����I2C�¼��ж�
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    //����I2C�����ж�
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
	
	if(HAL_I2C_GetState(i2c_handler) == HAL_I2C_STATE_RESET)
	{
		i2c_handler->Instance = I2C1;
		i2c_handler->Init.ClockSpeed       = BSP_I2C_SPEED;
		i2c_handler->Init.DutyCycle        = I2C_DUTYCYCLE_2;
		i2c_handler->Init.OwnAddress1      = 0;
		i2c_handler->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
		i2c_handler->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
		i2c_handler->Init.OwnAddress2      = 0;
		i2c_handler->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
		i2c_handler->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

		/* Init the I2C */
		HAL_I2C_Init(i2c_handler);
	}
}

//������
static void I2C1_Error(I2C_HandleTypeDef *i2c_handler, uint8_t Addr)
{
	/* De-initialize the I2C communication bus */
	HAL_I2C_DeInit(i2c_handler);
}

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
                                           uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(i2c_handler, Addr, (uint16_t)Reg, MemAddSize, Buffer, Length, 1000);

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* I2C error occurred */
		I2C1_Error(i2c_handler, Addr);
	}
	return status;    
}

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
                                            uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(i2c_handler, Addr, (uint16_t)Reg, MemAddSize, Buffer, Length, 1000);

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* Re-Initiaize the I2C Bus */
		I2C1_Error(i2c_handler, Addr);
	}
	return status;
}

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
                                           uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Master_Receive(i2c_handler, Addr, Buffer, Length, 1000);

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* I2C error occurred */
		I2C1_Error(i2c_handler, Addr);
	}
	return status;    
}

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
                                            uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Master_Transmit(i2c_handler, Addr, Buffer, Length, 1000);

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* Re-Initiaize the I2C Bus */
		I2C1_Error(i2c_handler, Addr);
	}
	return status;
}

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
HAL_StatusTypeDef I2C1_IsDeviceReady(I2C_HandleTypeDef *i2c_handler, uint16_t DevAddress, uint32_t Trials)
{ 
	return (HAL_I2C_IsDeviceReady(i2c_handler, DevAddress, Trials, 1000));
}

/*******************************************************************************
* Function Name  : I2C2_Init
* Description    : PB10 PB11��ʼ����I2C2
* Input          : i2c_handler : I2C handler
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void I2C2_Init(I2C_HandleTypeDef *i2c_handler)
{
	GPIO_InitTypeDef          GPIO_InitStruct;

    //ʹ��GPIOBʱ��
    __HAL_RCC_GPIOB_CLK_ENABLE();

    //����SCL�ܽ�
    GPIO_InitStruct.Pin = I2C2_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C2_SCL_SDA_GPIO_PORT, &GPIO_InitStruct);
    //����SDA�ܽ�
    GPIO_InitStruct.Pin = I2C2_SDA_PIN;
    HAL_GPIO_Init(I2C2_SCL_SDA_GPIO_PORT, &GPIO_InitStruct);

    //ʹ��I2C2ʱ��
    __HAL_RCC_I2C2_CLK_ENABLE();
    /* Force the I2C peripheral clock reset */
    __HAL_RCC_I2C2_FORCE_RESET();
    /* Release the I2C peripheral clock reset */
    __HAL_RCC_I2C2_RELEASE_RESET();
    //����I2C�¼��ж�
    HAL_NVIC_SetPriority(I2C2_EV_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C2_EV_IRQn);
    //����I2C�����ж�
    HAL_NVIC_SetPriority(I2C2_ER_IRQn, 0x05, 0);
    HAL_NVIC_EnableIRQ(I2C2_ER_IRQn);
	
	if(HAL_I2C_GetState(i2c_handler) == HAL_I2C_STATE_RESET)
	{
		i2c_handler->Instance = I2C2;
		i2c_handler->Init.ClockSpeed       = BSP_I2C_SPEED;
		i2c_handler->Init.DutyCycle        = I2C_DUTYCYCLE_2;
		i2c_handler->Init.OwnAddress1      = 0;
		i2c_handler->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
		i2c_handler->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
		i2c_handler->Init.OwnAddress2      = 0;
		i2c_handler->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
		i2c_handler->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

		/* Init the I2C */
		HAL_I2C_Init(i2c_handler);
	}
}

//������
static void I2C2_Error(I2C_HandleTypeDef *i2c_handler, uint8_t Addr)
{
	/* De-initialize the I2C communication bus */
	HAL_I2C_DeInit(i2c_handler);
}

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
                                           uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(i2c_handler, Addr, (uint16_t)Reg, MemAddSize, Buffer, Length, 1000);

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* I2C error occurred */
		I2C2_Error(i2c_handler, Addr);
	}
	return status;    
}

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
                                            uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Write(i2c_handler, Addr, (uint16_t)Reg, MemAddSize, Buffer, Length, 1000);

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* Re-Initiaize the I2C Bus */
		I2C2_Error(i2c_handler, Addr);
	}
	return status;
}

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
                                           uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Master_Receive(i2c_handler, Addr, Buffer, Length, 1000);

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* I2C error occurred */
		I2C2_Error(i2c_handler, Addr);
	}
	return status;    
}

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
                                            uint16_t Length)
{
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Master_Transmit(i2c_handler, Addr, Buffer, Length, 1000);

	/* Check the communication status */
	if(status != HAL_OK)
	{
		/* Re-Initiaize the I2C Bus */
		I2C2_Error(i2c_handler, Addr);
	}
	return status;
}

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
HAL_StatusTypeDef I2C2_IsDeviceReady(I2C_HandleTypeDef *i2c_handler, uint16_t DevAddress, uint32_t Trials)
{ 
	return (HAL_I2C_IsDeviceReady(i2c_handler, DevAddress, Trials, 1000));
}






