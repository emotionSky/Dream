#include "DreamSky/win_thread/win_semaphore.h"
#if defined(_WIN32) || defined(_WINDLL)
#include <Windows.h>
#include <process.h>
#endif

DREAM_NAMESPACE_BEGIN

#if defined(_WIN32) || defined(_WINDLL)

DWinSem::DWinSem(const char* name, long value, long max_count)
{
	m_sem = CreateSemaphoreA(nullptr, value, max_count, name);
}

DWinSem::~DWinSem()
{
	if (m_sem)
	{
		CloseHandle(m_sem);
		m_sem = nullptr;
	}
}

void DWinSem::Wait()
{
	WaitForSingleObject(m_sem, INFINITE);
}

void DWinSem::Post(long release_count)
{
	ReleaseSemaphore(m_sem, release_count, nullptr);
}

DWinEvent::DWinEvent(bool manual, bool init)
{
	BOOL manual_set = manual ? TRUE : FALSE;
	BOOL init_set = init ? TRUE : FALSE;
	m_event = CreateEventA(nullptr, manual_set, init_set, nullptr);
}

DWinEvent::~DWinEvent()
{
	if (m_event)
	{
		CloseHandle(m_event);
		m_event = nullptr;
	}
}

void DWinEvent::Set()
{
	SetEvent(m_event);
}

void DWinEvent::Reset()
{
	ResetEvent(m_event);
}

void DWinEvent::Pulse()
{
	PulseEvent(m_event);
}

void DWinEvent::Wait()
{
	WaitForSingleObject(m_event, INFINITE);
}

#endif

DREAM_NAMESPACE_END