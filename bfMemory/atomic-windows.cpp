#include "atomic.h"

#if BF_WINDOWS_FAMILY
#include <windows.h>

namespace bf {
namespace atomic {
long Exchange(volatile long* d, long v)
{
	return (long)InterlockedExchange((volatile LONG*)d, (LONG)v);
}

void* ExchangePointer(volatile void** d, void* v)
{
	return InterlockedExchangePointer((volatile PVOID*)d, v);
}

long CompareExchange(volatile long* d, long e, long c)
{
	return (long)InterlockedCompareExchange((volatile LONG*)d, e, c);
}

void* CompareExchangePointer(volatile void** d, void* e, void* c)
{
	return InterlockedCompareExchangePointer((volatile PVOID*)d, e, c);
}

long Increment(volatile long* v)
{
	return (long)InterlockedIncrement((volatile LONG*)v);
}

long Decrement(volatile long* v)
{
	return (long)InterlockedDecrement((volatile LONG*)v);
}

long Add(volatile long* v, long d)
{
	LONG n, o;
	do
	{
		o = *v;
		n = o + d;

	} while (InterlockedCompareExchange((volatile LONG*)v, n, o) != o);

	return n;
}

long Max(volatile long* a, long b)
{
	LONG n, o;
	do
	{
		o = *a;

		if (b > o)
			n = b;
		else
			n = o;

	} while (InterlockedCompareExchange((volatile LONG*)a, n, o) != o);

	return n;
}
}
}
#endif