#include "DreamSky/pthread/p_thread.h"
#include <pthread.h>

DREAM_NAMESPACE_BEGIN

class DPthread::ThreadData
{
public:
	pthread_t m_thread;
};

DPthread::DPthread() : 
	m_bDetached(false), m_bStarted(false),
	m_arg(this), m_func(pthread_func)
{
	m_pData = new ThreadData();
}

DPthread::DPthread(thread_func func, void* arg) :
	m_bDetached(false), m_bStarted(false),
	m_arg(arg), m_func(func)
{
	m_pData = new ThreadData();
}

DPthread::~DPthread()
{
	if (!m_bDetached)
	{
		StopThread();
	}

	if (m_pData)
	{
		delete m_pData;
		m_pData = nullptr;
	}
}

void DPthread::StartThread()
{
	if (!m_bStarted)
	{
		m_bStarted = true;
		pthread_t& thread = m_pData->m_thread;
		pthread_create(&thread, nullptr, m_func, m_arg);
	}	
}

void DPthread::StopThread()
{
	if (m_bStarted)
	{
		pthread_t& thread = m_pData->m_thread;
		pthread_join(thread, nullptr);
		m_bStarted = false;
	}
}

void DPthread::DetachThread()
{
	if (!m_bDetached)
	{
		m_bDetached = true;
		pthread_t& thread = m_pData->m_thread;
		pthread_create(&thread, nullptr, m_func, m_arg);
		pthread_detach(thread);
	}
}

void* DPthread::pthread_func(void* p)
{
	DPthread* dp = static_cast<DPthread*>(p);
	dp->ThreadFunc();
	return nullptr;
}

void DPthread::ThreadFunc()
{
	// 需要在继承类中进行具体实现
}

DREAM_NAMESPACE_END
