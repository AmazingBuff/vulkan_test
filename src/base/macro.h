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

#if defined(_DEBUG) || defined(DEBUG)
#define ASSERT(expr)					assert(expr)
#else
#define ASSERT(expr)
#endif // DEBUG
