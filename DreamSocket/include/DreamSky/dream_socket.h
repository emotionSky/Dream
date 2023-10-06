#ifndef __DREAM_SOCKET_H__
#define __DREAM_SOCKET_H__

#include "win/win_epoll.h"
#include "x/x_socket.h"

DREAM_NAMESPACE_BEGIN

enum SockType
{
	SockTypeError = -1,
	SockTcpType = 1,
	SockUdpType,
	SockRawType,
	SockRdmType,
	SockSeqpacketType,
};

enum SockOptType
{
	//SockOptSockType = 0,
	SockOptSendBuff = 0,
	SockOptRecvBuff,
};

DREAMSKY_API bool InitSocketEnv();
DREAMSKY_API bool CleanSocketEnv();
DREAMSKY_API int GetSocketError();
DREAMSKY_API SockType GetSockType(sock_t sock);
DREAMSKY_API bool SetSockOpt(sock_t sock, SockOptType type, int val);
DREAMSKY_API int GetSockOpt(sock_t sock, SockOptType type);

// 构造中直接得到错误码，注意使用时机！
class DREAMSKY_API SocketError
{
public:
	explicit SocketError();
	~SocketError();

	int GetSocketError() const;
	const char* GetErrorString();

#if __cplusplus >= 201103L
	SocketError& operator=(const SocketError&) = delete;
#else
private:
	SocketError& operator=(const SocketError&) { return *this; }
#endif

private:
	int   m_err;
	char* m_pStr;
};

DREAM_NAMESPACE_END

#endif //!__DREAM_SOCKET_H__