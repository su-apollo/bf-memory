#include "memory.h"

#if BF_WINDOWS_FAMILY
#include <windows.h>

namespace bf {
void* aligned_alloc(std::size_t size) {
	return _aligned_malloc(size, BF_MEMORY_ALLOCATION_ALIGNMENT);
}

void* aligned_alloc(std::size_t alignment, std::size_t size) {
	return _aligned_malloc(size, alignment);
}

void aligned_free(void* ptr) {
	_aligned_free(ptr);
}
}
#endif