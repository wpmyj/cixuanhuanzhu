/****************************************Copyright (c)**************************************************
**                               广州周立功单片机发展有限公司
**                                     研    究    所
**                                        产品一部 
**
**                                 http://www.zlgmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: uart0.h
**创   建   人: 陈明计
**最后修改日期: 2003年7月4日
**描        述: μCOS-II下LPC210x的UART0底层驱动头文件
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 陈明计
** 版  本: v1.0
** 日　期: 2003年7月4日
** 描　述: 原始版本
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 陈明计
** 日　期: 2003年7月9日
** 描　述: 更正注释
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef  __drv_ltc1448_h__
#define  __drv_ltc1448_h__

//#include		"Config.h"

#include 		"Os_cpu.h"
//#include 		"newbus.h"
#include 		"main.h"
#include		"LPC2294.h"
#include		"data_type.h"

#define DA_CLK_SET_LOW()  (IO0CLR = BIT21)
#define DA_CLK_SET_HIGH() (IO0SET = BIT21)

#define DA_DIN_SET_LOW()  (IO0CLR = BIT22)
#define DA_DIN_SET_HIGH() (IO0SET = BIT22)

#define DA_CS_SET_LOW()   (IO0CLR = BIT23)
#define DA_CS_SET_HIGH()  (IO0SET = BIT23)

/******************************************************************************
 函数名称：Ltc1448Init
 功能描述： DA芯片LTC1448驱动初始化
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Ltc1448Init(void);
/******************************************************************************
 函数名称：Ltc1448_CLK
 功能描述：LTC1448 SPI CLOCK一下
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Ltc1448_CLK(void);
/******************************************************************************
 函数名称：Ltc1448Delay
 功能描述：SPI Delay
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Ltc1448Delay(void);
/******************************************************************************
 函数名称：Ltc1448Output
 功能描述：LTC1448输出
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 channela	输入		INT16U		通道A输出值 0-4095 电流
 channela	输入		INT16U		通道B输出值 0-4095	电压
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Ltc1448Output(INT16U channela, INT16U channelb);

#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

