#ifndef __WIN_SEMAPHORE_H__
#define __WIN_SEMAPHORE_H__

#include <DreamSky/dream_define.h>

DREAM_NAMESPACE_BEGIN

#if defined(_WIN32) || defined(_WINDLL)

class DREAMSKY_API DWinSem
{
public:
	DWinSem(const char* name = nullptr, long value = 0, long max_count = 1);
	~DWinSem();

	void Wait();
	void Post(long release_count = 1);

private:
	d_handle m_sem;
};

class DREAMSKY_API DWinEvent
{
public:
	DWinEvent(bool manual = false, bool init = false);
	~DWinEvent();

	void Set();
	void Reset();
	void Pulse();
	void Wait();

private:
	d_handle m_event;
};

#endif

DREAM_NAMESPACE_END

#endif //!__WIN_SEMAPHORE_H__