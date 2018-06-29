#ifndef LOCKX_SYSTEM_XHB_H_
#define LOCKX_SYSTEM_XHB_H_
/************************************************************************/
/* 
临界区 锁
使用方式
criticalsectionx_ m_lockobject;
{
lockx_<criticalsectionx_> lock(&m_lockobject);
}

falselockx_ falseobject;
{
lockx_<falselockx_> lock(&falseobject);
}
*/
/************************************************************************/
#ifdef _WIN32
#include "Windows.h"
#else
#include <pthread.h>
#endif
namespace xhb
{
	
	//假锁
	class falselockx_
	{
	public:
		void Lock() {}
		void Unlock(){}
	};

	class mlockx_
	{
	private:
#ifdef _WIN32
		CRITICAL_SECTION m_syslock;
#else
		pthread_mutex_t m_syslock;
#endif

	public:
		mlockx_() 
		{
#ifdef _WIN32
			::InitializeCriticalSection(&m_syslock);
#else
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);
			pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
			pthread_mutex_init(&m_syslock,&attr);
#endif
		}
		~mlockx_() 
		{
#ifdef _WIN32
			::DeleteCriticalSection(&m_syslock);
#else
			pthread_mutex_destroy(&m_syslock);
#endif
		}

	public:
		void Lock() 
		{
#ifdef _WIN32
			::EnterCriticalSection(&m_syslock);
#else
			pthread_mutex_lock(&m_syslock);
#endif
		}

		void Unlock() 
		{
#ifdef _WIN32
			::LeaveCriticalSection(&m_syslock);
#else
			pthread_mutex_unlock(&m_syslock);
#endif
		}
		bool trylock()
		{
#ifdef _WIN32
			return false;
	//		return TryEnterCriticalSection(&m_syslock) != 0;
#else
			 return pthread_mutex_trylock(&m_syslock) == 0;
#endif
		}
		void* operator()()
		{
			return (void*)&m_syslock ;
		}
	};
	template<typename l_>
	class lockx_
	{
	public:
		lockx_(l_* plockobject)
		{
			m_plock = plockobject;
			m_plock->Lock();
		}
		~lockx_()
		{
			m_plock->Unlock();
		}
	private:
		lockx_(){};
		l_* m_plock;
	};
	typedef lockx_<mlockx_>  _lockx;
}
#endif	//LOCKX_SYSTEM_XHB_H_