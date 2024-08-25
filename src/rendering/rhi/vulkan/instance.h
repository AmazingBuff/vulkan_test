#pragma once

#include "types.h"
#include "utils/util.h"
#include "window/window.h"

VK_NAMESPACE_BEGIN

class Instance final : public RHI
{
public:
	explicit Instance(const std::shared_ptr<WINDOW_NAMESPACE::Window>& window);
	~Instance() override;
	void initialize() override;
	NODISCARD VkInstance get_instance() const;
	NODISCARD VkSurfaceKHR get_surface() const;
private:
	void create_instance();
	void create_surface();
#if defined(_DEBUG) || defined(DEBUG)
	void setup_debug_messenger();
#endif
private:
	std::shared_ptr<WINDOW_NAMESPACE::Window> m_window;
private:
	VK_TYPE_INIT(VkInstance, m_instance);
	VK_TYPE_INIT(VkSurfaceKHR, m_surface);
#if defined(_DEBUG) || defined(DEBUG)
	VK_TYPE_INIT(VkDebugUtilsMessengerEXT, m_debug_messenger);
#endif
};

VK_NAMESPACE_END