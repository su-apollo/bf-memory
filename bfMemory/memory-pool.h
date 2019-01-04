#pragma once
#include "atomic-stack.hpp"
#include "memory.h"
#include "atomic.h"

#include <memory>
#include <cassert>

namespace bf {
struct MallocInfo : atomic::Node {
	MallocInfo(int size) : alloc_size(size), extra_info(-1) {
	}
	long alloc_size;
	long extra_info;
};

inline void* AttachMallocInfo(MallocInfo* header, int size) {
	new (header)MallocInfo(size);
	return reinterpret_cast<void*>(++header);
}

inline MallocInfo* DetachMallocInfo(void* ptr) {
	MallocInfo* header = reinterpret_cast<MallocInfo*>(ptr);
	--header;
	return header;
}

class MemoryBucket {
public:
	MemoryBucket(unsigned long size);

	MallocInfo* Pop();
	void Push(MallocInfo* ptr);

private:
	atomic::Stack free_list;

	const unsigned long alloc_size;
	long alloc_count;
};

/// global로만 사용하기 때문에 쿨하게 소멸자 정의 안함
class MemoryPool {
public:
	MemoryPool();

	void* Allocate(int size);
	void Deallocate(void* ptr, long extra_info);

private:
	/// todo : 사이즈 쫌 조절해야함
	enum Config {
		/// 함부로 바꾸면 안됨. 철저히 계산후 바꿀 것
		MAX_BUCKET_COUNT = 1024 / 32 + 1024 / 128 + 2048 / 256, ///< ~1024까지 32단위, ~2048까지 128단위, ~4096까지 256단위
		MAX_ALLOC_SIZE = 4096
	};

	/// 원하는 크기의 메모리를 가지고 있는 풀에 O(1) access를 위한 테이블
	MemoryBucket* bucket_table[MAX_ALLOC_SIZE + 1];
};

extern MemoryPool* Pool;
/// 요놈을 상속 받아야만 xnew/xdelete 사용할 수 있게...
//struct PooledAllocatable {};

template <class T, class... Args>
T* xnew(Args&&... args) {
	//static_assert(true == std::is_convertible<T, PooledAllocatable>::value, "[fs::memory-pool.h] only allowed when PooledAllocatable");

	void* alloc = Pool->Allocate(sizeof(T));
	new (alloc)T(std::forward<Args>(args)...);
	return reinterpret_cast<T*>(alloc);
}

template <class T>
void xdelete(T* object) {
	//static_assert(true == std::is_convertible<T, PooledAllocatable>::value, "[fs::memory-pool.h] only allowed when PooledAllocatable");

	object->~T();
	Pool->Deallocate(object, sizeof(T));
}
}