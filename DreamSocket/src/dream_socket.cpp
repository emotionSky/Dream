#include <DreamSky/dream_socket.h>
#include <cstring>

#if defined(_WIN32) || defined(_WINDLL)
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib ")
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
#endif

DREAM_NAMESPACE_BEGIN

static int s_opts[] =
{
	SO_SNDBUF,
	SO_RCVBUF,
};

bool InitSocketEnv()
{
#if defined(_WIN32) || defined(_WINDLL)
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return false;
	}
	return true;
#else
	return true;
#endif
}

bool CleanSocketEnv()
{
#if defined(_WIN32) || defined(_WINDLL)
	if (WSACleanup() != 0)
	{
		return false;
	}
	return true;
#else
	return true;
#endif
}

int GetSocketError()
{
	int err = 0;
#if defined(_WIN32) || defined(_WINDLL)
	err = GetLastError();
#else
	err = errno;
#endif
	return err;
}

SockType GetSockType(sock_t sock)
{
	int sock_type = 0;
	socklen_t type_len = static_cast<socklen_t>(sizeof(int));
	int res = getsockopt(sock, SOL_SOCKET, SO_TYPE, (char*)&sock_type, &type_len);
	if (res)
	{
		return SockTypeError;
	}
	else
	{
		if (sock_type == SOCK_STREAM)
			return SockTcpType;
		else if (sock_type == SOCK_DGRAM)
			return SockUdpType;
		else if (sock_type == SOCK_RAW)
			return SockRawType;
		else if (sock_type == SOCK_RDM)
			return SockRdmType;
		else if (sock_type == SOCK_SEQPACKET)
			return SockSeqpacketType;
		else
			return SockTypeError;
	}
}

bool SetSockOpt(sock_t sock, SockOptType type, int val)
{
#if defined(_WIN32) || defined(_WINDLL)
	using ptr = char*;
#else
	using ptr = void*;
#endif

	int res = setsockopt(sock, SOL_SOCKET, s_opts[type], (ptr)&val, 4);
	if (res)
		return false;
	return true;
}

int GetSockOpt(sock_t sock, SockOptType type)
{
#if defined(_WIN32) || defined(_WINDLL)
	using ptr = char*;
#else
	using ptr = void*;
#endif
	int tmp = 0;
	socklen_t len = 4;
	int res = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (ptr)&tmp, &len);
	if (res)
		return res;
	return tmp;
}

SocketError::SocketError() : 
	m_pStr(nullptr)
{
#if defined(_WIN32) || defined(_WINDLL)
	m_err = GetLastError();
#else
	m_err = errno;
#endif
}

SocketError::~SocketError()
{
#if defined(_WIN32) || defined(_WINDLL)
	if (m_pStr)
	{
		LocalFree(m_pStr);
		m_pStr = nullptr;
	}
#endif
}

int SocketError::GetSocketError() const
{
	return m_err;
}

const char* SocketError::GetErrorString()
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
