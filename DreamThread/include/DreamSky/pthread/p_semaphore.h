#ifndef __P_SEMAPHORE_H__
#define __P_SEMAPHORE_H__

#include <DreamSky/dream_define.h>

DREAM_NAMESPACE_BEGIN

class DREAMSKY_API DPsem
{
public:
	DPsem(bool bShared = false, unsigned int value = 0);
	~DPsem();

	void Wait();
	void Post();

private:
	d_handle m_pSem;
};

DREAM_NAMESPACE_END

#endif //!__P_SEMAPHORE_H__