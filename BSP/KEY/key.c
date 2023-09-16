#include "key.h"
#include "delay.h"

_KEY_ Key_Info; 

/*******************************************************************************
* Function Name  : Key_Info_Init
* Description    : �����ṹ����Ϣ��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : None
*******************************************************************************/
void Key_Info_Init(void)
{
	Key_Info.Num = 0;
	Key_Info.State = Key_UP;		//��������̧��״̬
	Key_Info.Num_Last = 0;	
	Key_Info.Key_Delay_Cnt = 0;		//ȥ������������
}

/*******************************************************************************
* Function Name  : Key_Init
* Description    : ����IO��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : FIODIR��ÿһλ��Ӧһ��IO�ڵ������������,1Ϊ��� 0Ϊ����
*******************************************************************************/
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
  
	__GPIOA_CLK_ENABLE(); //ʹ��GPIOAʱ��
	__GPIOC_CLK_ENABLE(); //ʹ��GPIOAʱ��
  
    //����PA0 PA4 PA5Ϊ����,����
    GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_15;  
    GPIO_InitStruct.Mode = GPIO_PULLUP;           //����
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;               //����  
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;          //���� 
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;  
    GPIO_InitStruct.Pull = GPIO_PULLUP;               //����  
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	Key_Info_Init();		//������Ϣ��ʼ��
}

/*******************************************************************************
* Function Name  : Independent_Key_Scan
* Description    : ����ʽ����ɨ��
* Input          : None
* Output         : None
* Return         : ��ֵ,Ϊ0��ʾ�޼�����
* Note			 : ֻ��ȡ��ֵ,�����κ�ȥ������
*******************************************************************************/
u8 Independent_Key_Scan(void)
{
//	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_SET)   //KEY1��������
//	{
//		return KEY_ONE;	
//	}
	
	if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == GPIO_PIN_RESET)   //KEY2��������
	{
		return KEY_TWO;	
	}
    
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_RESET)   //KEY3��������
	{
		return KEY_THREE;	
	}
	
	return 0;
}  

/*******************************************************************************
* Function Name  : Key_Read
* Description    : ������ȡ,��֧�ֶ������ͬʱ����
* Input          : None
* Output         : None
* Return         : None
* Note			 : �˺���֧�ְ������´����Ͱ�������̧�𴥷�,��Ҫ���ú�
				   KEY_TRIGGER_MODEѡ�񴥷���ʽ
*******************************************************************************/
void Key_Read(void)
{
	u8 Key_CurrNum;		//��ǰ����ֵ
	
	//delay_ms(1);        //ÿ��1msɨ��һ�ΰ���
	
	Key_CurrNum = 0;	//ÿ�ζ�֮ǰ���ϴζ�����ֵ���

	Key_CurrNum = Independent_Key_Scan();	//��ȡ����	

	if((Key_Info.Shake_LastNum == Key_CurrNum)&&(Key_CurrNum != 0))	//ͬһ����������
	{
		Key_Info.Key_Delay_Cnt++; 			//ȥ������
	#if KEY_TRIGGER_MODE == DOWN_TRIGGER	//�������´���
		if(Key_Info.Key_Delay_Cnt > KEY_SHAKE_DELAY)	//������Ч������������������´���һ��
		{
			Key_Info.Key_Delay_Cnt = KEY_SHAKE_DELAY + 1;			//���������
			Key_Info.State = Key_KEEP;			//��������
		}
		
		if(Key_Info.Key_Delay_Cnt == KEY_SHAKE_DELAY)
		{
			Key_Info.Num = Key_Info.Shake_LastNum;	//����ֵ����
			Key_Info.State = Key_DOWN;			    //��������
		}
	#endif	
	}
	else		//�����ɿ�����������������ɿ����������Ը��ݰ������µĳ��̽��в�ͬ�Ĵ��������������̰��ɿ�����
	{
	#if KEY_TRIGGER_MODE == DOWN_TRIGGER	//�������´���
		Key_Info.Key_Delay_Cnt = 0;		//���������
		Key_Info.Num = 0;				//����ֵ����
		Key_Info.State = Key_UP; 		//����̧��
		goto KEY_END;

	#elif KEY_TRIGGER_MODE == UP_TRIGGER	//����̧�𴥷�
		if(Key_Info.Key_Delay_Cnt < KEY_SHAKE_DELAY)	//������Ч
		{
			Key_Info.Key_Delay_Cnt = 0;		//���������
			Key_Info.Num = 0;				//����ֵ����
			Key_Info.State = Key_UP; 		//����̧��
			goto KEY_END;
		}
		else	//������Ч
		{
			Key_Info.Key_Delay_Cnt = 0;			//���������
			Key_Info.Num = Key_Info.Shake_LastNum;	//��¼����ֵ
			Key_Info.State = Key_UP;
			goto KEY_END;
		}
	#endif
	}

KEY_END:
	Key_Info.Shake_LastNum = Key_CurrNum;	//��¼����ֵ
}


