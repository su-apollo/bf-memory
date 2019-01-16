#include "memory.hpp"

#if BF_UNIX_FAMILY
#include <new>

namespace bf {
void* aligned_alloc(std::size_t size) {
	void *p = nullptr;
	if (posix_memalign(&p, BF_MEMORY_ALLOCATION_ALIGNMENT, size))
		throw std::bad_alloc();

	return p;
}

void* aligned_alloc(std::size_t alignment, std::size_t size) {
	void *p = nullptr;
	if (posix_memalign(&p, alignment, size))
		throw std::bad_alloc();

	return p;
}

void free(void* ptr) {
	::free(ptr);
}
}
#endif