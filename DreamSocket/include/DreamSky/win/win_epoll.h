/*****************************************************************//**
 * \file   win_epoll.h
 * \brief  Windows下的epoll实现，代码源自GitHub开源：
 *         https://github.com/piscisaureus/wepoll
 *	       对开源作者表示感谢！！！
 *         本库中针对开源代码做了一定的修改，以符合本库的明明逻辑、代码风格等。
 * 
 * \author 凌佚(emotion_sky)
 * \date   October 2023
 *********************************************************************/

#ifndef __WIN_EPOLL_H__
#define __WIN_EPOLL_H__

#if defined(_WIN32) || defined(_WINDLL)

#include "win_socket.h"

enum EPOLL_EVENTS
{
	EPOLLIN      = (int)(1U << 0),
	EPOLLPRI     = (int)(1U << 1),
	EPOLLOUT     = (int)(1U << 2),
	EPOLLERR     = (int)(1U << 3),
	EPOLLHUP     = (int)(1U << 4),
	EPOLLRDNORM  = (int)(1U << 6),
	EPOLLRDBAND  = (int)(1U << 7),
	EPOLLWRNORM  = (int)(1U << 8),
	EPOLLWRBAND  = (int)(1U << 9),
	EPOLLMSG     = (int)(1U << 10), /* Never reported. */
	EPOLLRDHUP   = (int)(1U << 13),
	EPOLLONESHOT = (int)(1U << 31)
};

/** 
 * 这些都是wepoll不支持的部分
 * 标志	            说明
 * EPOLLONESHOT	    仅报告一次事件
 * EPOLLET	        wepoll不支持
 * EPOLLEXCLUSIVE	wepoll不支持
 * EPOLLWAKEUP	    wepoll不支持
 */

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_MOD 2
#define EPOLL_CTL_DEL 3

typedef union epoll_data_t
{
	void*    ptr;
	int      fd;
	uint32_t u32;
	uint64_t u64;
	sock_t   sock; /* Windows specific */
	d_handle hnd;  /* Windows specific */
}epoll_data_t;

struct epoll_event 
{
	uint32_t     events;   /* Epoll events and flags */
	epoll_data_t data;     /* User data variable */
};

#ifdef __cplusplus
extern "C" {
#endif

DREAMSKY_API d_handle epoll_create(int size);
DREAMSKY_API d_handle epoll_create1(int flags);

DREAMSKY_API int epoll_close(d_handle ephnd);

DREAMSKY_API int epoll_ctl(
	d_handle ephnd,
	int op,
	sock_t sock,
	struct epoll_event* event);

DREAMSKY_API int epoll_wait(
	d_handle ephnd,
	struct epoll_event* events,
	int maxevents,
	int timeout);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

#endif //!__WIN_EPOLL_H__