#define ARP_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"
typedef struct    { /* arp报文的内容总长28字节*/
                                       
                 uint16 	HardwareType;  	    /*以太网为0x0001*/
                 uint16 	ProtocalType;   	/*ip 为0X0800*/
                 uint8 	HardwareLen;        /*=0X06*/
                 uint8 	ProtocalLen;        /*=0X04*/
                 uint16 	Operation;         	/*操作  0X0001为请求   0X0002为应答  */
                                           	/*0X0003为反向地址请求 0X0004为反向地址应答*/
                 uint8 	SourceMacId[6];   	/*源网卡地址*/
                 uint8 	SourceIp[4];       	/*源IP地址*/
                 uint8 	DestMacId[6];     	/*目的网卡地址*/
                 uint8 	DestId[4];         	/*目的IP地址*/
                } arp ;
uint8 ARP_REP_MAC[6]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
uint8 Ping_Ip_Address[4];
uint8 Ping_IP_TTL;
//uint8 My_Ip_Address[4]=MY_IP;
//uint8 My_Gateway_Ip_Address[4]=MY_GATEWAY;
//uint8 MY_IP_MARK[4]=IP_MARK_SETTING;
//struct a_t ARP_TERM[MAX_ARP_TERM];


//===========================================================
/**********************************************************************
**函数原型：    unsigned char Arp_Answer(unsigned char EX_RAM * ARP_REC_PTR)
**入口参数:		* ARP_REC_PTR	:接收到的ARP请求包的指针
**出口参数:		unsigned char
**返 回 值：	0		:本子网内的ARP缓冲区已经有MAC
**				1		:目标IP不是本机不作回应
**				2		:本子网内的，但ARP缓冲区还没有MAC
**				3		:不是本子网的ARP请求
**				4		:ARP缓冲区已经满
**说    明：	对ARP请求报文的应答:填充本地物理地址,将ARP操作改为回答.
************************************************************************/
uint8 Arp_Answer(uint8 * ARP_REC_PTR,uint8 num) 
{
struct _pkst TxdArp;
uint8 SEND_ARP_MAC[6];
uint8 i;
//如果目标IP地址是本机IP
if (((arp*)ARP_REC_PTR)->DestId[0]==NetPort[num].My_Ip[0])
if (((arp*)ARP_REC_PTR)->DestId[1]==NetPort[num].My_Ip[1])
if (((arp*)ARP_REC_PTR)->DestId[2]==NetPort[num].My_Ip[2])
if (((arp*)ARP_REC_PTR)->DestId[3]==NetPort[num].My_Ip[3])
	{										//表示是要解析本地IP的请求
	for(i=0;i<4;i++)						//复制对方IP地址，填充源地址
		{
        SEND_ARP_MAC[i]=((arp*)ARP_REC_PTR)->SourceIp[i];
		((arp*)ARP_REC_PTR)->SourceIp[i]=NetPort[num].My_Ip[i];
		((arp*)ARP_REC_PTR)->DestId[i]=SEND_ARP_MAC[i];
		}
	for(i=0;i<6;i++)
   		{									//复制对方物理地址或网关地址
        SEND_ARP_MAC[i]=((arp*)ARP_REC_PTR)->SourceMacId[i];
		((arp*)ARP_REC_PTR)->SourceMacId[i]=NetPort[num].My_Mac[i];
		((arp*)ARP_REC_PTR)->DestMacId[i]=SEND_ARP_MAC[i];
    	}
#ifdef Big_End
	((arp*)ARP_REC_PTR)->Operation=0x0002;	//表明数据帧为ARP应答
#endif
#ifdef Little_End
	((arp*)ARP_REC_PTR)->Operation=0x0200;	//表明数据帧为ARP应答
#endif
	TxdArp.STPTR=NULL;
	TxdArp.length=0x60;
	TxdArp.DAPTR=ARP_REC_PTR;
	Send_ethernet_Frame(&TxdArp,SEND_ARP_MAC,ARP_PACKED,num);//发送ARP应答帧
	//如果发送方属于本网段
		if((((arp*)ARP_REC_PTR)->SourceIp[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
		if((((arp*)ARP_REC_PTR)->SourceIp[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
		if((((arp*)ARP_REC_PTR)->SourceIp[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
		if((((arp*)ARP_REC_PTR)->SourceIp[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
		{
			//查找有否属于该IP的对应MAC表
			for(i=0;i<MAX_ARP_TERM;i++)
			{
				if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((arp*)ARP_REC_PTR)->SourceIp[2])
				if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((arp*)ARP_REC_PTR)->SourceIp[3])
				if(NetPort[num].ARP_TERM[i].TTL>0)
				{//有则刷新
					NetPort[num].ARP_TERM[i].TTL=100;
					return(0);
				}
			}
			//查找有否空的MAC表项
			for(i=0;i<MAX_ARP_TERM;i++)
			{
				if(NetPort[num].ARP_TERM[i].TTL==0)
				{//有则保存
					NetPort[num].ARP_TERM[i].IP_NUM[0]=((arp*)ARP_REC_PTR)->SourceIp[0];
					NetPort[num].ARP_TERM[i].IP_NUM[1]=((arp*)ARP_REC_PTR)->SourceIp[1];
					NetPort[num].ARP_TERM[i].IP_NUM[2]=((arp*)ARP_REC_PTR)->SourceIp[2];
					NetPort[num].ARP_TERM[i].IP_NUM[3]=((arp*)ARP_REC_PTR)->SourceIp[3];
					NetPort[num].ARP_TERM[i].MAC_NUM[0]=((arp*)ARP_REC_PTR)->SourceMacId[0];
					NetPort[num].ARP_TERM[i].MAC_NUM[1]=((arp*)ARP_REC_PTR)->SourceMacId[1];
					NetPort[num].ARP_TERM[i].MAC_NUM[2]=((arp*)ARP_REC_PTR)->SourceMacId[2];
					NetPort[num].ARP_TERM[i].MAC_NUM[3]=((arp*)ARP_REC_PTR)->SourceMacId[3];
					NetPort[num].ARP_TERM[i].MAC_NUM[4]=((arp*)ARP_REC_PTR)->SourceMacId[4];
					NetPort[num].ARP_TERM[i].MAC_NUM[5]=((arp*)ARP_REC_PTR)->SourceMacId[5];
					NetPort[num].ARP_TERM[i].TTL=100;
					return(2);
				}
			}//FOR
			//MAC表已经满
			return(4);
		}//IF ARP
		//不属于同一网段的
		return (3);
   	}
	//目标IP不是本机
	return (1);
}
uint8 REC_ARP_REQ(uint8 * ARP_REC_REQ_PTR,uint8 num)  
{
uint8 i;
//======================================================================
if(((arp*)ARP_REC_REQ_PTR)->SourceIp[0]==NetPort[num].My_Gateway[0])
if(((arp*)ARP_REC_REQ_PTR)->SourceIp[1]==NetPort[num].My_Gateway[1])
if(((arp*)ARP_REC_REQ_PTR)->SourceIp[2]==NetPort[num].My_Gateway[2])
if(((arp*)ARP_REC_REQ_PTR)->SourceIp[3]==NetPort[num].My_Gateway[3])
	{  				//表示是网关对ARP请求的回答.
   	for (i=0;i<6;i++)
		{
		NetPort[num].My_Gateway_Mac[i]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[i];
		}
   	NetPort[num].Gateway_IP_TTL=100;	//表示网关地址已得到解析
	//return(3);
	}
	//如果发送方属于本网段
if((((arp*)ARP_REC_REQ_PTR)->SourceIp[0]&NetPort[num].My_Ip_Mark[0])==(NetPort[num].My_Ip[0]&NetPort[num].My_Ip_Mark[0]))
if((((arp*)ARP_REC_REQ_PTR)->SourceIp[1]&NetPort[num].My_Ip_Mark[1])==(NetPort[num].My_Ip[1]&NetPort[num].My_Ip_Mark[1]))
if((((arp*)ARP_REC_REQ_PTR)->SourceIp[2]&NetPort[num].My_Ip_Mark[2])==(NetPort[num].My_Ip[2]&NetPort[num].My_Ip_Mark[2]))
if((((arp*)ARP_REC_REQ_PTR)->SourceIp[3]&NetPort[num].My_Ip_Mark[3])==(NetPort[num].My_Ip[3]&NetPort[num].My_Ip_Mark[3]))
{//查找有否属于该IP的对应MAC表
	for(i=0;i<MAX_ARP_TERM;i++)
	{
		if(NetPort[num].ARP_TERM[i].IP_NUM[2]==((arp*)ARP_REC_REQ_PTR)->SourceIp[2])
		if(NetPort[num].ARP_TERM[i].IP_NUM[3]==((arp*)ARP_REC_REQ_PTR)->SourceIp[3])
		if(NetPort[num].ARP_TERM[i].TTL>0)
		{//有则刷新
			NetPort[num].ARP_TERM[i].TTL=100;
			return(0);
		}
	}
	//查找有否空的MAC表项
	for(i=0;i<MAX_ARP_TERM;i++)
	{
		if(NetPort[num].ARP_TERM[i].TTL==0)
		{//有则保存
			NetPort[num].ARP_TERM[i].IP_NUM[0]=((arp*)ARP_REC_REQ_PTR)->SourceIp[0];
			NetPort[num].ARP_TERM[i].IP_NUM[1]=((arp*)ARP_REC_REQ_PTR)->SourceIp[1];
			NetPort[num].ARP_TERM[i].IP_NUM[2]=((arp*)ARP_REC_REQ_PTR)->SourceIp[2];
			NetPort[num].ARP_TERM[i].IP_NUM[3]=((arp*)ARP_REC_REQ_PTR)->SourceIp[3];
			NetPort[num].ARP_TERM[i].MAC_NUM[0]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[0];
			NetPort[num].ARP_TERM[i].MAC_NUM[1]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[1];
			NetPort[num].ARP_TERM[i].MAC_NUM[2]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[2];
			NetPort[num].ARP_TERM[i].MAC_NUM[3]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[3];
			NetPort[num].ARP_TERM[i].MAC_NUM[4]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[4];
			NetPort[num].ARP_TERM[i].MAC_NUM[5]=((arp*)ARP_REC_REQ_PTR)->SourceMacId[5];
			NetPort[num].ARP_TERM[i].TTL=100;
			return(2);
		}
	}
	//MAC表已经满
	return(4);
}
//对方IP即不是本网段也不是GATEWAY
return(1);
}
//===========================================================================
//=======================================================================
/**********************************************************************
**函数原型：    void PROCESS_ARP_REC(unsigned char EX_RAM * ARP_PTR)
**入口参数:		* ARP_PTR	:接收到的ARP包的指针
**出口参数:		无
**返 回 值：	无
**说    明：	对ARP包进行分类处理
************************************************************************/
void PROCESS_ARP_REC(uint8 * ARP_PTR,uint8 num)  
{
//	EX_RAM PKST ARP_PACKED;
#ifdef Big_End
if(((arp*)ARP_PTR)->Operation==0X0001)
#endif
#ifdef Little_End
if(((arp*)ARP_PTR)->Operation==0X0100)
#endif
	{
		Arp_Answer(ARP_PTR,num);
	}
#ifdef Big_End
else if(((arp*)ARP_PTR)->Operation==0X0002)
#endif
#ifdef Little_End
else if(((arp*)ARP_PTR)->Operation==0X0200)
#endif
	{
		REC_ARP_REQ(ARP_PTR,num);
	}
	//可添加REARP操作。

}
/**********************************************************************
**函数原型:�    void        Arp_Request(unsigned char * ip_address)
**入口参数:�	ip_address: 要解析的IP地址
**出口参数:		无
**返 回 值:�	无
**说    明:�	请求对指定的IP地址进行解析,获取其物理地址
************************************************************************/
void Arp_Request(uint8 * ip_address,uint8 num)
{
struct _pkst TxdArpReq;
uint8 ARPREQ[46];
uint8 i;
for(i=0;i<6;i++)			  					//复制对方网卡地址或网关地址
	{											
	((arp*)ARPREQ)->SourceMacId[i]=NetPort[num].My_Mac[i];
	((arp*)ARPREQ)->DestMacId[i]=0x00;	//arp报文的目的物理地址填为0,由arp回答报文										//负责填充
    }
for(i=0;i<4;i++)								
	{
	((arp*)ARPREQ)->SourceIp[i]=NetPort[num].My_Ip[i];//填充源IP地址
	((arp*)ARPREQ)->DestId[i]=*ip_address;//填充目的IP地址
	ip_address++;
	}
#ifdef Big_End	
((arp*)ARPREQ)->HardwareType=0x0001;		//硬件类型：0x0001,以太网类型
((arp*)ARPREQ)->ProtocalType=0x0800;		//协议类型：0x0800,对应IPv4
((arp*)ARPREQ)->Operation=0x0001;			//操作类型：ARP请求
#endif

#ifdef Little_End
((arp*)ARPREQ)->HardwareType=0x0100;		//硬件类型：0x0001,以太网类型
((arp*)ARPREQ)->ProtocalType=0x0008;		//协议类型：0x0800,对应IPv4
((arp*)ARPREQ)->Operation=0x0100;			//操作类型：ARP请求
#endif	
((arp*)ARPREQ)->HardwareLen=0x06;			//硬件长度：即物理地址长度，单位字节
((arp*)ARPREQ)->ProtocalLen=0x04;			//协议长度：即逻辑地址长度，单位字节

/*******************************************************
 *注意:	
 *arp报文段的长度为28字节,而以太网数据包的最小单元为60字节,所以在发送arp报文时需要对
 *以太网报文段进行填充,以满足最小长度要求.
 *arp分组的封装格式:
 *发送报文:               |     以太网首部(共14字节)    |    arp请求分组(28字节)  | 填充数据 |       
 *接收报文: | RTL8019首部 |     以太网首部(共14字节)    |    arp回答分组(28字节)  | 填充数据 |
 *          |--> 4字节 <--|------->   14字节    <-------|-------> 28字节 <--------|->18字节<-|                   
 *          |--------------------------->0x00～0x<2e<-----------------------------|                                 
 *实际上4字节的8019首部在发送时是不起作用的,真正发送的数据从以太网首部起.所以在填充数据时,是从0x2e开始的.
 *因为程序中将发送数据报数据结构与接收数据报数据结构定义于公用体中,方便操作.关于数据结构定义
 *可以参看arp.h,以及相应的文档说明.
 ******************************************************/
//for(i=28;i<29;i++)				//不需要46		//当数据长度<60字节时，需要补足60字节数据
//	{
	ARPREQ[28]=0x00;			//填充数据为0x00
//	}
//启动发送数据,发送的是一个arp请求.
TxdArpReq.STPTR=NULL;
TxdArpReq.length=46;
TxdArpReq.DAPTR=ARPREQ;
Send_ethernet_Frame(&TxdArpReq,ARP_REP_MAC,ARP_PACKED,num);
}
/**********************************************************************
**函数原型:�    void	Initial_arp()
**入口参数:�	无
**出口参数:		无
**返 回 值:�	无
**说    明:�	ARP初始化函数
************************************************************************/
void	Initial_arp()  
{
uint8 i,num;
	Ping_IP_TTL=0;
	for(num=0;num<MAX_NET_PORT;num++)
	{
		NetPort[num].Gateway_IP_TTL=0;
		for(i=0;i<MAX_ARP_TERM;i++)
		{
			NetPort[num].ARP_TERM[i].TTL=0;
		}
		Arp_Request(NetPort[num].My_Gateway,num);
	}
}

