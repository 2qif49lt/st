#include "app.h"

static void onquit()
{
	g_udp_srv.shutdown();
}
static void sig_toquit(int signo)
{
	g_baselog.log(LOG_LV_WARN,"%s signal: %d",__FUNCTION__,signo);
	onquit();
}

bool initapp(int argc,char** argv)
{
	// 设置模式(参数-c为控制台)
	if(argc > 1)
	{
		for (int i = 1; i < argc; ++i)
		{
			if(0 == strcmp(argv[i],"-c"))
			{
				g_bdaemon = false;
				break;
			}
		}
	}
	printf("%s(%s) version: %s\n",g_srvcfg.m_strsrvname.c_str(),g_bdaemon ? "daemon" : "console",VERSION_STR);
	
//	g_bdaemon = false;
		
	if(g_bdaemon == true)
	{
		if (0 != daemon(0,0))
			return false;
	}

	// 设置资源
	struct rlimit rlim;
	struct rlimit rlim_new;
	int ret = 0;
	if (getrlimit(RLIMIT_NOFILE, &rlim)==0)
	{
		if(rlim.rlim_cur < 655350)
		{
			rlim_new.rlim_cur = rlim_new.rlim_max = 655350;
			if (setrlimit(RLIMIT_NOFILE, &rlim_new)!=0)
			{
				rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
				ret = setrlimit(RLIMIT_NOFILE, &rlim_new);
			}
		}

	}
	if (getrlimit(RLIMIT_CORE, &rlim_new) == 0) {
		rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
		setrlimit(RLIMIT_CORE, &rlim_new);
	}

	// 设置信号处理
	signal(SIGCHLD,SIG_IGN);
	signal(SIGINT,sig_toquit);
	signal(SIGUSR1,sig_toquit);
	signal(SIGTERM,sig_toquit);

	srand(time(NULL));
	// pidfile 忽略.
	chdir(getbinfolder().c_str());
	return true;
}

int main(int argc,char** argv)
{
	if(!initapp(argc,argv))
		return 1;

	g_argc = argc;
	g_argv = argv;

	if(!initinstance())
	{
		exitinstance();
		return 2;
	}
	
	int iret = run();
	g_baselog.log(LOG_LV_INFO,"%s run exit,return: %d",__FUNCTION__,iret);
	
	exitinstance();

	return 0;
}
