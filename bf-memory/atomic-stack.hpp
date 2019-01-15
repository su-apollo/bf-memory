#pragma once
#include <atomic>

namespace bf {
namespace atomic {
// none allocator stack
struct node {
	node* next;
};

struct head {
	node* next;
};

struct stack {
	std::atomic<head> header;
};

inline void push(stack& s, node* n) {
	head prev = s.header.load();
	head now;
	do {
		now.next = n;
	} while (!s.header.compare_exchange_weak(prev, now));
}

inline node* pop(stack& s) {
	head prev = s.header.load();
	head now;
	do {
		if (prev.next == nullptr)
			return nullptr;

		now.next = prev.next->next;
	} while (!s.header.compare_exchange_weak(prev, now));
	return prev.next;
}
}
}