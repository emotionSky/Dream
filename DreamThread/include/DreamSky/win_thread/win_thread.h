/*****************************************************************//**
 * \file   win_thread.h
 * \brief  优先使用 DWinThreadEx
 * 
 * \author 凌佚 emotion_sky
 * \date   October 2023
 *********************************************************************/

#ifndef __WIN_THREAD_H__
#define __WIN_THREAD_H__

#include "win_mutex.h"
#include "win_semaphore.h"
#include <DreamSky/string/dstring.h>
#include <functional>

DREAM_NAMESPACE_BEGIN

#if defined(_WIN32) || defined(_WINDLL)

class DREAMSKY_API DWinThread
{
public:
	using thread_func = std::function<void(void)>;

public:
	DWinThread();
	~DWinThread();
	bool IsCreated() const { return m_bCreated; }
	bool IsRunning() const { return m_bRunning; }
	bool CreateOneThread(thread_func func, bool is_run = true);
	bool SuspendThread();
	bool RunThread();
	bool WaitThread(int timeout_ms = -1);
	void CloseThread();
	const dstring& GetErrorString() const { return m_errStr; }

private:
	static unsigned long ThreadFunc(void* p);
	void _GetErrorString();

private:
	bool          m_bCreated;
	bool          m_bRunning;
	dstring       m_errStr;
	unsigned long m_threadID;
	unsigned long m_suspendCnt;
	d_handle      m_threadHdl;
	thread_func   m_threadFunc;
};

class DREAMSKY_API DWinThreadEx
{
public:
	using thread_func = unsigned int(*)(void*);

	DWinThreadEx();
	DWinThreadEx(thread_func func, void* arg);
	virtual ~DWinThreadEx();

	bool StartThread();
	bool StopThread(int timeout_ms = -1);
	bool DetachThread();
	
protected:
	static unsigned int dwin_thread_func(void* p);
	virtual void ThreadFunc();

private:
	friend class DWinThreadExManager;
	bool         m_bDetached;
	bool         m_bStarted;
	unsigned int m_threadID;
	void*        m_arg;
	thread_func  m_func;
	d_handle     m_threadHdl;
};

class DREAMSKY_API DWinThreadExManager
{
public:
	DWinThreadExManager();
	~DWinThreadExManager();

	bool AllocNewThread(DWinThreadEx::thread_func func, void* arg);
	void WaitThreads(bool wait_all = true, int timeout_ms = -1);

private:
	class ManagerData;
	ManagerData* m_pData;
};

#endif

DREAM_NAMESPACE_END

#endif //!__WIN_THREAD_H__