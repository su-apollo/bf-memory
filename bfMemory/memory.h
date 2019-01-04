#pragma once
#include "preprocessor.h"

#include <cstdlib>

namespace bf {
void* aligned_alloc(std::size_t size);
void* aligned_alloc(std::size_t alignment, std::size_t size);
void aligned_free(void* ptr);
}