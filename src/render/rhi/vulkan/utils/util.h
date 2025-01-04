#pragma once
#include "render/rhi/vulkan/types.h"
#include "base/util.h"
#include <func.h>

ENGINE_NAMESPACE_BEGIN

struct SwapChainSupportDetails
{
	std::optional<VkSurfaceCapabilitiesKHR>		capabilities;
	std::vector<VkSurfaceFormatKHR>				formats;
	std::vector<VkPresentModeKHR>				present_modes;

	NODISCARD constexpr explicit operator bool() const
	{
		return capabilities.has_value() && !formats.empty() && !present_modes.empty();
	}
};

SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface);


const char* vk_result_convert(VkResult res);

#if defined(_DEBUG) || defined(DEBUG)
#define VK_CHECK_RESULT(expr)																		\
{																									\
	VkResult res = expr;																			\
	if (res != VK_SUCCESS)																			\
	{																								\
		const char* message = vk_result_convert(res);												\
        std::string format = fmt::format(															\
			"[vulkan, fatal, result]: VkResult is \"{}\" in line: {}, function: {}, file: {}.",     \
			message, __LINE__, __FUNCTION__, __FILE__);												\
		std::cerr << format << std::endl;															\
		fatal_error(message);																		\
	}																								\
}
#else
#define VK_CHECK_RESULT(expr) (expr)
#endif

template<typename F, typename... Args>
auto vkEnumerateProperties(F&& func, Args&&... args)
{
	using type = std::remove_pointer_t<typename Trait::tail_type_t<typename Trait::function_traits<std::decay_t<F>>::argument_type>>;
	uint32_t count = 0;
	std::vector<type> properties;
	if constexpr (!std::is_same_v<typename Trait::function_traits<std::decay_t<F>>::return_type, void>)
	{
		VK_CHECK_RESULT(Trait::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, nullptr));
		properties.resize(count);
		VK_CHECK_RESULT(Trait::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, properties.data()));
	}
	else
	{
		Trait::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, nullptr);
		properties.resize(count);
		Trait::invoke(std::forward<F>(func), std::forward<Args>(args)..., &count, properties.data());
	}
	return properties;
}

ENGINE_NAMESPACE_END