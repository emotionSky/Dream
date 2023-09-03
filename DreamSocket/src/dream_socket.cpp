#include <DreamSky/dream_socket.h>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib ") 
#endif

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
