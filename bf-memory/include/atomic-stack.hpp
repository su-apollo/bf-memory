#pragma once
#include <atomic>

namespace bf {
namespace atomic {
// none allocator stack
struct node {
	node() : next(nullptr) {}
	node* next;
};

struct stack {
	stack() : head(nullptr) {}
	std::atomic<node*> head;
};

inline void push(stack& s, node* n) {
	node* prev = s.head.load();
	do {
		n->next = prev;
	} while (!s.head.compare_exchange_weak(prev, n));
}

inline node* pop(stack& s) {
	node* prev = s.head.load();
	node* n = nullptr;
	do {
		if (prev == nullptr)
			return nullptr;

		n = prev->next;
	} while (!s.head.compare_exchange_weak(prev, n));
	return prev;
}
}
}