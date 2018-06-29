#ifndef UTIL_FUN_H_
#define UTIL_FUN_H_

#include <memory>
#include <utility>
#include <string>
#include <sstream>

#define uniform() (rand()/(RAND_MAX + 1.0))


extern std::string getipbyint(unsigned int uiip);

// getipbystr 把"127.0.0.1" 转为 16777343
extern unsigned int getipbystr( std::string strip );

// getticktime 获取tick点
extern unsigned int getticktime();

// getinterval 得到上次uilast 和这次 uinow tick点的时间差
extern unsigned int getinterval(unsigned int uilast,unsigned int uinow);

// gettimestr 获取“20010101235959”格式的字符串
extern std::string gettimestr();
extern std::string gettimestrbyint(unsigned int uitime);
extern std::string getorderid32();
extern std::string getorderid();

extern int myaccess(const char* szpath,int iresver);
extern int mymkdir(const char* szpath);

// 整理路径地址,比如将 /home//one\\two 处理为 /home/one/two
extern std::string cleanurl(std::string strurl);

// 获取当前可执行文件所在全路径
extern std::string getbinpath();

// 获取当前可执行文件的所在目录,结尾不包含斜杠
extern std::string getbinfolder();


extern void millisleep(unsigned int uimillisecond);

extern unsigned int cpunum();

extern bool isnetip(unsigned int ip_addr_net_byte);


extern bool sendwarn(const char* szmsd,unsigned short ustype);
extern bool sendstat(const char* pstat,unsigned int uilen,unsigned short ustype);

#define NUM_HASH_BUCKET  249997   // hash桶默认大小
extern unsigned int BKDRHash(const char*str,int bucket = NUM_HASH_BUCKET);

extern std::string gen_random(int len);

extern std::vector<std::string> split(const std::string& s, char delim);

#endif  // UTIL_FUN_H_