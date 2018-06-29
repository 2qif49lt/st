#include "srvconfig.h"
#include "inireader.h"
#include "basedefine.h"


CServiceConfig::CServiceConfig(void)
{
	m_strsrvname = "Qi Fan mobile game log Srv";//服务的名字
	m_strsrvdisplayname = "Qi Fan mobile game log Srv";//服务显示的名字
	m_strdumpfilename = "mblogcore";
}

CServiceConfig::~CServiceConfig(void)
{

}

//读取配置文件信息
bool CServiceConfig::init_config()
{
	m_uibuildtime = static_cast<uint32_t>(BUILD_TIME);
	m_strExcePath = getbinfolder();

	char szpath[260] = {};
	string strtmp;

	sprintf(szpath,"%s/config/config.ini",m_strExcePath.c_str());
	INIReader ini(szpath);

	if (ini.ParseError())
		return false;	
	
	strtmp = ini.Get("base","localip","all");
	m_strSrvIP = getipbymode(strtmp);
	if (strtmp == "all")
	{
		m_strnetip = getipbymode("auto");
	}
	else
		m_strnetip = m_strSrvIP;

	strtmp = ini.Get("base","netip","");
	if(strtmp != "")
		m_strnetip = strtmp;
	
	m_usSrvPort = (uint16_t)ini.GetInteger("base","port",6636);

	m_nSrvTypeID = static_cast<uint32_t>(ini.GetInteger("base","srvid",99999));
	m_nloglevel = (uint32_t)ini.GetInteger("base","loglevel",5);


	m_stragentip = ini.Get("agent","ip","127.0.0.1");			
	m_usagentport = (uint16_t)ini.GetInteger("agent","port",1312);		
	m_uisendstatfrequency = (uint32_t)ini.GetInteger("agent","frequncy",120);
	if (m_uisendstatfrequency == 0)
		m_uisendstatfrequency = 120;

	m_strdbip = ini.Get("db","ip","127.0.0.1");
	m_usdbport = (uint16_t)ini.GetInteger("db","port",3306);	
	m_strdbname = ini.Get("db","dbname","mblog");
	m_strdbuser = ini.Get("db","user","root");
	m_strdbpwd = ini.Get("db","passwd","123456");

	return true;
}
unsigned int CServiceConfig::readloglevel()
{
	char szpath[260] ={0};
	sprintf(szpath,"%s/config/config.ini",m_strExcePath.c_str());

	INIReader ini(szpath);
	if (ini.ParseError())
		return false;		

	return (uint32_t)ini.GetInteger("base","loglevel",5);
}
string CServiceConfig::getipbymode(std::string strmode)
{
	std::transform(strmode.begin(),strmode.end(),strmode.begin(),::tolower);
	if (strmode == "all")
	{
		return "INADDR_ANY";	
	}
	if (strmode == "auto")
	{
		std::list<string> iplst;
		int i = gethostiplist(iplst);
		if (i == 0)
		{
			return "INADDR_ANY";
		}

		return iplst.front();
	}
	return strmode;
}

int CServiceConfig::gethostiplist(std::list<std::string>& iplst) 
{ 
#ifdef WIN32
	WORD   wVersionRequested   =   MAKEWORD(2,   2); 
	WSADATA   wsaData; 
	if   (WSAStartup(wVersionRequested,   &wsaData)   !=   0)	return   0; 

	char hostname[260] = {0};

	gethostname(hostname, sizeof(hostname));// 获得本机主机名.

	hostent*   pHost=gethostbyname(hostname);

	in_addr   **ppAddr=(in_addr   **)pHost-> h_addr_list;

	in_addr   *pAddr;

	std::string strip;
	while(pAddr=*(ppAddr++))
	{
		strip = getipbyint(pAddr->s_addr);

		if (isnetip(pAddr->s_addr))
		{
			iplst.push_front(strip);
		}
		else
			iplst.push_back(strip);
	}

	WSACleanup(); 
#else
	struct ifaddrs *myaddrs, *ifa;
    void *in_addr;
    char buf[64];

    if(getifaddrs(&myaddrs) != 0)
       return 0;

    for (ifa = myaddrs; ifa != NULL; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == NULL)
            continue;
        if (!(ifa->ifa_flags & IFF_UP))
            continue;

        switch (ifa->ifa_addr->sa_family)
        {
            case AF_INET:
            {
                struct sockaddr_in *s4 = (struct sockaddr_in *)ifa->ifa_addr;
                in_addr = &s4->sin_addr;
                break;
            }

            case AF_INET6:
            {
                struct sockaddr_in6 *s6 = (struct sockaddr_in6 *)ifa->ifa_addr;
                in_addr = &s6->sin6_addr;
                break;
            }

            default:
                continue;
        }

        if (inet_ntop(ifa->ifa_addr->sa_family, in_addr, buf, sizeof(buf)))
        {
            printf("%s: %s\n", ifa->ifa_name, buf);
            iplst.push_back(buf);
        }
    }

    freeifaddrs(myaddrs);
#endif
	return   iplst.size(); 
} 
