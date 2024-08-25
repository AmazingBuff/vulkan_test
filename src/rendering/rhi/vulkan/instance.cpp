#include "instance.h"
#include "utils/util.h"


VK_NAMESPACE_BEGIN

#if defined(_DEBUG) || defined(DEBUG)
static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
	VkDebugUtilsMessageTypeFlagsEXT message_type,
	const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
	void* user_data)
{
	std::string type;
	switch (message_type)
	{
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		type = "General";
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		type = "Validation";
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		type = "Performance";
		break;
	case VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT:
		type = "Device Address Binding";
		break;
	default:
		break;
	}

	switch (message_severity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		std::cout << "[Verbose, " << type << "]: " << callback_data->pMessage << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		std::cout << "[Info, " << type << "]: " << callback_data->pMessage << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		std::cerr << "[Warning, " << type << "]: " << callback_data->pMessage << std::endl;
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		std::cerr << "[Error, " << type << "]: " << callback_data->pMessage << std::endl;
		break;
	default:
		break;
	}

	return VK_FALSE;
}
#endif

Instance::~Instance()
{
#if defined(_DEBUG) || defined(DEBUG)
	vkDestroyDebugUtilsMessengerEXT(m_instance, m_debug_messenger, nullptr);
#endif
	vkDestroyInstance(m_instance, nullptr);
}

void Instance::initialize()
{
	create_instance();
#if defined(_DEBUG) || defined(DEBUG)
	setup_debug_messenger();
#endif
}

void Instance::create_instance()
{
	VK_CHECK_RESULT(volkInitialize());

	std::vector<const char*> extensions(Platform_Extensions);
	std::vector<const char*> layers;
#if defined(_DEBUG) || defined(DEBUG)
	std::vector<VkLayerProperties> layer_properties = vkEnumerateProperties(vkEnumerateInstanceLayerProperties);
	if (std::any_of(layer_properties.begin(), layer_properties.end(),
		[&layer_properties](const VkLayerProperties& property) -> bool
		{
			if (std::strcmp(property.layerName, Validation_Layers) == 0)
				return true;
			else
				return false;
		}))
	{
		layers.push_back(Validation_Layers);
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
#endif

	std::vector<VkExtensionProperties> extension_properties = vkEnumerateProperties(vkEnumerateInstanceExtensionProperties, nullptr);
	size_t counter = 0;
	bool extension_support = false;
	for (auto& property : extension_properties)
	{
		for (auto& extension : extensions)
		{
			if (std::strcmp(property.extensionName, extension) == 0)
				counter++;
			if (counter == extensions.size())
			{
				extension_support = true;
				break;
			}
		}
	}

	if (!extension_support)
	{
		std::cerr << "Fatal : Required extensions are not supported." << std::endl;
		ASSERT(extension_support);
	}

	VkApplicationInfo app_info{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = Application_Name,
		.applicationVersion = Application_Version,
		.pEngineName = Engine_Name,
		.engineVersion = Engine_Version,
		.apiVersion = Vulkan_Version
	};

#if defined(_DEBUG) || defined(DEBUG)
	VkDebugUtilsMessengerCreateInfoEXT debug_utils_create_info{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
						VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
						VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
						VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
		.pfnUserCallback = debug_callback,
		.pUserData = nullptr
	};
#endif

	VkInstanceCreateInfo create_info{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#if defined(_DEBUG) || defined(DEBUG)
		.pNext = &debug_utils_create_info,
#endif
		.pApplicationInfo = &app_info,
		.enabledLayerCount = static_cast<uint32_t>(layers.size()),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
		.ppEnabledExtensionNames = extensions.data(),
	};

	VK_CHECK_RESULT(vkCreateInstance(&create_info, nullptr, &m_instance));
	volkLoadInstance(m_instance);
}

#if defined(_DEBUG) || defined(DEBUG)
void Instance::setup_debug_messenger()
{
	VkDebugUtilsMessengerCreateInfoEXT create_info{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
							VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
						VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
						VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT |
						VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT,
		.pfnUserCallback = debug_callback,
		.pUserData = nullptr
	};

	VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(m_instance, &create_info, nullptr, &m_debug_messenger));
}


#endif

VK_NAMESPACE_END