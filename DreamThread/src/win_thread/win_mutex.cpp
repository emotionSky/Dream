#include "DreamSky/win_thread/win_mutex.h"
#if defined(_WIN32) || defined(_WINDLL)
#include <Windows.h>
#include <process.h>
#endif

DREAM_NAMESPACE_BEGIN

#if defined(_WIN32) || defined(_WINDLL)

class DWinCs::CsData
{
public:
	CRITICAL_SECTION m_cs;
};

DWinCs::DWinCs()
{
	m_pData = new CsData();
	InitializeCriticalSection(&m_pData->m_cs);
}

DWinCs::~DWinCs()
{
	if (m_pData)
	{
		DeleteCriticalSection(&m_pData->m_cs);
		delete m_pData;
		m_pData = nullptr;
	}
}

void DWinCs::Lock()
{
	EnterCriticalSection(&m_pData->m_cs);
}

void DWinCs::Unlock()
{
	LeaveCriticalSection(&m_pData->m_cs);
}

DWinMutex::DWinMutex(const char* name)
{
	m_mutex = CreateMutexA(nullptr, FALSE, name);
}

DWinMutex::~DWinMutex()
{
	if (m_mutex)
	{
		CloseHandle(m_mutex);
		m_mutex = nullptr;
	}
}

void DWinMutex::Lock()
{
	WaitForSingleObject(m_mutex, INFINITE);
}

void DWinMutex::Unlock()
{
	ReleaseMutex(m_mutex);
}

#endif

DREAM_NAMESPACE_END
