/****************************************Copyright (c)**************************************************
**                               大连华科仪器有限公司
**                                        硬件部
**                                        产品一部 
**                                  
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Rtc_DRIVER
**创   建   人: 黄力国
**最后修改日期: 2004年12月28日
**描        述: μ/COS-II上的LPC2214的通用Rtc  的驱动
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
#include		"config.h"

extern 		OS_EVENT			*Rtc_Sem;


/*****************************************************************
函数原型：Rtc_init
功能描述：Rtc总线初始化，在应用函数中首先调用本函数
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者： 
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Rtc_Init(void)
{
	ToggleWD();

	PINSEL0 = PINSEL0 & (~BIT13);
	PINSEL0 = PINSEL0 & (~BIT12);        //P0.6配置成GPIO
	IO0DIR |= BIT6;								//P0.6置成输出		SCL

	PINSEL0 = PINSEL0 & (~BIT15);
	PINSEL0 = PINSEL0 & (~BIT14);		//P0.7配置成GPIO
	IO0DIR |= BIT7;								//P0.7置成输出		SDA

	RX8025_SCL_SetLow();
	Rtc_Stop();
}

/*****************************************************************
函数原型：Rtc_start
功能描述：提供Rtc总线工作时序中的起始位
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：ok(0)-建立成功
				notok(0xFF)-建立失败
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Rtc_Start(void)
{
	//ToggleWD();

	Rtc_Delay();
	RX8025_SDA_SetHigh();
	Rtc_Delay();
	RX8025_SCL_SetHigh();
	Rtc_Delay();
	if ( Rtc_GetSDAStat() == 0 )
	{
		return notok;
	}
	if ( Rtc_GetSCLStat() == 0 )
	{
		return notok;
	}

	RX8025_SDA_SetLow();
	Rtc_Delay();
	RX8025_SCL_SetLow();
	Rtc_Delay();
	return ok;
}

/*****************************************************************
函数原型：Rtc_stop
功能描述：提供Rtc总线工作时序中的停止位
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Rtc_Stop(void)
{
//  ToggleWD();

	RX8025_SDA_SetLow();
	Rtc_Delay();
	RX8025_SCL_SetHigh();
	Rtc_Delay();
	RX8025_SDA_SetHigh();
	Rtc_Delay();

	SleepMs(5);
}

/*****************************************************************
函数原型：Rtc_Ack
功能描述：向Rtc总线发送一个应答信号ACK，一般用于连续数据读取时。
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：无
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Rtc_Acknowledge(void)
{
// 	RX8025_SDA_SetLow();
// 	(void)Rtc_Clock();
// 	RX8025_SDA_SetHigh();

//  ToggleWD();

	Rtc_Delay();
	RX8025_SDA_SetLow();
	Rtc_Delay();
	RX8025_SCL_SetHigh();
	Rtc_Delay();
	RX8025_SCL_SetLow();
	Rtc_Delay();

}

/*****************************************************************
函数原型：Rtc_Nack
功能描述：伪应答
参数描述：
参数名称：  输入/输出？ 类型        描述
-----------     ----------- ------      -------
// 返  回  值：无
作      者  ：许岩
日      期：2003-05-12
修改历史：
日期        修改人      修改描述
------      ---------   -------------
*****************************************************************/
void Rtc_Nack(void)
{
//  ToggleWD();

    Rtc_Delay();
	RX8025_SDA_SetHigh();
    Rtc_Delay();
	RX8025_SCL_SetHigh();
    Rtc_Delay();
	RX8025_SCL_SetLow();
    Rtc_Delay();
}

/*****************************************************************
函数原型：bit    Rtc_clock
功能描述：提供 Rtc 总线的时钟信号，并返回在时钟电平为
高期间SDA信号线上状态，本           函数可用于数据发送，
也可用于数据接收。
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：SDA信号线的状态(0|1)
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Rtc_Clock(void)
{
	INT8U h;

//  ToggleWD();

	Rtc_Delay();
	RX8025_SCL_SetHigh();
	Rtc_Delay();
	h = Rtc_GetSDAStat();
	RX8025_SCL_SetLow();
	Rtc_Delay();

	if (h == 0)
		return ok;
	else
		return notok;
// 	return h;
}

/*****************************************************************
函数原型：delay
功能描述：本函数实际上只有一条返回命令，在具体应用中可视具体要求增加延时指令
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：许岩
日      期	：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Rtc_Delay(void)
{
//  ToggleWD();

//  Sleep10us(10);
	Sleep10us(9);
}

/*****************************************************************
函数原型：delay
功能描述：本函数实际上只有一条返回命令，在具体应用中可视具体要求增加延时指令
参数描述：无
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者	：许岩
日      期	：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void Rtc_Delay2(void)
{
//  ToggleWD();

	Sleep10us(1);
}

/*****************************************************************
函数原型：bit   Rtc_send
功能描述：向Rtc总线发送8 位数据,并请求一个应答信号ACK。
			      返回SDA 信号线上状态
参数描述：发送的一字节8位发送数据
参数名称：	输入/输出？	类型		描述
data_to_send    输入	INT8U     发送的字节

返  回  值：ok(0)-正确
				notok(0xFF)-错误
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Rtc_Send(INT8U data_to_send)
{
	INT8U LOCAL_V_TYPE i;
	INT8U LOCAL_V_TYPE h;

	//ToggleWD();

	for ( i=0; i<8; i++ )
	{
		if ( (data_to_send << i) & 0x80 )
		{
			RX8025_SDA_SetHigh();
		}
		else
		{
			RX8025_SDA_SetLow();
		}
		Rtc_Delay();
		RX8025_SCL_SetHigh();
		Rtc_Delay();
		RX8025_SCL_SetLow();
		Rtc_Delay();
	}
	RX8025_SDA_SetHigh();

// 	IO1DIR &= (~BIT25);							//P1.25置成输入		SDA

	Rtc_Delay();
	RX8025_SCL_SetHigh();
	Rtc_Delay();
	h = Rtc_GetSDAStat();
	RX8025_SCL_SetLow();
	Rtc_Delay();

// 	IO1DIR |= BIT25;							//P1.25置成输出		SDA

 	return h;
}

/*****************************************************************
函数原型：Rtc_receive
功能描述：从Rtc总线接收8位数据信号，并将接收到的8位数据作为一个字节返回，不回送应答信号ACK。
主函数在调用本函数之前应保证SDA信号线处于浮置状态，既使8051该脚电平为1。
参数描述：
参数名称：	输入/输出？	类型		描述

返  回  值：接受到的一个字节数据
作      者	：黄立国、许岩
日      期：2003-05-12
修改历史：
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Rtc_Receive(void)
{
    INT8U       data_received = 0;
    INT8U       i;

	//ToggleWD();

	RX8025_SDA_SetHigh();

	for ( i=0; i<8; i++ )
	{
		RX8025_SCL_SetHigh();
		Rtc_Delay();

		data_received <<= 1;

		if (Rtc_GetSDAStat() == 1)
		{
			data_received += 1;
		}
// 		Rtc_Delay();
		RX8025_SCL_SetLow();
		Rtc_Delay();

	}

// 	RX8025_SCL_SetHigh();
// 	Rtc_Delay();

	return data_received;

}

/*****************************************************************
函数原型：Rtc_GetSDAStat
功能描述：取得Rtc总线中SDA的状态
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：0-LOW
				1-HIGH
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Rtc_GetSDAStat(void)
{
	INT8U i = 0;

	//ToggleWD();

// 	IO1DIR &= (~BIT25);							//P1.25置成输入		SDA
	IO0DIR &= (~BIT6);								//P0.6置成输入		SDA
//  IO0DIR &= (~BIT15);                             //P0.15置成输入     SDA

	Rtc_Delay2();

// 	if ( (IO1PIN & BIT25) == 0 )
	if ( (IO0PIN & BIT6) == 0 )
//  if ( (IO0PIN & BIT15) == 0 )
	{
		i = 0;
	}
	else
	{
		i = 1;
	}

// 	IO1DIR |= BIT25;							//P1.25置成输出		SDA
	IO0DIR |= BIT6;								//P0.7置成输出		SDA
//  IO0DIR |= BIT15;                                //P0.15置成输出     SDA

	return i;
}

/*****************************************************************
函数原型：Rtc_GetSCLStat
功能描述：取得Rtc总线中SCL的状态
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：0-LOW
				1-HIGH
作      者	：许岩
日      期：2003-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U Rtc_GetSCLStat(void)
{
	INT8U i = 0;

	//ToggleWD();

	IO0DIR &= (~BIT7);								//P0.7置成输入		SCL

	Rtc_Delay2();

	if ( (IO0PIN & BIT7) == 0 )
	{
		i = 0;
	}
	else
	{
		i = 1;
	}

	IO0DIR |= BIT7;								//P0.7置成输出		SCL

	return i;
}

/*******************************************************************
向有子地址器件读取多字节数据函数               
函数原型: bit  ISendStr(INT8U sla,INT8U suba,ucahr *s,INT8U no);  
功能:     从启动总线到发送地址，子地址,读数据，
结束总线的全过程,从器件地址sla，子地址suba，
读出的内容放入s指向的存储区，读no个字节。
如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
INT8U Rtc_SendAndReceive(INT8U sla,INT8U suba,INT8U *s,INT8U no)
{
	INT8U i = 0;
	INT8U j = 0;
	INT8U err = 0;

	//ToggleWD();

	OSSemPend(Rtc_Sem, 0, &err);
	OS_ENTER_CRITICAL();

	i = Rtc_Start();					//如果Rtc_Start不过，重试10次
	if ( i != ok )
	{
		j = 20;
		while ( j )
		{
// 			Rtc_Nack();			//add by xuyan for debug
			Rtc_Stop();
			i = Rtc_Start();
			if ( i != ok )
			{
				j--;
			}
			else
			{
				break;
			}
			if ( j == 0 )
			{
				Rtc_Stop();
				OS_EXIT_CRITICAL();
				OSSemPost(Rtc_Sem);
				return notok;
			}
		}
	}

	i = Rtc_Send(sla);
	if (i != ok)
	{
		Rtc_Stop();
		OS_EXIT_CRITICAL();
		OSSemPost(Rtc_Sem);
		return notok;
	}

	i = Rtc_Send(suba);
	if (i != ok)
	{
		Rtc_Stop();
		OS_EXIT_CRITICAL();
		OSSemPost(Rtc_Sem);
		return notok;
	}

	(void)Rtc_Start();

	i = Rtc_Send(sla + 1);
	if (i != ok)
	{
		Rtc_Stop();
		OS_EXIT_CRITICAL();
		OSSemPost(Rtc_Sem);
		return notok;
	}

	for ( i=0; i<no-1; i++ )
	{
		*s=Rtc_Receive();				/*发送数据*/
		Rtc_Acknowledge();				   /*发送就答位*/  
		s++;
	} 
	*s=Rtc_Receive();

// 	RX8025_SDA_SetHigh();
// 	IO0DIR |= BIT3;								//P0.3置成输出		SDA

	Rtc_Nack();					/*发送非应位*/
	Rtc_Stop();					   /*结束总线*/ 

	OS_EXIT_CRITICAL();
	OSSemPost(Rtc_Sem);

	return ok;

}

