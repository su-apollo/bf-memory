#pragma once
#include <atomic>

namespace bf {
namespace atomic {
// none allocator stack
struct Node {
	Node* next;
};

struct Head {
	Node* next;
};

struct Stack {
	std::atomic<Head> header;
};

inline void Push(Stack& s, Node* n) {
	Head prev = s.header.load();
	Head now;
	do {
		now.next = n;
	} while (!s.header.compare_exchange_weak(prev, now));
}

inline Node* Pop(Stack& s) {
	Head prev = s.header.load();
	Head now;
	do {
		if (prev.next == nullptr)
			return nullptr;

		now.next = prev.next->next;
	} while (!s.header.compare_exchange_weak(prev, now));
	return prev.next;
}
}
}