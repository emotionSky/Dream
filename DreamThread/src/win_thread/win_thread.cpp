#include "DreamSky/win_thread/win_thread.h"
#if defined(_WIN32) || defined(_WINDLL)
#include <Windows.h>
#include <process.h> // _beginthreadex  _endthreadex
#endif
#include <vector>

DREAM_NAMESPACE_BEGIN

#if defined(_WIN32) || defined(_WINDLL)

DWinThread::DWinThread() : 
	m_bCreated(false), m_bRunning(false),
	m_threadID(0), m_suspendCnt(0),
	m_threadHdl(nullptr), m_threadFunc(nullptr)
{}

DWinThread::~DWinThread()
{
	CloseThread();
}

bool DWinThread::CreateOneThread(thread_func func, bool is_run)
{
	if (!func)
		return false;

	if (m_bCreated)
		return m_bCreated;

	m_threadFunc = func;
	m_threadHdl = CreateThread(
		nullptr,
		0,
		DWinThread::ThreadFunc,
		this,
		is_run ? 0 : CREATE_SUSPENDED,
		&m_threadID);
	if (m_threadHdl)
	{
		m_bCreated = true;
		m_bRunning = is_run;
	}
	else
	{
		this->_GetErrorString();
	}
	return m_bCreated;
}

bool DWinThread::SuspendThread()
{
	bool res = false;
	if (m_bRunning)
	{
		DWORD ret = ::SuspendThread(m_threadHdl);
		if (ret == -1)
		{
			this->_GetErrorString();
		}
		else
		{
			m_suspendCnt = ret;
			m_bRunning = false;
			res = true;
		}
	}
	else
	{
		m_errStr = "没有运行的线程";
	}
	return res;
}

bool DWinThread::RunThread()
{
	bool res = false;
	if (m_bCreated && !m_bRunning && m_threadHdl)
	{
		DWORD ret = ResumeThread(m_threadHdl);
		if (ret == -1)
		{
			this->_GetErrorString();
		}
		else
		{
			m_suspendCnt = ret;
			m_bRunning = true;
			m_errStr.clear();
			res = true;
		}
	}
	else
	{
		m_errStr = "没有可用线程";
	}
	return res;
}

bool DWinThread::WaitThread(int timeout_ms)
{
	bool res = false;
	if (m_bRunning)
	{
		DWORD timeout = static_cast<int>(timeout_ms);
		DWORD ret = WaitForSingleObject(m_threadHdl, timeout);
		if (ret == -1)
		{
			this->_GetErrorString();
		}
		else
		{
			res = true;
			m_errStr.clear();
		}
	}	
	else
	{
		m_errStr = "没有运行的线程";
	}
	return res;
}

void DWinThread::CloseThread()
{
	if (m_threadHdl)
	{
		CloseHandle(m_threadHdl);
		m_threadHdl = nullptr;
		m_threadID = 0;
	}
	m_suspendCnt = 0;
	m_errStr.clear();
	m_bRunning = false;
	m_bCreated = false;
}

unsigned long DWinThread::ThreadFunc(void* p)
{
	DWinThread* pTh = static_cast<DWinThread*>(p);
	if (pTh)
	{
		pTh->m_threadFunc();
	}
	return 0;
}

void DWinThread::_GetErrorString()
{
	int err = GetLastError();
	char* pStr = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pStr, 0, NULL);
	m_errStr = pStr;
	LocalFree(pStr);
}

DWinThreadEx::DWinThreadEx() :
	m_bDetached(false), m_bStarted(false),
	m_threadID(0), m_arg(this), 
	m_func(dwin_thread_func),
	m_threadHdl(nullptr)
{}

DWinThreadEx::DWinThreadEx(thread_func func, void* arg) : 
	m_bDetached(false), m_bStarted(false),
	m_threadID(0), m_arg(arg), m_func(func), 
	m_threadHdl(nullptr)
{}

DWinThreadEx::~DWinThreadEx()
{
	if (!m_bDetached)
	{
		StopThread();
	}

	if (m_threadHdl)
	{
		CloseHandle(m_threadHdl);
		m_threadHdl = nullptr;
	}
}

bool DWinThreadEx::StartThread()
{
	if (!m_bStarted)
	{
		m_threadHdl = (d_handle)_beginthreadex(nullptr, 0, m_func, m_arg, 0, &m_threadID);
		if (m_threadHdl)
			m_bStarted = true;
	}

	return m_bStarted;
}

bool DWinThreadEx::StopThread(int timeout_ms)
{
	bool res = true;
	if (m_bStarted)
	{
		DWORD ret = WaitForSingleObject(m_threadHdl, timeout_ms);
		if (ret == WAIT_FAILED)
		{
			res = false;
		}
		else
		{
			CloseHandle(m_threadHdl);
			m_threadHdl = nullptr;
			m_bStarted = false;
		}
	}
	return res;
}

bool DWinThreadEx::DetachThread()
{
	if (!m_bDetached)
	{
		m_threadHdl = (d_handle)_beginthreadex(nullptr, 0, m_func, m_arg, 0, &m_threadID);
		if (m_threadHdl)
		{
			CloseHandle(m_threadHdl);
			m_threadHdl = nullptr;
			m_bDetached = true;
		}		
	}
	return m_bDetached;
}

unsigned int DWinThreadEx::dwin_thread_func(void* p)
{
	DWinThreadEx* dt = static_cast<DWinThreadEx*>(p);
	dt->ThreadFunc();
	return 0;
}

void DWinThreadEx::ThreadFunc()
{
	// 需要在继承类中实现
}

class DWinThreadExManager::ManagerData
{
public:
	ManagerData() = default;
	~ManagerData()
	{
		for (auto& thread : m_threads)
		{
			delete thread;
		}
		m_threads.clear();
	}

	std::vector<DWinThreadEx*> m_threads;
	std::vector<d_handle>      m_handles;
};

DWinThreadExManager::DWinThreadExManager()
{
	m_pData = new ManagerData();
}

DWinThreadExManager::~DWinThreadExManager()
{	
	if (m_pData)
	{
		WaitThreads(true, -1);
		delete m_pData;
		m_pData = nullptr;
	}
}

bool DWinThreadExManager::AllocNewThread(DWinThreadEx::thread_func func, void* arg)
{
	DWinThreadEx* thread = new DWinThreadEx(func, arg);
	if (thread->StartThread())
	{
		m_pData->m_threads.push_back(thread);
		m_pData->m_handles.push_back(thread->m_threadHdl);
		return true;
	}
	delete thread;
	return false;
}

void DWinThreadExManager::WaitThreads(bool wait_all, int timeout_ms)
{
	if (!m_pData->m_threads.empty())
	{
		DWORD count = static_cast<DWORD>(m_pData->m_handles.size());
		BOOL b_wait = wait_all ? TRUE : FALSE;
		WaitForMultipleObjects(count, &m_pData->m_handles[0], b_wait, timeout_ms);
	}
}

#endif

DREAM_NAMESPACE_END


