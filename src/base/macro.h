#pragma once

#include <cassert>

#define NAMESPACE_BEGIN(name)	namespace name {
#define NAMESPACE_END(name)		}

#define STRING(x)				#x

#define ZERO_MEMORY(ptr, size)	memset(ptr, 0, size)

#if defined(_DEBUG) || defined(DEBUG)
#define ASSERT(expr)			assert(expr)
#else
#define ASSERT(expr)
#endif // DEBUG
