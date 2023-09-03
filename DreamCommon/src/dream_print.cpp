#include <DreamSky/dream_print.h>
#include <cstdarg>
#include <cstdio>

#define BUFFER_SIZE 1024

DREAMSKY_ALWAYS_INLINE DreamPrintLevel GetPrintLevel(int level)
{
	if (level < 0)
	{
		return PRINT_DEBUG;
	}
	else if (level > PRINT_ERROR)
	{
		return PRINT_ERROR;
	}
	else
	{
		return static_cast<DreamPrintLevel>(level);
	}
}

void print_console(int level, const char* fmt, ...)
{
	va_list args;
	char buffer[BUFFER_SIZE] = { 0 };
	va_start(args, fmt);
	int ret = vsnprintf(buffer, BUFFER_SIZE - 1, fmt, args);
	if (ret < 0)
	{
		va_end(args);
		return;
	}
	va_end(args);
	char* p = buffer;
	p += ret;
	*p++ = '\n';
	
	DreamPrintLevel p_level = GetPrintLevel(level);

	switch (p_level)
	{
	case PRINT_DEBUG:
		fputs("DEBUG: ", stderr);
		fputs(buffer, stdout);
		break;
	case PRINT_INFOR:
		fputs("INFOR: ", stderr);
		fputs(buffer, stdout);
		break;
	case PRINT_ERROR:
		fputs("ERROR: ", stderr);
		fputs(buffer, stderr);
		break;
	default:
		break;
	}
}
