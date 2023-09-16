
#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f1xx.h"
#include "stm32_types.h"
#include "stm32f1xx_hal.h"

#define KEY_SHAKE_DELAY	 5	//按键去抖动延时

/*
宏编译开关参数定义只能采用#define定义
不能采用enum定义
*/
#define UP_TRIGGER  1		//按键抬起触发
#define DOWN_TRIGGER  2		//按键按下触发

//配置按键触发方式
#define KEY_TRIGGER_MODE DOWN_TRIGGER	//按键触发方式

//按键值
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

//按键状态
enum key_state
{
	Key_UP = 1,
	Key_DOWN = 2,
	Key_KEEP = 3,
};

typedef struct
{
	u8   Num;		      //按键值
	u8   State;	          //按键状态
	u8   Num_Last;	      //上次按键值
	u8   Shake_LastNum;   //上次去抖动键值
	u32  Key_Delay_Cnt;   //按键去抖计数器
}_KEY_;

extern _KEY_ Key_Info; 

/*******************************************************************************
* Function Name  : Key_Init
* Description    : 按键IO初始化
* Input          : None
* Output         : None
* Return         : None
* Note			 : FIODIR的每一位对应一个IO口的输入输出配置,1为输出 0为输入
*******************************************************************************/
void Key_Init(void);

/*******************************************************************************
* Function Name  : Key_Read
* Description    : 按键读取,不支持多个按键同时按下
* Input          : None
* Output         : None
* Return         : None
* Note			 : 此函数支持按键按下触发和按键按下抬起触发,需要配置宏
				   KEY_TRIGGER_MODE选择触发方式
*******************************************************************************/
void Key_Read(void);

#endif	/* __KEY_H_ */
