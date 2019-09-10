#include "basedefine.h"

std::string getipbyint(unsigned int uiip)
{
	in_addr in={0};
	in.s_addr = uiip;
	return inet_ntoa(in);
}

unsigned int getipbystr( std::string strip )
{
	return inet_addr(strip.c_str());
}

unsigned int getticktime()
{
	unsigned int currentTime;
#ifdef _WIN32
	currentTime = GetTickCount();
#else
	struct timeval current;
	gettimeofday(&current, NULL);
	currentTime = current.tv_sec * 1000 + current.tv_usec/1000;
#endif
	return currentTime;
}

unsigned int getinterval(unsigned int uilast,unsigned int uinow)
{
	if( uinow < uilast )
		return 0xFFFFFFFF - uilast + uinow;
	else
		return uinow - uilast;
}


std::string gettimestr()
{
	time_t curtime = time(NULL);
	tm tim = *localtime(&curtime); 
	char strtimebuff[250] = {0};
	sprintf(strtimebuff,"%04d%02d%02d%02d%02d%02d",tim.tm_year + 1900,
		tim.tm_mon + 1,tim.tm_mday,tim.tm_hour,tim.tm_min,tim.tm_sec);
	return strtimebuff;
}
std::string gettimestrbyint(unsigned int uitime)
{
	time_t curtime = uitime;
	tm tim = *localtime(&curtime); 
	char strtimebuff[250] = {0};
	sprintf(strtimebuff,"%02d%02d%02d%02d",
		tim.tm_mon + 1,tim.tm_mday,tim.tm_hour,tim.tm_min);
	return strtimebuff;
}

std::string getorderid32() {
	time_t curtime = time(NULL);
	tm tim = *localtime(&curtime); 
	char strtimebuff[250] = {0};
	sprintf(strtimebuff,"ss%04d%02d%02d%02d%02d%02d%06d%010d",tim.tm_year + 1900,
		tim.tm_mon + 1,tim.tm_mday,tim.tm_hour,tim.tm_min,tim.tm_sec, rand(), rand());
	return strtimebuff;
}

std::string getorderid() {
	time_t curtime = time(NULL);
	tm tim = *localtime(&curtime); 
	char strtimebuff[250] = {0};
	sprintf(strtimebuff,"ss%04d%02d%02d%02d%02d%02d%06d",tim.tm_year + 1900,
		tim.tm_mon + 1,tim.tm_mday,tim.tm_hour,tim.tm_min,tim.tm_sec, rand());
	return strtimebuff;
}

int myaccess(const char* szpath,int iresver)
{
#ifdef WIN32
	return _taccess(szpath,0);
#else
	return access(szpath,F_OK);
#endif
}
int mymkdir(const char* szpath)
{
#ifdef WIN32
	return _tmkdir(szpath);
#else
	return mkdir(szpath,S_IRWXU | S_IRWXG | S_IRWXO);
#endif
}

char replace_slash(char ch)
{
	if(ch == '\\')
		ch = '/';
	return ch;
}

bool cleanurlfun(char i,char j)
{
	return (i==j && i == '/' );
}

std::string cleanurl(std::string strurl)
{
/*	class cleanurlfun() // 该版本g++不支持
	{
	public:
		bool operator() (char i,char j) const
		{
			return (i==j && i == '/' ); 
		}
	};*/
	std::transform(strurl.begin(),strurl.end(),strurl.begin(),replace_slash);
	std::string::iterator it = std::unique (strurl.begin(), strurl.end(), cleanurlfun);  
	strurl.resize( std::distance(strurl.begin(),it));
 	return strurl;
}
std::string getbinpath()
{
#ifdef _WIN32
	char szbuff[250] = {0};
	GetModuleFileName(NULL,szbuff,sizeof(szbuff));
	return szbuff;
#else
	char path[200] = { 0 };
	int rst = readlink("/proc/self/exe", path, sizeof(path));
	if (rst > 0 && rst < sizeof(path))
		path[rst] = 0;

	return path;
#endif
}
std::string getbinfolder()
{
	std::string strtmp = getbinpath();
	transform(strtmp.begin(),strtmp.end(),strtmp.begin(),replace_slash);
	size_t iindex = strtmp.rfind('/');
	if (iindex != std::string::npos)
		strtmp = strtmp.substr(0,iindex);
	return strtmp;
}

bool sendwarn(const char* szmsd,unsigned short ustype)
{
//	return true;
	return OMASDll_ReportWarning(szmsd,ustype) == 0;
}

bool sendstat(const char* pstat,unsigned int uilen,unsigned short ustype)
{
//	return true;
	return OMASDll_ReportInfo(pstat,uilen,ustype) == 0;
}
void millisleep(unsigned int uimillisecond)
{
#ifdef _WIN32
	Sleep(uimillisecond);
#else
	usleep(uimillisecond * 1000);
#endif
}

unsigned int cpunum()
{
	unsigned int uinum = 0;
#ifdef _WIN32
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	uinum = (unsigned int)SysInfo.dwNumberOfProcessors;
#else
	int iret = sysconf(_SC_NPROCESSORS_ONLN); 
	if(iret == -1)
		uinum = 4;
	else
		uinum = (unsigned int)iret;
#endif
	return uinum;
}

bool isnetip(unsigned int ip_addr_net_byte)
{
	if ((ip_addr_net_byte & 0xff) == 127)	// 整个127...段皆为回环IP
		return false;

	//检查3类地址
	unsigned int uihostbyte = ntohl(ip_addr_net_byte);
	if (
		(uihostbyte >= 0x0A000000 && uihostbyte <= 0x0AFFFFFF) ||	// 10.0.0.0 ~ 10.255.255.255；
		(uihostbyte >= 0xAC100000 && uihostbyte <= 0xAC1FFFFF) ||	// 172.16.0.0 ~ 172.31.255.255；
		(uihostbyte >= 0xC0A80000 && uihostbyte <= 0xC0A8FFFF)		// 192.168.0.0 ~ 192.168.255.255
		)
		return false;
	return true;
}

unsigned int BKDRHash(const char*str,int bucket)   
{   
    unsigned int seed=131 ;// 31 131 1313 13131 131313 etc..    
    unsigned int hash=0 ;       
    while(*str)   
    {   
        hash=hash*seed+(*str++);   
    }
    return(hash % bucket);   
}   
std::string gen_random(int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::stringstream ss;
    for (int i = 0; i < len; ++i) {
        ss << alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return ss.str();
}

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string& s, char delim) {
	std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}