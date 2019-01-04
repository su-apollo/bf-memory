#pragma once
#include "preprocessor.h"

namespace bf {
namespace atomic {
long Exchange(volatile long* destination, long value);
void* ExchangePointer(volatile void** destination, void* value);
long CompareExchange(volatile long* destination, long exchange, long compare);
void* CompareExchangePointer(volatile void** destination, void* exchange, void* compare);
long Increment(volatile long* value);
long Decrement(volatile long* value);
long Add(volatile long* value, long delta);
long Max(volatile long* a, long b);
}
}