
#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

#define KEY_SHAKE_DELAY	 5	//����ȥ������ʱ

/*
����뿪�ز�������ֻ�ܲ���#define����
���ܲ���enum����
*/
#define UP_TRIGGER  1		//����̧�𴥷�
#define DOWN_TRIGGER  2		//�������´���

//���ð���������ʽ
#define KEY_TRIGGER_MODE DOWN_TRIGGER	//����������ʽ

//����ֵ
enum key_num
{
	KEY_ONE = 1,
	KEY_TWO = 2,
	KEY_THREE = 3,
	KEY_FOUR = 4,
	KEY_FIVE = 5,
	KEY_SIX = 6,
	KEY_SEVEN = 7,
	KEY_EIGHT = 8,
	KEY_NINE = 9,
};

//����״̬
enum key_state
{
	Key_UP = 1,
	Key_DOWN = 2,
	Key_KEEP = 3,
};

typedef struct
{
	u8   Num;		      //����ֵ
	u8   State;	          //����״̬
	u8   Num_Last;	      //�ϴΰ���ֵ
	u8   Shake_LastNum;   //�ϴ�ȥ������ֵ
	u32  Key_Delay_Cnt;   //����ȥ��������
}_KEY_;

extern _KEY_ Key_Info; 

/*******************************************************************************
* Function Name  : Key_Init
* Description    : ����IO��ʼ��
* Input          : None
* Output         : None
* Return         : None
* Note			 : FIODIR��ÿһλ��Ӧһ��IO�ڵ������������,1Ϊ��� 0Ϊ����
*******************************************************************************/
void Key_Init(void);

/*******************************************************************************
* Function Name  : Key_Read
* Description    : ������ȡ,��֧�ֶ������ͬʱ����
* Input          : None
* Output         : None
* Return         : None
* Note			 : �˺���֧�ְ������´����Ͱ�������̧�𴥷�,��Ҫ���ú�
				   KEY_TRIGGER_MODEѡ�񴥷���ʽ
*******************************************************************************/
void Key_Read(void);

#endif	/* __KEY_H_ */
