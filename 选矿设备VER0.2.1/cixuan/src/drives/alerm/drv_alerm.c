/****************************************Copyright (c)**************************************************
**                                  大连易达通电子技术有限公司
**                                         硬件部
**                                        产品一部 
**                                  
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: alerm_driver
**创   建   人: 刘及华
**最后修改日期: 2014年7月28日
**描        述: μ/COS-II上的LPC2214的通用报警门的驱动
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 
** 日　期:
** 描　述:
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#include		"config.h"

/*****************************************************************
函数原型：alerm_init
功能描述：报警开关部分，初始化设置
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------

返  回  值：无
作      者：刘及华 
日      期：2014-05-12
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
void alerm_door_Init(void)
{
	ToggleWD();

	PINSEL1 = PINSEL1 & (~BIT24);
	PINSEL1 = PINSEL1 & (~BIT25);       //P0.28配置成GPIO
	IO0DIR |= BIT28;					//P0.28置成输出  门控锁

	PINSEL2 = PINSEL2 & (~BIT5);
	PINSEL2 = PINSEL2 & (~BIT4);		//P3.29配置成GPIO
	IO3DIR |= BIT29;					//P3.29置成输出  报警灯

	set_close_door();
	set_alerm_stop();
}


