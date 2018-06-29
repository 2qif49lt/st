#include "app.h"

// 在进程目录创建log目录.
static void make_logfile()
{
	char szlogdir[260]={0};
	sprintf(szlogdir,"%s/%s",g_strbinfolder.c_str(),"log");
	if(myaccess(szlogdir,0) == -1)
		mymkdir(szlogdir);

	strcat(szlogdir,"/");
	strcat(szlogdir,g_strappstarttime.c_str());

	if(myaccess(szlogdir,0) == -1)
		mymkdir(szlogdir);

	g_baselog.InitLog(szlogdir,"base.log",LOG_LV_NORMAL);
	g_threadlog.InitLog(szlogdir,"thread.log",LOG_LV_NORMAL);

	g_baselog.log(LOG_LV_INFO,"%s(%s) version: %s",
		g_srvcfg.m_strsrvname.c_str(),g_bdaemon ? "daemon" : "console",VERSION_STR);
}


// 每2分钟读取配置文件的日志等级
static void update_loglev()
{
	static unsigned int uilastupdateloglevtick = getticktime();
	if(getinterval(uilastupdateloglevtick,getticktime()) >= 2 * 60 * 1000)
	{
		uilastupdateloglevtick = getticktime();

		unsigned int nlev = g_srvcfg.readloglevel();

		if (nlev == 0)
		{
			g_baselog.log(LOG_LV_WARN,"config file is missing");
			return;
		}
		if (nlev != g_srvcfg.m_nloglevel)
		{
			g_baselog.log(LOG_LV_WARN,"log level change old level %d,now %d",
				g_srvcfg.m_nloglevel,
				nlev);
			g_baselog.SetWriteLevel(nlev);
			g_threadlog.SetWriteLevel(nlev);
			
			g_srvcfg.m_nloglevel = nlev;
		}
	}
}

//初始化系统参数
static void  init_para()
{
	g_baselog.SetWriteLevel(g_srvcfg.m_nloglevel);
	g_threadlog.SetWriteLevel(g_srvcfg.m_nloglevel);
}

void cb_omasdll(unsigned int uiCmd,void* pData,unsigned int uilen,unsigned int uiID)
{
	switch(uiCmd)
	{
		case 1:
		{
			const char* retData = "OK";
			OMASDll_SendCustomCmdRet(uiCmd, retData, (unsigned int)strlen(retData), uiID);

		}
		break;
		default:
		{
			const char* retData = "非法命令操作";
			OMASDll_SendCustomCmdRet(uiCmd, retData, (unsigned int)strlen(retData), uiID);
		}	
	}
}
void init_omasdll()
{
	InitOMASDll(g_srvcfg.m_nSrvTypeID,g_srvcfg.m_uibuildtime,VERSION_STR);
	OMASDll_RegCallbackFun(cb_omasdll);
}
void uninit_omaddll()
{
	ReleaseOMASDll();
}
void send_omasdll()
{
	static unsigned int uilast_send_stat_tick = getticktime();
	if (getinterval(uilast_send_stat_tick,getticktime()) > 60 * 1000)
	{
		// do here
		uilast_send_stat_tick = getticktime();
	}
}

bool initinstance()
{
	g_strbinfolder = getbinfolder();

	make_logfile();
	//获取配置信息
	if (!g_srvcfg.init_config())
	{
		g_baselog.log(LOG_LV_FAILD,"%s init_config fail", __FUNCTION__);
		return false;
	}

	init_para();
	init_omasdll();

	g_baselog.log(LOG_LV_FAILD,"initinstance done");
	return true;
}

int exitinstance()
{
	uninit_omaddll();

	g_baselog.log(LOG_LV_INFO,"%s stop over", __FUNCTION__);
	return 0;
}


int run()
{
	return g_udp_srv.listen_serve(udp_handler{}, g_srvcfg.m_usSrvPort,
			 g_srvcfg.m_strSrvIP);

	g_baselog.log(LOG_LV_INFO,"%s run over",__FUNCTION__);
	return 0;
}

