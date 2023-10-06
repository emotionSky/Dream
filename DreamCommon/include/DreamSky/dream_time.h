#ifndef __DREAM_TIME_H__
#define __DREAM_TIME_H__

#include "dream_define.h"
#include <cstdint>

DREAM_NAMESPACE_BEGIN

DREAMSKY_API void SleepUs(unsigned usec);
DREAMSKY_API void SleepMs(unsigned msec);
DREAMSKY_API void SleepSe(unsigned secs);
DREAMSKY_API uint64_t GetCurrentMs();

DREAM_NAMESPACE_END

#endif //!__DREAM_TIME_H__