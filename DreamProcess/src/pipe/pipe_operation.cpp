#include "DreamSky/pipe/pipe_operation.h"
#include <cstring>

#if defined(_WIN32) || defined(_WINDLL)
#include <Windows.h>
#else
#include <unistd.h>
#endif

DREAM_NAMESPACE_BEGIN

#if defined(_WIN32) || defined(_WINDLL)
void GetErrorStringByNumber(int err, dstring& str)
{
	char* pStr = nullptr;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pStr, 0, NULL);
	str = pStr;
}
#endif

DreamPipe::DreamPipe() :
#if defined(_WIN32) || defined(_WINDLL)
	m_writeHdl(nullptr), m_readHdl(nullptr)
#else
	m_writeFd(-1), m_readFd(-1)
#endif
{}

DreamPipe::~DreamPipe()
{

}

bool DreamPipe::CreatePipe(bool is_named, const char* name)
{
	if (is_named)
	{
		return false;
	}

#if defined(_WIN32) || defined(_WINDLL)
	SECURITY_ATTRIBUTES saAttr;

	// 设置管道属性
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// 创建管道
	if (!::CreatePipe(&m_readHdl, &m_writeHdl, &saAttr, 0)) 
	{
		return false;
	}
	return true;
#else
	int fd[2];
	int ret = pipe(fd);
	if (ret == -1)
	{
		return false;
	}
	else
	{
		m_readFd = fd[0];
		m_writeFd = fd[1];
		return true;
	}
#endif
}

int DreamPipe::WriteData(const char* buf, int len)
{
#if defined(_WIN32) || defined(_WINDLL)
	DWORD write_len = 0;
	if (!WriteFile(m_writeHdl, buf, len, &write_len, nullptr))
	{
		return -1;
	}
	return write_len;
#else
	return write(m_writeFd, buf, len);
#endif
}

int DreamPipe::ReadData(char* buf, int size)
{
#if defined(_WIN32) || defined(_WINDLL)
	DWORD read_len = 0;
	if (!ReadFile(m_readHdl, buf, size, &read_len, nullptr))
	{
		return -1;
	}
	return read_len;
#else
	return read(m_readFd, buf, size);
#endif
}

void DreamPipe::CloseWritePipe()
{
#if defined(_WIN32) || defined(_WINDLL)
	if (m_writeHdl)
	{
		CloseHandle(m_writeHdl);
		m_writeHdl = nullptr;
	}
#else
	if (m_writeFd >= 0)
	{
		close(m_writeFd);
		m_writeFd = -1;
	}
#endif
}

void DreamPipe::CloseReadPipe()
{
#if defined(_WIN32) || defined(_WINDLL)
	if (m_readHdl)
	{
		CloseHandle(m_readHdl);
		m_readHdl = nullptr;
	}
#else
	if (m_readFd >= 0)
	{
		close(m_readFd);
		m_readFd = -1;
	}
#endif
}

#if defined(_WIN32) || defined(_WINDLL)
d_handle DreamPipe::GetStdReadPipe()
{
	return GetStdHandle(STD_INPUT_HANDLE);
}

void DreamPipe::CloseStdReadPipe(d_handle hdl)
{
	if (hdl)
		CloseHandle(hdl);
}

#endif

DREAM_NAMESPACE_END
