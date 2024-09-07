#pragma once

#include "types.h"
#include "utils/util.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(Instance) final : public RHI
{
public:
	VK_CLASS(Instance)() = default;
	~VK_CLASS(Instance)() override;
	void initialize() override;
	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_instance();
	void create_surface();
#if defined(_DEBUG) || defined(DEBUG)
	void setup_debug_messenger();
#endif
private:
	VK_TYPE_INIT(VkInstance,				m_instance);
	VK_TYPE_INIT(VkSurfaceKHR,				m_surface);
#if defined(_DEBUG) || defined(DEBUG)
	VK_TYPE_INIT(VkDebugUtilsMessengerEXT,	m_debug_messenger);
#endif

	friend class VK_CLASS(PhysicalDevice);
	friend class VK_CLASS(Device);
	friend class VK_CLASS(SwapChain);
};

ENGINE_NAMESPACE_END