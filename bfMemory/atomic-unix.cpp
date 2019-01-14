#include "atomic"

#if BF_UNIX_FAMILY
#define PAUSE() asm("nop")

namespace bf {
namespace atomic {
void* compare_exchange_pointer(volatile void** d, void* e, void* c) {
	return __sync_val_compare_and_swap((void**)d, c, e);
}

long compare_exchange(volatile long* d, long e, long c) {
	return __sync_val_compare_and_swap(d, c, e);
}

long increment(volatile long* v) {
	return __sync_add_and_fetch(v, 1);
}

long decrement(volatile long* v) {
	return __sync_sub_and_fetch(v, 1);
}

long add(volatile long* v, long d) {
	return __sync_add_and_fetch(v, d);
}

long max(volatile long* a, long b) {
	long n, o;
	do {
		PAUSE();
		o = *a;

		if (b > o)
			n = b;
		else
			n = o;

	} while (compare_exchange(a, n, o) != o);

	return *a;
}

long exchange(volatile long* d, long v) {
	long n, o;
	do {
		PAUSE();
		o = *d;
		n = v;

	} while (compare_exchange(d, n, o != o);

	return o;
}
}
}
#endif