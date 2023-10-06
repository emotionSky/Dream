#include "DreamSky/dream_error.h"
#include <cstring>

#if defined(_WIN32) || defined(_WINDLL)
#include <Windows.h>
#else
#include <unistd.h>
#endif

DREAM_NAMESPACE_BEGIN

DreamError::DreamError() :
	m_pStr(nullptr)
{
#if defined(_WIN32) || defined(_WINDLL)
	m_err = GetLastError();
#else
	m_err = errno;
#endif
}

DreamError::~DreamError()
{
#if defined(_WIN32) || defined(_WINDLL)
	if (m_pStr)
	{
		LocalFree(m_pStr);
		m_pStr = nullptr;
	}
#endif
}

int DreamError::GetSocketError() const
{
	return m_err;
}

const char* DreamError::GetErrorString()
{
#if defined(_WIN32) || defined(_WINDLL)
	if (!m_pStr)
	{
		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, m_err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&m_pStr, 0, NULL);
	}
	return m_pStr;
#else
	return strerror(m_err);
#endif
}

DREAM_NAMESPACE_END
