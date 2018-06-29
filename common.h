
// 该文件为系统命令码分段,公有的c++格式定义等.
#pragma  once
#include <stdint.h>

#pragma pack(push)
#pragma pack(1)

//基础范围命令ID段划分

#define CAT_BaseId									0x0
#define CAT_MaxId									0x7ffffff 

#define MAX_LEN_UDPSOCKETBUFFER						1024 //udp报文数据的最大长度
#define MAX_LEN_TCPSOCKETBUFFER						100*1024 //tcp报文数据的最大长度


//基础报文头
struct CmdHeader 
{
	uint32_t		uilen;		// 数据包的字节数（含msghead）
	uint32_t 		uicmd;
	uint32_t 		uires;
};
typedef struct CmdHeader* PCmdHeader;

#define CMDHEADSIZE		(sizeof(CmdHeader))


#ifndef _WIN32
	#ifndef MAX_PATH
		#define MAX_PATH 260
	#endif
#endif


#pragma pack(pop)
