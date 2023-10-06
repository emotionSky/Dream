#include <DreamSky/dream_time.h>

#if defined(_WIN32) || defined(_WINDLL)
#include <windows.h>
#else
#include <unistd.h>
#include <ctime>
#endif // _WIN32

DREAM_NAMESPACE_BEGIN

void SleepUs(unsigned usec)
{
#if defined(_WIN32) || defined(_WINDLL)
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * (int64_t)usec);

	timer = CreateWaitableTimer(nullptr, TRUE, nullptr);
	SetWaitableTimer(timer, &ft, 0, nullptr, nullptr, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
#else
	usleep(usec);
#endif
}

void SleepMs(unsigned msec)
{
#if defined(_WIN32) || defined(_WINDLL)
	Sleep(msec);
#else
	usleep(msec * 1000);
#endif
}

void SleepSe(unsigned secs)
{
#if defined(_WIN32) || defined(_WINDLL)
	Sleep(secs * 1000);
#else
	sleep(secs);
#endif
}

uint64_t GetCurrentMs()
{
	uint64_t time = 0;

#if defined(_WIN32) || defined(_WINDLL)
	static LARGE_INTEGER freq = { 0 };
	LARGE_INTEGER count;
	if (freq.QuadPart == 0)
	{
		if (QueryPerformanceFrequency(&freq))
		{
		}
	}
	QueryPerformanceCounter(&count);
	time = (uint64_t)(count.QuadPart
		/ ((double)freq.QuadPart
			/ 1000));
#else
	struct timespec count;
	if (clock_gettime(CLOCK_MONOTONIC, &count) < 0)
	{
		return 0;
	}
	time = (uint64_t)count.tv_sec * 1000 + count.tv_nsec / 1000000.0;
#endif

	return time;
}

DREAM_NAMESPACE_END
