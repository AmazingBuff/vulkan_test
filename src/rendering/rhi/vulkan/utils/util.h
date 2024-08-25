#pragma once
#include "rendering/rhi/vulkan/types.h"
#include <traits/func.h>

VK_NAMESPACE_BEGIN

const char* vk_result_convert(VkResult res);
			
#define VK_CHECK_RESULT(expr)														\
{																					\
	VkResult res = expr;															\
	if (res != VK_SUCCESS)															\
	{																				\
		std::cerr << "Fatal : VkResult is \"" << vk_result_convert(res) << "\" in "	\
					<< __FILE__	<< " at line " << __LINE__ << std::endl;			\
		ASSERT(res == VK_SUCCESS);													\
	}																				\
}

template<typename F, typename... Args>
auto vkEnumerateProperties(F&& func, Args&&... args)
{
	using type = std::remove_pointer_t<typename Trait::tail_type_t<typename Trait::function_traits<std::decay_t<F>>::argument_type>>;
	uint32_t count = 0;
	VK_CHECK_RESULT(std::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, nullptr));
	std::vector<type> properties(count);
	VK_CHECK_RESULT(std::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, properties.data()));
	return properties;
}


VK_NAMESPACE_END