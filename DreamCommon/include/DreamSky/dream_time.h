﻿#ifndef __DREAM_TIME_H__
#define __DREAM_TIME_H__

#include <DreamSky/dream_define.h>
#include <cstdint>

DREAMSKY_API void SleepUs(unsigned usec);
DREAMSKY_API void SleepMs(unsigned msec);
DREAMSKY_API uint64_t GetCurrentMs();

#endif //!__DREAM_TIME_H__