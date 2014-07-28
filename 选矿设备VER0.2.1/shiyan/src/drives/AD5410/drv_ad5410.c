/****************************************Copyright (c)**************************************************
**                          大连金德姆电子有限公司
**                                        硬件部
**                                        产品一部 
**                                   http://www.modern.org
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: AD5410
**创   建   人: 刘及华
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
 函数名称：AD5410Init
 功能描述： DA芯片AD5410驱动初始化
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void AD5410Init(void)
{
	ToggleWD();

//  PINSEL1 = PINSEL1 & (~BIT10);
//  PINSEL1 = PINSEL0 & (~BIT11);		//P1.20 配置成GPIO
//
//  PINSEL1 = PINSEL0 & (~BIT12);
//  PINSEL1 = PINSEL0 & (~BIT13);		//P1.21 配置成GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT14);
//  PINSEL1 = PINSEL1 & (~BIT15);		//P1.22 配置成GPIO
//
//  PINSEL1 = PINSEL1 & (~BIT14);
//  PINSEL1 = PINSEL1 & (~BIT15);		//P1.23 配置成GPIO

	PINSEL2 = PINSEL2 & (~BIT3);        //P1.20 P1.21 P1.22 P1.23 配置成GPIO     

	IO0DIR |= BIT20;                    //p1.20 设置为输出
	IO0DIR |= BIT21;                    //p2.21 设置为输出
	IO0DIR |= BIT22;					//p3.22 设置为输出
	IO0DIR |= BIT23;					//p4.23 设置为输出

	DA_CLEAR_SET_LOW();

	ad5410_control(0x56, 0x0001);    	//复位
	ad5410_control(0x55, 0x1005);		//设置4-20ma工作模式
	ad5410_control(0x01, 0x00);			//稳定输出4mA电流
}

/******************************************************************************
 函数名称：ad5410_control
 功能描述：设置寄存器
 参数描述：
 参数名称： 输入/输出？ 类型		描述
  输入  	 unsigned char addr     地址
  输入  	 unsigned int data		配置字
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2014-07-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void ad5410_control(unsigned char addr, unsigned int data)
{
	DA_LATCH_SET_LOW();                   //latch为低电平

	ad5410_sendbyte(addr);
	ad5410_sendbyte(data/256);
	ad5410_sendbyte(data);

	DA_LATCH_SET_HIGH();                  //latch为高电平
}

/******************************************************************************
 函数名称：ad5410_sendByte
 功能描述：设置控制寄存器
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 输  入  	 unsigned int data		配置字 
   
 返  回  值：无
				   
 作 	 者 ：刘及华
 日 	 期：2014-07-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void ad5410_sendbyte(INT8U byte)
{
    unsigned char i;
    for ( i = 0; i < 8; i++)
    {
        DA_SCLK_SET_LOW(); 
        if ( byte & 0x80 )
          DA_SDIN_SET_HIGH();
        else
          DA_SDIN_SET_LOW(); 
        DA_SCLK_SET_HIGH(); 
        byte <<= 1;     
        DA_SCLK_SET_LOW();
    }
}


/******************************************************************************
 函数名称：ad5410Output
 功能描述：ad5410输出
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
void ad5410Output(INT16U data)
{
	INT8U i = 0;

	ToggleWD();

	DA_CLEAR_SET_LOW();

	for ( i=0; i<12; i++ )
	{
		DA_SCLK_SET_LOW();
		if ( ((data << i) & 0x8000) != 0 )
		{
			DA_SDIN_SET_HIGH();
		}
		else
		{
			DA_SDIN_SET_LOW();
		}
		DA_SCLK_SET_HIGH();
	}

	DA_CLEAR_SET_HIGH();

}
