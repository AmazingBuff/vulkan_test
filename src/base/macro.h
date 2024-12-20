#pragma once

#include <cassert>

#define NAMESPACE_BEGIN(name)			namespace name {
#define NAMESPACE_END(name)				}
#define NAMESPACE(name)					name
#define NAMESPACE_VARIABLE(name, var)	name::var
#define NAMESPACE_CONCAT(a, b)			a::b

#define AMAZING_NAMESPACE_BEGIN			NAMESPACE_BEGIN(Amazing)	
#define AMAZING_NAMESPACE_END			NAMESPACE_END(Amazing)	
#define AMAZING_NAMESPACE				NAMESPACE(Amazing)	
#define AMAZING_NAMESPACE_CONCAT(name)	NAMESPACE_CONCAT(NAMESPACE(Amazing), NAMESPACE(name))

#define ENGINE_NAMESPACE_BEGIN			AMAZING_NAMESPACE_BEGIN		\
										NAMESPACE_BEGIN(Engine)
#define ENGINE_NAMESPACE_END			NAMESPACE_END(Engine)			\
										AMAZING_NAMESPACE_END
#define ENGINE_NAMESPACE				NAMESPACE_CONCAT(AMAZING_NAMESPACE, NAMESPACE(Engine))
#define ENGINE_NAMESPACE_VARIABLE(name)	NAMESPACE_CONCAT(ENGINE_NAMESPACE, NAMESPACE(name))

#define STRING(x)						#x
#define CONCAT(a, b)					a##b
#define IDENTITY(x)						x

#define ZERO_MEMORY(ptr, size)			memset(ptr, 0, size)

#define NODISCARD						[[nodiscard]]
#define NORETURN						[[noreturn]]

#if defined(_DEBUG) || defined(DEBUG)
#define ASSERT(expr)					assert(expr)
#else
#define ASSERT(expr)
#endif // DEBUG

#define EXPAND(...) EXPAND4(EXPAND4(EXPAND4(EXPAND4(__VA_ARGS__))))
#define EXPAND4(...) EXPAND3(EXPAND3(EXPAND3(EXPAND3(__VA_ARGS__))))
#define EXPAND3(...) EXPAND2(EXPAND2(EXPAND2(EXPAND2(__VA_ARGS__))))
#define EXPAND2(...) EXPAND1(EXPAND1(EXPAND1(EXPAND1(__VA_ARGS__))))
#define EXPAND1(...) __VA_ARGS__

#define APPLY(macro, ...) IDENTITY(macro(__VA_ARGS__))
// for further use
// only support 10 arguments for now
#define COUNT_N(_1, _2, _3, _4 ,_5, _6, _7, _8, _9, _10, N, ...) N 
#define COUNT(...) APPLY(COUNT_N, __VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)




#ifdef HIGH_PRECISION_FLOAT
using Float = double;
#else
using Float = float;
#endif // HIGH_PRECISION_FLOAT