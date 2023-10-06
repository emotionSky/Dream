#include "DreamSky/pthread/p_semaphore.h"
#include <semaphore.h>

DREAM_NAMESPACE_BEGIN

DPsem::DPsem(bool bShared, unsigned int value)
{
	sem_t* sem = new sem_t;
	int shared = bShared ? 1 : 0;
	sem_init(sem, shared, value);
	m_pSem = sem;
}

DPsem::~DPsem()
{
	if (m_pSem)
	{
		sem_t* sem = static_cast<sem_t*>(m_pSem);
		sem_destroy(sem);
		delete m_pSem;
		m_pSem = nullptr;
	}
}

void DPsem::Wait()
{
	sem_t* sem = static_cast<sem_t*>(m_pSem);
	sem_wait(sem);
}

void DPsem::Post()
{
	sem_t* sem = static_cast<sem_t*>(m_pSem);
	sem_post(sem);
}

DREAM_NAMESPACE_END
