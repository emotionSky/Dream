#ifndef __P_MUTEX_H__
#define __P_MUTEX_H__

#include <DreamSky/dream_define.h>

DREAM_NAMESPACE_BEGIN

class DREAMSKY_API DPmutex
{
public:
	DPmutex();
	~DPmutex();

	DPmutex& operator=(const DPmutex&) = delete;

	void Lock();
	void Unlock();

private:
	d_handle m_pMutex;
};

DREAM_NAMESPACE_END

#endif //!__P_MUTEX_H__