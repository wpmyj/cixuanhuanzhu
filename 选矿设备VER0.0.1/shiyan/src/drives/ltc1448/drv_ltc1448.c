/****************************************Copyright (c)**************************************************
**                          大连金德姆电子有限公司
**                                        硬件部
**                                        产品一部 
**                                   http://www.modern.org
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: I2C_DRIVER
**创   建   人: 黄力国
**最后修改日期: 2004年12月28日
**描        述: μ/COS-II上的LPC2214的通用I2C  的驱动
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 黄力国
** 版  本 : v1.0
** 日　期: 2004年12月28日
** 描　述: 原始版本
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include "config.h"

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
void Ltc1448Init(void)
{
	ToggleWD();

	PINSEL1 = PINSEL1 & (~BIT10);
	PINSEL1 = PINSEL0 & (~BIT11);		//P0.21 配置成GPIO

	PINSEL1 = PINSEL0 & (~BIT12);
	PINSEL1 = PINSEL0 & (~BIT13);		//P0.22 配置成GPIO

	PINSEL1 = PINSEL1 & (~BIT14);
	PINSEL1 = PINSEL1 & (~BIT15);		//P0.23 配置成GPIO


	IO0DIR |= BIT21;                    //p0.21 设置为输出
	IO0DIR |= BIT22;					//p0.22 设置为输出
	IO0DIR |= BIT23;					//p0.23 设置为输出

	DA_CS_SET_HIGH();
	DA_CLK_SET_LOW();
	DA_DIN_SET_LOW();
}

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
void Ltc1448_CLK(void)
{
	Ltc1448Delay();
	DA_CLK_SET_HIGH();
	Ltc1448Delay();
	DA_CLK_SET_LOW();
	Ltc1448Delay();
}

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
void Ltc1448Delay(void)
{
//  Sleep10us(1);
	INT8U i = 0;
	for ( i=0; i<5; i++ )			   //about 10us
		ToggleWD();
}

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
void Ltc1448Output(INT16U channela, INT16U channelb)
{
	INT8U i = 0;
//	INT16U result = 0;
	INT16U send = 0;

	ToggleWD();

	DA_CLK_SET_LOW();
	Ltc1448Delay();
	DA_CS_SET_LOW();
	Ltc1448Delay();

	send = channela << 4;

	for ( i=0; i<12; i++ )
	{
		if ( ((send << i) & 0x8000) != 0 )
		{
			DA_DIN_SET_HIGH();
		}
		else
		{
			DA_DIN_SET_LOW();
		}
		Ltc1448_CLK();
	}

	send = channelb << 4;

	for ( i=0; i<12; i++ )
	{
		if ( ((send << i) & 0x8000) != 0 )
		{
			DA_DIN_SET_HIGH();
		}
		else
		{
			DA_DIN_SET_LOW();
		}
		Ltc1448_CLK();
	}

	DA_CS_SET_HIGH();

}
