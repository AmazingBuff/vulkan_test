#pragma once

#include "types.h"
#include "utils/util.h"

VK_NAMESPACE_BEGIN

class VK_CLASS(Instance) final : public RHI
{
public:
	VK_CLASS(Instance)() = default;
	~VK_CLASS(Instance)() override;
	void initialize() override;
	NODISCARD constexpr RHIFlag flag() const override;
	NODISCARD VkInstance get_instance() const;
	NODISCARD VkSurfaceKHR get_surface() const;
private:
	void create_instance();
	void create_surface();
#if defined(_DEBUG) || defined(DEBUG)
	void setup_debug_messenger();
#endif
private:
	VK_TYPE_INIT(VkInstance, m_instance);
	VK_TYPE_INIT(VkSurfaceKHR, m_surface);
#if defined(_DEBUG) || defined(DEBUG)
	VK_TYPE_INIT(VkDebugUtilsMessengerEXT, m_debug_messenger);
#endif
};

VK_NAMESPACE_END