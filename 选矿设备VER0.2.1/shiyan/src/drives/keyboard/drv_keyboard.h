/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名  ：drv_keyboard.h
模块名称：键盘驱动的头文件

当前版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create
******************************************************************************/
#ifndef __DRV_KEYBOARD_H__
#define __DRV_KEYBOARD_H__

#include "data_type.h"


#define  RESET_7290Low()  		(IO0CLR = BIT27)
#define  RESET_7290High()		 (IO0SET = BIT27)


// #define Key_F1  12
// #define Key_F2  13
// //#define Key_multiply  14//*
// #define Key_ESC 15     //
// //#define Key_subtract  16//-
// //#define Key_equal 17//=
// #define Key_enter   18
// #define ID_NUM_OVER 50


#define ZLG7290ADDR     0x70

#define SlvZlg7290 0x70 //ZLG7290 从地址
#define SubKey 0x1 //键码值子地址


#define NOTOK	notok
#define CLCLTRGOON		0x60		/***键值过滤的结果，表示可以继续***/
#define CLCLTRSTOOP		0x06		/***键值过滤的结果，表示不能进行计算操作***/
#define CLCLTRSNDMLBX			0x3c		/***要求往邮箱里发送信息***/

////////////////////////////////////////////////////////////////////
//定义键值
///////////////////////////////////////////////////////////////////
#define 	KEY_0		0x0B
#define 	KEY_1		0x01
#define 	KEY_2		0x02
#define 	KEY_3		0x03
#define 	KEY_4		0x04
#define 	KEY_5		0x05
#define 	KEY_6		0x06
#define 	KEY_7		0x07
#define 	KEY_8		0x08
#define 	KEY_9		0x0A
// #define 	KEY_POINT	0x13
// #define 	KEY_ADD		0x14
// #define 	KEY_SUB		0x18
// #define 	KEY_MUL		0x0c
// #define 	KEY_EQUAL	0x02
#define 	KEY_ENTER	0x0C
#define 	KEY_CANCEL	0x09
// #define 	KEY_MODE	0x04
// #define 	KEY_FUNC	0x06
#define		NO_KEY	(INT8U)0xFF

// #define     KEY_MENU    0x04
// #define     KEY_F3      0x06


// typedef struct
// {
//     FP32    keyresult;  /***键盘输入的数值***/
//     INT8U   keyfunction;        /***功能键标志***/
// }   KEYMBOX ;




/******************************************************************************
 函数名称：
 功能描述：外部中断3 ( 即键盘中断初始化)
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：何文荣
 日 	 期：2005-04-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U Eint3KeyInit(void);



/******************************************************************************
 函数名称：
 功能描述：外部中断3 ( 即键盘中断) 函数
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：何文荣
 日 	 期：2005-04-18
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void    Eint3Key_Exception(void);			/***需在IRQ.S  中声明成句柄***/



/******************************************************************************
函数名称：keyboard_init
功能描述：键盘控制芯片ZLG7290初始化
参数描述：

返  回  值	：无
作      者	：许岩
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void keyboard_init(void);



/******************************************************************************
函数名称：RESET_7290() 
功能描述：用I/O和7290复位
参数描述：

返  回  值	：无
作      者	：许岩
日      期：2005-10-10
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
void RESET_7290(void);



/******************************************************************************
函数名称：get_key
功能描述：得到一个按键，死等
参数描述：无

返  回  值	：该键值
作      者	：张坛
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
INT8U get_key(void);



/******************************************************************************
函数名称：query_key
功能描述：用查询方式得到一个按键，不死等
参数描述：无

返  回  值	：该键值
作      者	：许岩
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
INT8U query_key(void);



/******************************************************************************
函数名称：query_key_nsnd
功能描述：用查询方式得到一个按键，不死等，比query_key省去语音部分
参数描述：无

返  回  值	：该键值
作      者	：许岩
日      期：2005-04-08
修改历史：
日期		修改人		修改描述
------		---------	---------
******************************************************************************/
INT8U query_key_nsnd(void);



#endif

