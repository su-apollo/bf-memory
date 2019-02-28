#include "memory-pool.hpp"
#include "memory.hpp"

#include <cstring>

namespace bf {
memory_pool* pool = nullptr;

memory_bucket::memory_bucket(unsigned long size) : alloc_size(size), alloc_count(0) {
	if (alloc_size < BF_MEMORY_ALLOCATION_ALIGNMENT)
		throw std::bad_alloc();
}

malloc_info* memory_bucket::pop() {
	malloc_info* mem = static_cast<malloc_info*>(atomic::pop(free_list));

	if (mem == nullptr) {
		mem = reinterpret_cast<malloc_info*>(bf::aligned_alloc(alloc_size));
	}
	else {
		if (mem->alloc_size != 0)
			throw std::bad_alloc();
	}

	atomic::increment(&alloc_count);
	return mem;
}

void memory_bucket::push(malloc_info* ptr) {
	atomic::push(free_list, ptr);
	atomic::decrement(&alloc_count);
}

memory_pool::memory_pool() {
	std::memset(bucket_table, 0, sizeof(bucket_table));

	int recent = 0;

	for (int i = 32; i < 1024; i += 32) {
		auto pool = new memory_bucket(i);
		for (int j = recent + 1; j <= i; ++j) {
			bucket_table[j] = pool;
		}
		recent = i;
	}

	for (int i = 1024; i < 2048; i += 128) {
		auto pool = new memory_bucket(i);
		for (int j = recent + 1; j <= i; ++j) {
			bucket_table[j] = pool;
		}
		recent = i;
	}

	for (int i = 2048; i <= 4096; i += 256) {
		auto pool = new memory_bucket(i);
		for (int j = recent + 1; j <= i; ++j) {
			bucket_table[j] = pool;
		}
		recent = i;
	}
}

void* memory_pool::allocate(int size) {
	malloc_info* header = nullptr;
	auto real_size = size + sizeof(malloc_info);

	if (real_size > MAX_ALLOC_SIZE) {
		header = reinterpret_cast<malloc_info*>(bf::aligned_alloc(real_size));
	}
	else {
		header = bucket_table[real_size]->pop();
	}

	return attach_malloc_info(header, real_size);
}

void memory_pool::deallocate(void* ptr, long extra_info) {
	malloc_info* header = detach_malloc_info(ptr);
	header->extra_info = extra_info; // hint

	long real_size = atomic::exchange(&header->alloc_size, 0);

	if (real_size <= 0)
		throw std::bad_alloc();

	if (real_size > MAX_ALLOC_SIZE) {
		bf::free(header);
	}
	else {
		bucket_table[real_size]->push(header);
	}
}
}