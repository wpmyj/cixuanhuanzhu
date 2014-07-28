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
#ifndef  __uart_driver_
#define __uart_driver_

//#include		"Config.h"

#include 		"Os_cpu.h"
//#include 		"newbus.h"
#include 		"main.h"
#include		"LPC2294.h"
#include		"data_type.h"

// #define     UART0_SEND_QUEUE_LENGTH     60
// #define     UART1_SEND_QUEUE_LENGTH     60
#define		UART0_SEND_QUEUE_LENGTH		50
#define		UART0_RCV_QUEUE_LENGTH		50

#define		UART1_SEND_QUEUE_LENGTH		512
#define     UART1_RCV_QUEUE_LENGTH      512

//#define COM_SND_BUF_SIZE    256                 //定义串口接收缓冲区的大小
//#define COM_RCV_BUF_SIZE    256                 //定义串口接收缓冲区的大小

#define UART0_FIFO_LENGTH         	16
#ifndef NOT_OK
#define NOT_OK              			0xff                        /* 参数错误                                     */
#endif

#define QUEUE_FULL          			8                           /* 队列满                                       */
#define QUEUE_EMPTY         			4                           /* 无数据                                       */
#define QUEUE_OK            			1                           /* 操作成功                                     */

#define Q_WRITE_MODE        		1                           /* 操作成功                                     */
#define Q_WRITE_FRONT_MODE  	2                           /* 操作成功                                     */

/*communication message struct*/
//typedef struct _tagmsg
//{
//    INT8U   msg_type;
//    INT8U   length;
//    INT8U   rcvbuf[COM_RCV_BUF_SIZE];
//}MSG;
//
//__packed struct CMD
//{
//    INT8S   id[8];
//    INT8S   cmd[4];
//    INT8S   len[3];
//    INT8S   DATA[100];
//};
//
//__packed struct CMD_LONG
//{
//    INT8S   id[8];
//    INT8S   cmd[4];
//    INT8S   len[3];
//    INT8S   DATA[1009];
//};


#define EN_QUEUE_WRITE      		1
#define EN_QUEUE_NDATA    		1
#define EN_QUEUE_SIZE    			1
#define EN_QUEUE_FLUSH				1

#ifndef QUEUE_DATA_TYPE
	#define QUEUE_DATA_TYPE     		uint8
#endif

//设置奇偶校验位时会用到的宏定义
#define NONE	0			
#define ODD		1
#define EVEN	2

//专供ChangeComChannel和CurComChannel使用的定义
// #define CHANNL_PSAM_PRI_T 0
// #define CHANNL_PSAM_PRI_R 1
// #define CHANNL_PSAM_SLV_T 2
// #define CHANNL_PSAM_SLV_R 3
// #define CHANNL_COM1_TR 4
// #define CHANNL_COM2_TR 5
// #define CHANNL_DATA_BOX_TR 7
#define CHANNL_COM1_TR			0
#define CHANNL_PSAM_SLV_T		1
#define CHANNL_PSAM_SLV_R		2
#define CHANNL_PSAM_PRI_T		3
#define CHANNL_PSAM_PRI_R		4
#define CHANNL_ICCARD_T			5
#define CHANNL_ICCARD_R			6
#define CHANNL_485_TR				7


//#define A4051SetLow()   (IO0CLR = BIT21)
//#define A4051SetHigh()  (IO0SET |= BIT21)
//
//#define B4051SetLow()   (IO0CLR = BIT22)
//#define B4051SetHigh()  (IO0SET |= BIT22)
//
//#define C4051SetLow()   (IO0CLR = BIT23)
//#define C4051SetHigh()  (IO0SET |= BIT23)

#define C485_0SetLow()    (IO0CLR = BIT23)
#define C485_0SetHigh()   (IO0SET |= BIT23)

#define Set485_0Input()		C485_0SetLow()
#define Set485_0Output()	C485_0SetHigh()

#define C485_1SetLow()    (IO0CLR = BIT24)
#define C485_1SetHigh()   (IO0SET |= BIT24)

#define Set485_1Input()		C485_1SetLow()
#define Set485_1Output()	C485_1SetHigh()



/*********************************************************************************************************
** 函数名称: UART0Init
** 功能描述: 初始化UART0 
** 输　入: bps：波特率
**
** 输　出:TRUE  :成功
**        FALSE:失败
** 全局变量: Uart0_Sem
** 调用模块: QueueCreate,OSSemCreate
********************************************************************************************************/
uint8 UART0_Init(uint32 bps);

/******************************************************************************
 函数名称：UART0_SetBPS
 功能描述：设置串口0波特率，和奇偶校验方式
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 bps			输入			INT32U		要设置的波特率
 odd_even	输入		INT8U		校验方式：NONE为无，ODD为奇校验，EVEN为偶校验
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void UART0_SetBPS(INT32U bps, INT8U odd_even);

/*********************************************************************************************************
** 函数名称: UART0Putch
** 功能描述: 发送一个字节数据 
** 输　入: Data：发送的数据
** 输　出:无
** 全局变量: 无
** 调用模块: QueueWrite,QueueRead
********************************************************************************************************/
void UART0Putch(uint8 Data);
        

/*********************************************************************************************************
** 函数名称: UART0Write
** 功能描述: 发送多个字节数据 
** 输　入: Data:发送数据存储位置
**        NByte:发送数据个数
** 输　出:无
** 全局变量: 无
** 调用模块: UART0Putch
********************************************************************************************************/
// void UART0Write(uint8 *Data, uint16 NByte);
void UART0Write(INT8U const * const snd_buf, INT16U const snd_length);
    
/*********************************************************************************************************
** 函数名称: UART0Getch
** 功能描述: 接收一个字节数据
** 输　入: 无
** 输　出: 接收到的数据
** 全局变量: 无
** 调用模块: OSSemPend
********************************************************************************************************/
uint8 UART0Getch(void);    


/*********************************************************************************************************
** 函数名称: UART0_Exception
** 功能描述: UART0中断服务程序
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: OSSemPost,QueueRead
********************************************************************************************************/
 void UART0_Exception(void);



/******************************************************************************
 函数名称：CleanCom0Buf
 功能描述：清空串口0缓存
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void CleanCom0Buf(void);



/******************************************************************************
 函数名称：com0_send_hex
 功能描述：
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 str_to_send	输入	INT8U *		要发送的字符串
 len				输入	INT16U		长度
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void com0_send_hex(INT8U * str_to_send, INT16U len);



/*****************************************************************
函数原型：com0_gets
功能描述：串口1接收
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
str_got		输出			INT8U*		得到的字符串
timeout_start输入			INT16U		首字超时，单位mS，0死等
timeout		输入			INT16U		首字超时，单位mS，0死等
// endchar		输入			INT8U		提示符

 返  回  值：ok(0)-成功
				 KEY_CANCEL-按退出键了
				 notok(0xFF)-失败，超时
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U com0_gets(INT8U *str_got, INT16U *rcv_len, INT16U timeout_start, INT16U timeout);


/*********************************************************************************************************
** 函数名称: UART0Init
** 功能描述: 初始化UART0 
** 输　入: bps：波特率
**
** 输　出:TRUE  :成功
**        FALSE:失败
** 全局变量: Uart0Sem
** 调用模块: QueueCreate,OSSemCreate
********************************************************************************************************/
uint8 UART1_Init(uint32 bps);

/*********************************************************************************************************
** 函数名称: UART0Putch
** 功能描述: 发送一个字节数据 
** 输　入: Data：发送的数据
** 输　出:无
** 全局变量: 无
** 调用模块: QueueWrite,QueueRead
********************************************************************************************************/
void UART1Putch(uint8 Data);
        

/*********************************************************************************************************
** 函数名称: UART0Write
** 功能描述: 发送多个字节数据 
** 输　入: Data:发送数据存储位置
**        NByte:发送数据个数
** 输　出:无
** 全局变量: 无
** 调用模块: UART0Putch
********************************************************************************************************/
// void UART1Write(uint8 *Data, uint16 NByte);
void UART1Write(INT8U const *const snd_buf, INT16U const snd_length);
    
/*********************************************************************************************************
** 函数名称: UART0Getch
** 功能描述: 接收一个字节数据
** 输　入: 无
** 输　出: 接收到的数据
** 全局变量: 无
** 调用模块: OSSemPend
********************************************************************************************************/
uint8 UART1Getch(void);    



/******************************************************************************
 函数名称：UART1GetchPSAM
 功能描述：PSAM卡接收函数
 参数描述：
 参数名称：	输入/输出？	类型		描述
 time_out	输入		INT16U		设定超时，time_outx10 ms
 err			输出		INT8U*		错误状态
				
 返  回  值：接收到的数
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U UART1GetchPSAM(const INT16U timeout, INT8U * const err);



/*********************************************************************************************************
** 函数名称: UART0_Exception
** 功能描述: UART0中断服务程序
** 输　入: 无
**
** 输　出: 无
**         
** 全局变量: 无
** 调用模块: OSSemPost,QueueRead
********************************************************************************************************/
 void UART1_Exception(void);
/*********************************************************************************************************
** 函数名称: QueueCreate
** 功能描述: 初始化数据队列
** 输　入: Buf      ：为队列分配的存储空间地址
**         SizeOfBuf：为队列分配的存储空间大小（字节）
**         ReadEmpty：为队列读空时处理程序
**         WriteFull：为队列写满时处理程序
** 输　出: NOT_OK  :参数错误
**         QUEUE_OK:成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint8 QueueCreate(void *Buf,uint32 SizeOfBuf,uint8 (* ReadEmpty)(),uint8 (* WriteFull)());

/*********************************************************************************************************
** 函数名称: QueueRead
** 功能描述: 获取队列中的数据
** 输　入: Ret:存储返回的消息的地址
**		   Buf:指向队列的指针
** 输　出: NOT_OK	  ：参数错误
**		   QUEUE_OK   ：收到消息
**		   QUEUE_EMPTY：队列空
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint8 QueueRead(QUEUE_DATA_TYPE *Ret, void *Buf);

 
/*********************************************************************************************************
** 函数名称: QueueWrite
** 功能描述: FIFO方式发送数据
** 输　入: Buf :指向队列的指针
**		   Data:发送的数据
** 输　出: NOT_OK	：参数错误
**		   QUEUE_FULL:队列满
**		   QUEUE_OK  :发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint8 QueueWrite(void *Buf, QUEUE_DATA_TYPE Data);


/*********************************************************************************************************
** 函数名称: QueueWrite
** 功能描述: LIFO方式发送数据
** 输　入: Buf:指向队列的指针
**		   Data:消息数据
** 输　出: QUEUE_FULL:队列满
**		   QUEUE_OK:发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint8 QueueWriteFront(void *Buf, QUEUE_DATA_TYPE Data);

 
/*********************************************************************************************************
** 函数名称: QueueNData
** 功能描述: 取得队列中数据数
** 输　入: Buf:指向队列的指针
** 输　出: 消息数
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint16 QueueNData(void *Buf);


/*********************************************************************************************************
** 函数名称: QueueSize
** 功能描述: 取得队列总容量
** 输　入: Buf:指向队列的指针
** 输　出: 队列总容量
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
uint16 QueueSize(void *Buf);

 
/*********************************************************************************************************
** 函数名称: OSQFlush
** 功能描述: 清空队列
** 输　入: Buf:指向队列的指针
** 输　出: 无
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
void QueueFlush(void *Buf);



/******************************************************************************
 函数名称：ChangeComChannel
 功能描述：改变串口通道函数，通过4051
 参数描述：
 参数名称：	输入/输出？	类型		描述
 com_channel	输入	INT8U	要改为的通道
				
 返  回  值：无
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void ChangeComChannel(INT8U com_channel);



/******************************************************************************
 函数名称：BCC_Check
 功能描述：对接收到的数据进行BCC校验
 参数描述：
 参数名称：	输入/输出？	类型		描述
 rcv_buf	输入		const INT8U*		要进行BCC校验的数据缓冲区
 rcvd_len	输入		INT8U		接收到的长度
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败(BCC校验错误)
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U BCC_Check(const INT8U *rcv_buf, INT8U rcvd_len);



// /******************************************************************************
//  函数名称：Cal_BCC
//  功能描述：对接收到的数据进行BCC校验
//  参数描述：
//  参数名称：	输入/输出？	类型		描述
//  buf		输入		const INT8U*		要进行BCC计算的数据缓冲区
//  len		输入		INT16U		长度
//
//  返  回  值：计算出来的BCC值
//
//  作      者	：许岩
//  日      期：2004-09-02
//  修改历史：
// 		日期		修改人		修改描述
// 		------		---------	-------------
// ******************************************************************************/
// INT16U Cal_BCC(const INT8U *buf, INT16U len);



/******************************************************************************
 函数名称：ComCreateConnctn
 功能描述：上层函数，把初始化串口和切换通道集成在一起
 参数描述：
 参数名称：	输入/输出？	类型		描述
 bps		输入			INT32U		要设置的波特率为115200-9600
 com_channel	输入	INT8U	要改为的通道
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U ComCreateConnctn(INT32U bps, INT8U com_channel);



/******************************************************************************
 函数名称：UART1SetBPS
 功能描述：设置UART1的波特率和奇偶校验位
 参数描述：
 参数名称：	输入/输出？	类型		描述
 bps		输入			INT32U		要设置的波特率为115200-9600
 data_lenth	输入		INT8U		长度，＝8八位 ＝9九位 
				
 返  回  值：无
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void UART1SetBPS(INT32U bps, INT8U data_lenth);



/*********************************************************************************************************
** 函数名称: UART1WritePSAM
** 功能描述: 发送多个字节数据
** 输　入: Data:发送数据存储位置
**        NByte:发送数据个数
** 输　出:无
** 全局变量: 无
** 调用模块: UART0Putch
**-------------------------------------------------------------------------------------------------------
** 修改人: 许岩
** 日　期: 2005年10月26日
**-------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void UART1WritePSAM(INT8U const *const snd_buf, INT16U const snd_length);



/*********************************************************************************************************
** 函数名称: UART1PutchPSAM
** 功能描述: 发送一个字节数据
** 输　入: Data：发送的数据数据
** 输　出:无
** 全局变量: 无
** 调用模块: 无
**
** 作　者: 许岩
** 日　期: 2005年10月26日
**-------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void UART1PutchPSAM(uint8 Data);



/******************************************************************************
 函数名称：CRC_Check
 功能描述：对接收到的数据进行CRC校验
 参数描述：
 参数名称：	输入/输出？	类型		描述
 rcv_buf	输入		const INT8U*		要进行BCC校验的数据缓冲区
 rcvd_len	输入		INT8U		接收到的长度
				
 返  回  值：ok(0)-成功
				 notok(0xFF)-失败(BCC校验错误)
				   
 作      者	：许岩
 日      期：2004-09-02
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
INT8U CRC_Check(const INT8U *rcv_buf, INT16U rcvd_len);



/******************************************************************************
 函数名称：com1_send
 功能描述：
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 str_to_send	输入			char *		要发送的字符串
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void com1_send(char * str_to_send);



/******************************************************************************
 函数名称：com1_send_hex
 功能描述：
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 str_to_send	输入	INT8U *		要发送的字符串
 len				输入	INT16U		长度
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void com1_send_hex(INT8U * str_to_send, INT16U len);



/*****************************************************************
函数原型：com1_gets2
功能描述：通讯处理
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
str_got		输出			INT8U*		得到的字符串
timeout		输入			INT16U		超时，单位mS，0死等
endchar		输入			INT8U		提示符

 返  回  值：ok(0)-成功
				 KB_CANCEL-按退出键了
				 notok(0xFF)-失败，超时
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U com1_gets2(INT8U *str_got, INT16U *rcv_len, INT16U timeout, INT8U endchar);



/*****************************************************************
函数原型：UART1Getch2
功能描述：通讯处理
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
timeout		输入			INT16U		超时，单位mS，0死等
stat		输出			INT8U*		状态  ok=成功   notok=超时

 返  回  值：读到的数
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U UART1Getch2(INT16U timeout, INT8U * stat);



/******************************************************************************
 函数名称：CleanCom1Buf
 功能描述：清空串口1缓存
 参数描述：
 参数名称： 输入/输出？ 类型		描述

 返  回  值：无

 作 	 者 ：许岩
 日 	 期：2005-04-11
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void CleanCom1Buf(void);



/*****************************************************************
函数原型：com1_gets
功能描述：串口1接收
参数描述：
参数名称：	输入/输出？	类型		描述
-----------		-----------	------	   	-------
str_got		输出			INT8U*		得到的字符串
timeout_start输入			INT16U		首字超时，单位mS，0死等
timeout		输入			INT16U		首字超时，单位mS，0死等
// endchar		输入			INT8U		提示符

 返  回  值：ok(0)-成功
				 KEY_CANCEL-按退出键了
				 notok(0xFF)-失败，超时
作      者	：许岩
日      期：2005-08-20
修改历史：
日期		修改人		修改描述
------		---------	-------------
*****************************************************************/
INT8U com1_gets(INT8U *str_got, INT16U *rcv_len, INT16U timeout_start, INT16U timeout);



/******************************************************************************
 函数名称：com1_send_str_protocol
 功能描述：封装协议包头和包尾,发送出去
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 str_to_send	输入			char *		要发送的字符串
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void com1_send_str_protocol(char * str_to_send);



/******************************************************************************
 函数名称：com1_send_str_protocol
 功能描述：封装协议包头和包尾,发送出去
 参数描述：
 参数名称： 输入/输出？ 类型		描述
 str_to_send	输入	INT8U *		要发送的字符串
 len				输入		INT16U		发送长度
				
 返  回  值：无
				   
 作 	 者 ：许岩
 日 	 期：2005-04-07
 修改历史：
		日期		修改人		修改描述
		------		---------	-------------
******************************************************************************/
void com1_send_hex_protocol(INT8U * str_to_send, INT16U len);






#endif

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/

