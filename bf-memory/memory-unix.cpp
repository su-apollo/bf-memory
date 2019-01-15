#include "memory.hpp"

#if BF_UNIX_FAMILY
#include <malloc/malloc.h>

namespace bf {
void* aligned_alloc(std::size_t size) {
	return ::memalign(BF_MEMORY_ALLOCATION_ALIGNMENT, size);
}

void* aligned_alloc(std::size_t alignment, std::size_t size) {
	return ::memalign(alignment, size);
}

void aligned_free(void* ptr) {
	::free(ptr);
}
}
#endif