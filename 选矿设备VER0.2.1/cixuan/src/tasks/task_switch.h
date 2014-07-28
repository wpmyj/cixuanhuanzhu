/******************************************************************************
Copyright 2005 大连金德姆电子有限公司
All rights riserved.

文件名  ：task_pushc.h
模块名称：推拉C型架任务的头文件

当前版本：0.0.1
修改人	：许岩
完成日期：2005.04.06
升级说明：create
******************************************************************************/
#ifndef __TASK_PUSHC_H__
#define __TASK_PUSHC_H__

#include "data_type.h"




/******************************************************************************
 函数名称：Task_PushC
 功能描述：推拉C型架任务
 参数描述：
 参数名称： 输入/输出？ 类型		描述
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void Task_Switch0(void * data);
void Task_Switch1(void * data);
void Task_Switch2(void * data);
void Task_Switch3(void * data);
void Task_Switch4(void * data);
void Task_Switch5(void * data);
void Task_Switch6(void * data);
void Task_Switch7(void * data);



#endif

