#include "atomic.hpp"

#if BF_WINDOWS_FAMILY
#include <windows.h>
#undef max
#undef min

namespace bf {
	namespace atomic {
		long exchange(volatile long *d, long v) {
			return (long) InterlockedExchange(d, (LONG) v);
		}

		void *exchange_pointer(volatile void **d, void *v) {
			return InterlockedExchangePointer((volatile PVOID *) d, v);
		}

		long compare_exchange(volatile long *d, long e, long c) {
			return (long) InterlockedCompareExchange((volatile LONG *) d, e, c);
		}

		void *compare_exchange_pointer(volatile void **d, void *e, void *c) {
			return InterlockedCompareExchangePointer((volatile PVOID *) d, e, c);
		}

		long increment(volatile long *v) {
			return (long) InterlockedIncrement((volatile LONG *) v);
		}

		long decrement(volatile long *v) {
			return (long) InterlockedDecrement((volatile LONG *) v);
		}

		long add(volatile long *v, long d) {
			LONG n, o;
			do {
				o = *v;
				n = o + d;

			} while (InterlockedCompareExchange((volatile LONG *) v, n, o) != o);

			return n;
		}

		long max(volatile long *a, long b) {
			LONG n, o;
			do {
				o = *a;

				if (b > o)
					n = b;
				else
					n = o;

			} while (InterlockedCompareExchange((volatile LONG *) a, n, o) != o);

			return n;
		}
	}
}
#endif