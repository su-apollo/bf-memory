#include "memory-pool.h"

namespace bf {
MemoryPool* Pool = nullptr;

MemoryBucket::MemoryBucket(unsigned long size) : alloc_size(size), alloc_count(0) {
	if (alloc_size < BF_MEMORY_ALLOCATION_ALIGNMENT)
		throw std::bad_alloc();
}

MallocInfo* MemoryBucket::Pop() {
	MallocInfo* mem = static_cast<MallocInfo*>(atomic::Pop(free_list));

	if (mem == nullptr) {
		mem = reinterpret_cast<MallocInfo*>(aligned_alloc(alloc_size));
	}
	else {
		if (mem->alloc_size != 0)
			throw std::bad_alloc();
	}

	atomic::Increment(&alloc_count);
	return mem;
}

void MemoryBucket::Push(MallocInfo* ptr) {
	atomic::Push(free_list, ptr);
	atomic::Decrement(&alloc_count);
}

MemoryPool::MemoryPool() {
	memset(bucket_table, 0, sizeof(bucket_table));

	int recent = 0;

	for (int i = 32; i < 1024; i += 32) {
		MemoryBucket* pool = new MemoryBucket(i);
		for (int j = recent + 1; j <= i; ++j) {
			bucket_table[j] = pool;
		}
		recent = i;
	}

	for (int i = 1024; i < 2048; i += 128) {
		MemoryBucket* pool = new MemoryBucket(i);
		for (int j = recent + 1; j <= i; ++j) {
			bucket_table[j] = pool;
		}
		recent = i;
	}

	for (int i = 2048; i <= 4096; i += 256) {
		MemoryBucket* pool = new MemoryBucket(i);
		for (int j = recent + 1; j <= i; ++j) {
			bucket_table[j] = pool;
		}
		recent = i;
	}
}

void* MemoryPool::Allocate(int size) {
	MallocInfo* header = nullptr;
	int real_size = size + sizeof(MallocInfo);

	if (real_size > MAX_ALLOC_SIZE) {
		header = reinterpret_cast<MallocInfo*>(aligned_alloc(real_size));
	}
	else {
		header = bucket_table[real_size]->Pop();
	}

	return AttachMallocInfo(header, real_size);
}

void MemoryPool::Deallocate(void* ptr, long extra_info) {
	MallocInfo* header = DetachMallocInfo(ptr);
	header->extra_info = extra_info; ///< 최근 할당에 관련된 정보 힌트

	long real_size = atomic::Exchange(&header->alloc_size, 0); ///< 두번 해제 체크 위해

	if (real_size <= 0)
		throw std::bad_alloc();

	if (real_size > MAX_ALLOC_SIZE) {
		aligned_free(header);
	}
	else {
		bucket_table[real_size]->Push(header);
	}
}
}