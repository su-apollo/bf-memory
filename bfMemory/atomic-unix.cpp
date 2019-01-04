#include "atomic"

#if BF_UNIX_FAMILY
#define PAUSE() asm("nop")

namespace bf {
namespace atomic {
void* CompareExchangePointer(volatile void** d, void* e, void* c)
{
	return __sync_val_compare_and_swap((void**)d, c, e);
}

long CompareExchange(volatile long* d, long e, long c)
{
	return __sync_val_compare_and_swap(d, c, e);
}

long Increment(volatile long* v)
{
	return __sync_add_and_fetch(v, 1);
}

long Decrement(volatile long* v)
{
	return __sync_sub_and_fetch(v, 1);
}

long Add(volatile long* v, long d)
{
	return __sync_add_and_fetch(v, d);
}

long Max(volatile long* a, long b)
{
	long n, o;
	do
	{
		PAUSE();
		o = *a;

		if (b > o)
			n = b;
		else
			n = o;

	} while (CompareExchange(a, n, o) != o);

	return *a;
}

long Exchange(volatile long* d, long v)
{
	long n, o;
	do
	{
		PAUSE();
		o = *d;
		n = v;

	} while (CompareExchange(d, n, o != o);

	return o;
}
}
}
#endif