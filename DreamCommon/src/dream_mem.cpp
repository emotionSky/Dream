#include <DreamSky/dream_mem.h>
#include <malloc.h>

DREAM_NAMESPACE_BEGIN

void* alloc(size_t size)
{
	return malloc(size);
}

void free(void* p, size_t size)
{
	::free(p);
}

void* realloc(void* p, size_t old_size, size_t new_size)
{
	return ::realloc(p, new_size);
}

DREAM_NAMESPACE_END
