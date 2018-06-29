#include "basedefine.h"

// 全局变量
int							g_argc = 0;
char**						g_argv = NULL;
bool 						g_bdaemon = true;

std::string 						g_strappstarttime = gettimestr();
std::string						g_strbinfolder; 
CServiceConfig				g_srvcfg; // 服务器的配置信息
SrvCalculateInfo			g_SrvCalculateInfo={0};//服务器端的统计信息

logx<mlockx_>				g_baselog;	//多线程日志
logx<mlockx_>				g_threadlog;	//多线程日志

udp_server 					g_udp_srv{};