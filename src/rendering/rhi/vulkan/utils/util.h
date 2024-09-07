#pragma once
#include "rendering/rhi/vulkan/types.h"
#include <traits/func.h>

ENGINE_NAMESPACE_BEGIN

const char* vk_result_convert(VkResult res);

#if defined(_DEBUG) || defined(DEBUG)
#define VK_CHECK_RESULT(expr)																		\
{																									\
	VkResult res = expr;																			\
	if (res != VK_SUCCESS)																			\
	{																								\
		std::cerr << "Fatal : VkResult is \"" << vk_result_convert(res)								\
					<< "\" in "	<< __FILE__	<< " at line " << __LINE__ << std::endl;				\
		ASSERT(res == VK_SUCCESS);																	\
	}																								\
}
#else
#define VK_CHECK_RESULT(expr) (expr)
#endif

template<typename F, typename... Args>
	requires(!std::is_same_v<typename Trait::function_traits<std::decay_t<F>>::return_type, void>)
inline auto vkEnumerateProperties(F&& func, Args&&... args)
{
	using type = std::remove_pointer_t<typename Trait::tail_type_t<typename Trait::function_traits<std::decay_t<F>>::argument_type>>;
	uint32_t count = 0;
	VK_CHECK_RESULT(std::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, nullptr));
	std::vector<type> properties(count);
	VK_CHECK_RESULT(std::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, properties.data()));
	return properties;
}

template<typename F, typename... Args>
	requires(std::is_same_v<typename Trait::function_traits<std::decay_t<F>>::return_type, void>)
inline auto vkEnumerateProperties(F&& func, Args&&... args)
{
	using type = std::remove_pointer_t<typename Trait::tail_type_t<typename Trait::function_traits<std::decay_t<F>>::argument_type>>;
	uint32_t count = 0;
	std::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, nullptr);
	std::vector<type> properties(count);
	std::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, properties.data());
	return properties;
}


ENGINE_NAMESPACE_END