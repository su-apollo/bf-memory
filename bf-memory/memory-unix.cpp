#include "memory.hpp"

#if BF_UNIX_FAMILY

namespace bf {
void* aligned_alloc(std::size_t size) {
#if BF_APPLE_FAMILY
// Always 16byte memory align on apple side.
	return ::malloc(size);
#else
	return ::memalign(BF_MEMORY_ALLOCATION_ALIGNMENT, size);	
#endif
}

void* aligned_alloc(std::size_t alignment, std::size_t size) {
#if BF_APPLE_FAMILY
// Always 16byte memory align on apple side.
	return ::malloc(size);
#else
	return ::memalign(alignment, size);
#endif
}

void aligned_free(void* ptr) {
	::free(ptr);
}
}
#endif