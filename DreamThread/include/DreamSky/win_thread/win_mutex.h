#ifndef __WIN_MUTEX_H__
#define __WIN_MUTEX_H__

#include <DreamSky/dream_define.h>

DREAM_NAMESPACE_BEGIN

#if defined(_WIN32) || defined(_WINDLL)

class DREAMSKY_API DWinCs
{
public:
	DWinCs();
	~DWinCs();

	void Lock();
	void Unlock();

private:
	class CsData;
	CsData* m_pData;
};

class DREAMSKY_API DWinMutex
{
public:
	DWinMutex(const char* name = nullptr);
	~DWinMutex();

	void Lock();
	void Unlock();

private:
	d_handle m_mutex;
};

#endif

DREAM_NAMESPACE_END

#endif //!__WIN_MUTEX_H__
