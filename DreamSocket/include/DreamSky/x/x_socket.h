#ifndef __X_SOCKET_H__
#define __X_SOCKET_H__

#include <DreamSky/dream_define.h>

#if defined(_WIN32) || defined(_WINDLL) || defined(__CYGWIN__)
#else
#include <cstdio>

#define close_socket   close
#define sock_t         int
#define invalid_sock   -1
#define sock_error     -1
#define epoll_close    close

#endif

#endif //!__X_SOCKET_H__