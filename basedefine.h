#ifndef __BASEDEFINE_H__
#define __BASEDEFINE_H__
#pragma warning(disable:4200)
#pragma warning(disable:4996)
#pragma warning(disable:4099)

#include <stdint.h>

#include "common.h"
#include "errcode.h"

// 网络协议头文件

// 内部通讯结构体,用于跨线程传递消息.
#include "message.h"	


// 系统,crt头文件
#ifdef _WIN32
	#include <WinSock2.h>
	#include <windows.h>
#else
	#include <stdio.h>
	#include <string.h>
	#include <sys/types.h>
	#include <unistd.h>
	#include <sys/time.h>
	#include <sys/socket.h>
	#include <ifaddrs.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <net/if.h>
	#include <pthread.h>
	#include <sys/wait.h>
    #include <sys/resource.h>
    #include <sys/stat.h>
#endif
#include <time.h>

// stl
#include <cstdint>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <mutex>
#include <chrono>
#include <map>
#include <set>
#include <vector>
#include <cmath> // abs
#include <memory>
#include <exception>

#include "omas/omasdll_interface.h"



// 宏定义
#define VERSION_STR							"1.0.0" // 版本号


// 全局函数
#include "utilfun.h"

#include "logx.h"
using namespace xhb;


#include "srvconfig.h"	// 配置文件
#include "udp_server.h"
#include "udp_handler.h"

// 全局变量

extern int 									g_argc;
extern char**								g_argv;
extern bool 									g_bdaemon;

extern std::string 							g_strappstarttime; // 程序启动时间
extern std::string							g_strbinfolder; // 本程序文件目录,结尾不包含斜杠
extern CServiceConfig						g_srvcfg; // 服务器的配置信息
extern SrvCalculateInfo    					g_SrvCalculateInfo; // 服务器端的统计信息

extern logx<mlockx_>							g_baselog; // 多线程日志
extern logx<mlockx_>							g_threadlog; // 多线程日志

extern udp_server 							g_udp_srv;
#endif