#ifndef __DREAM_PRINT_H__
#define __DREAM_PRINT_H__

#include <DreamSky/dream_define.h>

enum DreamPrintLevel
{
	PRINT_DEBUG = 0,
	PRINT_INFOR,
	PRINT_ERROR,
};

DREAMSKY_API void print_console(int level, const char* fmt, ...);

#endif //!__DREAM_PRINT_H__