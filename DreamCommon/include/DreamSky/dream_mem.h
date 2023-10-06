#ifndef __DREAM_MEM_H__
#define __DREAM_MEM_H__

#include "dream_define.h"
#include <cstddef>

DREAM_NAMESPACE_BEGIN

// 目前还是使用基础函数实现，后续再更改为自定义的实现 
DREAMSKY_API void* alloc(size_t size);
DREAMSKY_API void free(void* p, size_t size);
DREAMSKY_API void* realloc(void* p, size_t old_size, size_t new_size);

DREAM_NAMESPACE_END

#endif //!__DREAM_MEM_H__
