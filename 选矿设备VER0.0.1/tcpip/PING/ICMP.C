
#define ICMP_GLOBALS
#include "config.h"
#include "../include/cfg_net.h"

/**********************************************************************
**函数原型：    void 	CreateIcmpCrc( )
**入口参数:�	无
**出口参数:		无
**返 回 值：	crc ：  16位的校验和        
**说    明：	产生ICMP数据段的校验和 
************************************************************************/
uint16  CreateIcmpCrc(union icmp_rc icmppk) 
{
uint8 i;
union  w  CrcTemp;
CrcTemp.dwords=0;
for(i=0;i<20;i++)				//用于查询的ICMP报文共40字节
	{
	CrcTemp.dwords = CrcTemp.dwords + icmppk.words.wordbuf[i];
	}
while(CrcTemp.words.high>0) 
	{
    CrcTemp.dwords = (uint32)(CrcTemp.words.high+CrcTemp.words.low);
    }
CrcTemp.words.low = 0xffff - CrcTemp.words.low;
return(CrcTemp.words.low);
}
uint16  CreateIcmpCrc1(union icmp_rc icmppk) 
{
uint8 i;
union  w  CrcTemp;
CrcTemp.dwords=0;
for(i=0;i<18;i++)				//用于查询的ICMP报文共40字节
	{
	CrcTemp.dwords = CrcTemp.dwords + icmppk.words.wordbuf[i];
	}
while(CrcTemp.words.high>0) 
	{
    CrcTemp.dwords = (uint32)(CrcTemp.words.high+CrcTemp.words.low);
    }
CrcTemp.words.low = 0xffff - CrcTemp.words.low;
return(CrcTemp.words.low);
}
/**********************************************************************
**函数原型：    void icmp_process(unsigned char  * ICMPSENDPTR)
**入口参数:�	* ICMPSENDPTR	:	数据指针
**出口参数:		无
**返 回 值：	无    
**说    明：	处理ICMP包 
************************************************************************/
void icmp_process(uint8  * ICMPSENDPTR,uint8 num)  
{
uint8 i,temp;
struct _pkst SendIcmpData;  
	if(((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.type==0x08)//请求！！
	{
		((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.type=0;//回复
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.ttl--;
		for (i = 0; i < 4; i++)//IP翻转
    	{
        	temp = ((icmppro*)ICMPSENDPTR)->ipf.e_ip.SourceIp[i];
        	((icmppro*)ICMPSENDPTR)->ipf.e_ip.SourceIp[i] = ((icmppro*)ICMPSENDPTR)->ipf.e_ip.DestId[i];
        	((icmppro*)ICMPSENDPTR)->ipf.e_ip.DestId[i] = temp;
    	}
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc=0;
		OS_ENTER_CRITICAL();
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc=CreateIpHeadCrc(ICMPSENDPTR);//(((icmppro*)ICMPSENDPTR)->ipf);
#ifdef Little_End
		((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc=swap_int16(((icmppro*)ICMPSENDPTR)->ipf.e_ip.Crc);
#endif
		OS_EXIT_CRITICAL();
		((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.Crc=0;
		((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.Crc=CreateIcmpCrc(((icmppro*)ICMPSENDPTR)->icmpf);
		SendIcmpData.length=((icmppro*)ICMPSENDPTR)->ipf.e_ip.TotalLen;
#ifdef Little_End
		SendIcmpData.length=swap_int16(SendIcmpData.length);
#endif
		SendIcmpData.STPTR=NULL;
		SendIcmpData.DAPTR=ICMPSENDPTR;
		OSSemPend(SendFlag,5,&temp);
		if(temp==OS_NO_ERR)
		{
			Send_Ip_To_LLC(&SendIcmpData,((icmppro*)ICMPSENDPTR)->ipf.e_ip.DestId,num);
			OSSemPost(SendFlag);
		}
	}
	else if(((icmppro*)ICMPSENDPTR)->icmpf.e_icmp.type==0x00)//回复
	{
		i=i;
	}

}
void Icmp_Send(uint8 type,uint8 option,uint8 *so_ip,uint8 *de_ip, uint8 * REC_FRAME )
{
struct _pkst SendPingData;
union icmp_rc IcmpPacked;
//static uint16 temp_findex=0x2345;
	OS_ENTER_CRITICAL();
	memcpy (&(IcmpPacked.e_icmp.icmpdata), REC_FRAME, 28);
	OS_EXIT_CRITICAL();
	
	IcmpPacked.e_icmp.type=type;
	IcmpPacked.e_icmp.option=option;
	IcmpPacked.e_icmp.Crc=0;
	IcmpPacked.e_icmp.id=0;
	IcmpPacked.e_icmp.seq=0;
	IcmpPacked.e_icmp.Crc=CreateIcmpCrc1(IcmpPacked);
	SendPingData.length=36;
	SendPingData.STPTR=NULL;
	SendPingData.DAPTR=( uint8 * )&IcmpPacked;
	Send_Ip_Frame	(
			&SendPingData,
			de_ip,
			so_ip,
			1
		);
}
void Ping_Precess(uint8 * de_ip,uint8 * so_ip) 
{
struct _pkst SendPingData;
union icmp_rc IcmpPacked;
static uint16 temp_findex=0x2345;
IcmpPacked.e_icmp.type=0x08;
IcmpPacked.e_icmp.option=0;
IcmpPacked.e_icmp.Crc=0;
#ifdef Big_End
IcmpPacked.e_icmp.id=0x0300;
#endif
#ifdef Little_End
IcmpPacked.e_icmp.id=0x0003;
#endif
IcmpPacked.e_icmp.seq=temp_findex++;
IcmpPacked.e_icmp.Crc=CreateIcmpCrc(IcmpPacked);
SendPingData.length=40;
SendPingData.STPTR=NULL;
SendPingData.DAPTR=( uint8 * )&IcmpPacked;
Send_Ip_Frame	(
			&SendPingData,
			de_ip,
			so_ip,
			1
		);
}
