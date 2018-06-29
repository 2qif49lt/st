#ifndef __SERVICECONFIG_H__
#define __SERVICECONFIG_H__

#include <string>
#include <list>
#include <algorithm>


class CServiceConfig
{
public:
	CServiceConfig(void);
	~CServiceConfig(void);
public:
	//读取配置文件信息(各个程序自己去重新改写该函数,获取系统的配置信息)
	bool init_config();
	std::string getipbymode(std::string strmode);
	void stltrim(std::string& str)
	{
		str.erase(
			str.begin(),
			std::find_if(str.begin(),str.end(),std::not1(std::ptr_fun<int,int>(isspace))));
		str.erase(
			std::find_if(str.rbegin(),str.rend(),std::not1(std::ptr_fun<int,int>(isspace))).base(),
			str.end());
		//	transform(str.begin(),str.end(),str.begin(),tolower);
	}
	unsigned int readloglevel();
	int gethostiplist(std::list<std::string>& iplst);

public:
	std::string				m_strdumpfilename;	//dump文件名
	std::string				m_strsrvname;//服务的名字
	std::string				m_strsrvdisplayname;//服务显示的名字

	uint32_t 				m_uibuildtime;	//编译时间
	std::string 				m_strExcePath;				//进程路径

	std::string				m_strnetip;	//外网地址(可能和m_strsrvip不一样,如:当配置为all时,srvip为"inaddr_any",而netip则为外网或内网ip。)
	std::string				m_strSrvIP;//服务器的IP
	uint16_t				m_usSrvPort;//服务器的port

	uint32_t				m_nSrvTypeID;//服务器类型ID
	uint32_t				m_nloglevel;		//日志等级
	
	std::string				m_stragentip;			//agent ip
	uint16_t				m_usagentport;			//agent port
	uint32_t				m_uisendstatfrequency;	//统计数据发送频率,单位秒

	std::string				m_strdbip;			//db ip
	uint16_t				m_usdbport;			//db port
	std::string 				m_strdbname;
	std::string 				m_strdbuser;
	std::string 				m_strdbpwd; 	
};

#endif