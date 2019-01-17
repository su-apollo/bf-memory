#pragma once
#include "preprocessor.hpp"

namespace bf {
namespace atomic {
long	exchange(volatile long* destination, long value);
void*	exchange_pointer(volatile void** destination, void* value);
long	compare_exchange(volatile long* destination, long exchange, long compare);
void*	compare_exchange_pointer(volatile void** destination, void* exchange, void* compare);
long	increment(volatile long* value);
long	decrement(volatile long* value);
long	add(volatile long* value, long delta);
long	max(volatile long* a, long b);
}
}