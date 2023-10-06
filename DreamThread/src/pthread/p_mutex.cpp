#include "DreamSky/pthread/p_mutex.h"
#include <pthread.h>

DREAM_NAMESPACE_BEGIN

DPmutex::DPmutex()
{
	pthread_mutex_t* mutex = new pthread_mutex_t;
	pthread_mutex_init(mutex, nullptr);
	m_pMutex = mutex;
}

DPmutex::~DPmutex()
{
	if (m_pMutex)
	{
		pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(m_pMutex);
		pthread_mutex_destroy(mutex);
		delete m_pMutex;
		m_pMutex = nullptr;
	}
}

void DPmutex::Lock()
{
	pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(m_pMutex);
	pthread_mutex_lock(mutex);
}

void DPmutex::Unlock()
{
	pthread_mutex_t* mutex = static_cast<pthread_mutex_t*>(m_pMutex);
	pthread_mutex_unlock(mutex);
}

DREAM_NAMESPACE_END
