#ifndef __DREAM_PRINT_H__
#define __DREAM_PRINT_H__

#include "dream_define.h"

DREAM_NAMESPACE_BEGIN

enum DreamPrintLevel
{
	PRINT_DEBUG = 0,
	PRINT_INFOR,
	PRINT_ERROR,
};

DREAMSKY_API void print_console(int level, const char* fmt, ...);

DREAM_NAMESPACE_END

#endif //!__DREAM_PRINT_H__