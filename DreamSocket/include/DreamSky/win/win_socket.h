#ifndef __WIN_SOCKET_H__
#define __WIN_SOCKET_H__

#if defined(_WIN32) || defined(_WINDLL)

#include <DreamSky/dream_define.h>
#include <stdint.h>
// windows 下使用这个来定义
#define sock_t         uintptr_t
#define invalid_sock   (sock_t)(~0)
#define sock_error     -1

#ifdef __cplusplus
extern "C" {
#endif

	DREAMSKY_API void close_socket(sock_t sock);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

#endif //!__WIN_SOCKET_H__
