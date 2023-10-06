#include "DreamSky/win/win_socket.h"

#if defined(_WIN32) || defined(_WINDLL)
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib ") 

void close_socket(sock_t sock)
{
	closesocket(sock);
}

#endif
