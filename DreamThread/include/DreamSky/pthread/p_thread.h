#ifndef __P_THREAD_H__
#define __P_THREAD_H__

#include "p_mutex.h"
#include "p_semaphore.h"

DREAM_NAMESPACE_BEGIN

class DREAMSKY_API DPthread
{
public:
	using thread_func = void*(*)(void*);

	DPthread();
	DPthread(thread_func func, void* arg);
	virtual ~DPthread();

	void StartThread();
	void StopThread();
	void DetachThread();

protected:
	static void* pthread_func(void* p);
	virtual void ThreadFunc();

private:
	class ThreadData;

private:
	bool        m_bDetached;
	bool        m_bStarted;
	ThreadData* m_pData;
	void*       m_arg;
	thread_func m_func;
};

DREAM_NAMESPACE_END

#endif //!__P_THREAD_H__