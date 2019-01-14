#pragma once
#include "atomic-stack.hpp"
#include "memory.h"
#include "atomic.h"

#include <memory>
#include <cassert>

namespace bf {
// todo : 64bit align
struct malloc_info : atomic::node {
	malloc_info(int size) : alloc_size(size), extra_info(-1) {
	}
	long alloc_size;
	long extra_info;
};

inline void* attach_malloc_info(malloc_info* header, int size) {
	new (header)malloc_info(size);
	return reinterpret_cast<void*>(++header);
}

inline malloc_info* detach_malloc_info(void* ptr) {
	malloc_info* header = reinterpret_cast<malloc_info*>(ptr);
	--header;
	return header;
}

class memory_bucket {
public:
	memory_bucket(unsigned long size);

	malloc_info* pop();
	void push(malloc_info* ptr);

private:
	atomic::stack free_list;

	const unsigned long alloc_size;
	long alloc_count;
};

// only used global
class memory_pool {
public:
	memory_pool();

	void* allocate(int size);
	void deallocate(void* ptr, long extra_info);

private:
	enum {
		MAX_BUCKET_COUNT = 1024 / 32 + 1024 / 128 + 2048 / 256,
		MAX_ALLOC_SIZE = 4096
	};

	// for O(1) access
	memory_bucket* bucket_table[MAX_ALLOC_SIZE + 1];
};

extern memory_pool* pool;
//struct PooledAllocatable {};

template <class T, class... Args>
T* xnew(Args&&... args) {
	//static_assert(true == std::is_convertible<T, PooledAllocatable>::value, "[fs::memory-pool.h] only allowed when PooledAllocatable");

	void* alloc = pool->allocate(sizeof(T));
	new (alloc)T(std::forward<Args>(args)...);
	return reinterpret_cast<T*>(alloc);
}

template <class T>
void xdelete(T* object) {
	//static_assert(true == std::is_convertible<T, PooledAllocatable>::value, "[fs::memory-pool.h] only allowed when PooledAllocatable");

	object->~T();
	pool->deallocate(object, sizeof(T));
}
}