#ifndef _INSIDE_MESSAGE_H_
#define _INSIDE_MESSAGE_H_

#include "common.h"

#pragma pack(push)
#pragma pack(1)

#define InCmd_Net_Disconncect				(CAT_MaxId + 1000)	//连接断开命令码
#define InCmd_Net_ConnectPeer				(CAT_MaxId + 1001) //连接上远程服务器

//服务器端统计信息
typedef struct tagSrvCalculateInfo
{
	uint32_t			ulMsgTransNum;		// 发送的消息条数
	uint32_t			ulDataSize;			// 发送的消息字节数
	uint32_t			ulUserNum;			// 当前总人数
	uint32_t			ulUserHMNum;		// 网络连接映射用户人数
	uint32_t			ulGroupLimit;		// 当前所有群的上限
	uint32_t			ulGroupNum;			// 当前群数目
}SrvCalculateInfo,*PSrvCalculateInfo,**PPSrvCalculateInfo;

#pragma pack(pop)
#endif	//_INSIDE_MESSAGE_H_