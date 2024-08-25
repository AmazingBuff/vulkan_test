#pragma once

#include "types.h"

VK_NAMESPACE_BEGIN

class Instance final : public RHI
{
public:
	Instance() = default;
	~Instance() override;
	void initialize() override;

private:
	void create_instance();
#if defined(_DEBUG) || defined(DEBUG)
	void setup_debug_messenger();
#endif
private:
	VkInstance m_instance;
#if defined(_DEBUG) || defined(DEBUG)
	VkDebugUtilsMessengerEXT m_debug_messenger;
#endif
};

VK_NAMESPACE_END