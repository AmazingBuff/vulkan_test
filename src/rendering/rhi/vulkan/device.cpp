#include "device.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"
#include "window/window.h"
#include <SDL2/SDL_vulkan.h>

ENGINE_NAMESPACE_BEGIN

static int rate_score(VkPhysicalDevice device, VK_CLASS(PhysicalDevice)& physical_device)
{
	vkGetPhysicalDeviceProperties(device, &physical_device.m_properties);
	vkGetPhysicalDeviceFeatures(device, &physical_device.m_features);

	int score = 0;
	if (physical_device.m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;
	else if (physical_device.m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
		score += 500;
	else if (physical_device.m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
		score += 250;
	else if (physical_device.m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
		score += 100;

	score += physical_device.m_properties.limits.maxImageDimension2D;

	if (!physical_device.m_features.geometryShader)
		return 0;

	return score;
}

static VK_CLASS(PhysicalDevice)::QueueFamilyIndices find_queue_families(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
	VK_CLASS(PhysicalDevice)::QueueFamilyIndices indices;
	std::vector<VkQueueFamilyProperties> queue_properties = vkEnumerateProperties(vkGetPhysicalDeviceQueueFamilyProperties, physical_device);
	for (size_t i = 0; i < queue_properties.size(); i++)
	{
		if (queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphics_family = i;

		VkBool32 present_support = false;
		VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(physical_device, i, surface, &present_support));
		if (present_support)
			indices.present_family = i;

		if (indices)
			break;
	}
	return indices;
}

static bool check_device_extension_support(VkPhysicalDevice device)
{
	std::vector<VkExtensionProperties> available_extensions = vkEnumerateProperties(vkEnumerateDeviceExtensionProperties, device, nullptr);
	std::set<std::string> required_extensions(Device_Extensions.begin(), Device_Extensions.end());
	for (auto& extension : available_extensions)
		required_extensions.erase(extension.extensionName);
	return required_extensions.empty();
}

static VK_CLASS(PhysicalDevice)::SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
	VK_CLASS(PhysicalDevice)::SwapChainSupportDetails support_details;
	VkSurfaceCapabilitiesKHR capabilities;
	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities));
	support_details.capabilities = std::move(capabilities);
	support_details.formats = vkEnumerateProperties(vkGetPhysicalDeviceSurfaceFormatsKHR, physical_device, surface);
	support_details.present_modes = vkEnumerateProperties(vkGetPhysicalDeviceSurfacePresentModesKHR, physical_device, surface);
	return support_details;
}

void VK_CLASS(PhysicalDevice)::initialize()
{
	pick_physical_device();
	choose_swap_chain_details();
}

constexpr RHIFlag VK_CLASS(PhysicalDevice)::flag() const
{
	return RHIFlag::e_physical_device;
}

void VK_CLASS(PhysicalDevice)::pick_physical_device()
{
	auto instance = g_system_context->g_render_system->m_drawable->m_instance;
	std::vector<VkPhysicalDevice> devices = vkEnumerateProperties(vkEnumeratePhysicalDevices, instance->m_instance);

	std::multimap<int, PhysicalDevice> candidates;
	for (auto& device : devices)
	{
		PhysicalDevice physical_device;
		physical_device.m_device = device;
		physical_device.m_indices = find_queue_families(device, instance->m_surface);
		bool extensions_supported = check_device_extension_support(device);
		if(extensions_supported)
			physical_device.m_support_details = query_swap_chain_support(device, instance->m_surface);
		int score = rate_score(device, physical_device);
		if (!physical_device.m_indices || !extensions_supported || !physical_device.m_support_details)
			score = 0;
		else if(physical_device.m_indices.is_same_family())
			score += 1000;
		candidates.emplace(score, physical_device);
	}

	if (candidates.rbegin()->first > 0)
		*this = candidates.rbegin()->second;
	else
		ASSERT(candidates.rbegin()->first > 0);
}

void VK_CLASS(PhysicalDevice)::choose_swap_chain_details()
{
	if(!std::any_of(m_support_details.formats.begin(), m_support_details.formats.end(), 
		[&](const VkSurfaceFormatKHR& available_format) -> bool
		{
			if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB &&
				available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				m_swap_chain_details.format = available_format;
				return true;
			}
			else
				return false;
		}))
		m_swap_chain_details.format = m_support_details.formats[0];

	if (!std::any_of(m_support_details.present_modes.begin(), m_support_details.present_modes.end(),
		[&](const VkPresentModeKHR& available_present_mode) -> bool
		{
			if (available_present_mode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				m_swap_chain_details.present_mode = available_present_mode;
				return true;
			}
			else
				return false;
		}))
		m_swap_chain_details.present_mode = VK_PRESENT_MODE_FIFO_KHR;

	if (m_support_details.capabilities->currentExtent.width != std::numeric_limits<uint32_t>::max())
		m_swap_chain_details.extent = m_support_details.capabilities->currentExtent;
	else
	{
		int width, height;
		SDL_Vulkan_GetDrawableSize(g_system_context->g_window_system->get_window(), &width, &height);
		VkExtent2D actual_extent{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };
		actual_extent.width = std::clamp(actual_extent.width, m_support_details.capabilities->minImageExtent.width, m_support_details.capabilities->maxImageExtent.width);
		actual_extent.height = std::clamp(actual_extent.height, m_support_details.capabilities->minImageExtent.height, m_support_details.capabilities->maxImageExtent.height);
		m_swap_chain_details.extent = actual_extent;
	}
}

VK_CLASS(Device)::~VK_CLASS(Device)()
{
	vkDestroyDevice(m_device, nullptr);
}

void VK_CLASS(Device)::initialize()
{
	create_logical_device();
}

constexpr RHIFlag VK_CLASS(Device)::flag() const
{
	return RHIFlag::e_device;
}

void VK_CLASS(Device)::create_logical_device()
{	
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
		layers.push_back(Validation_Layers);
#endif
	
	auto physical_device = g_system_context->g_render_system->m_drawable->m_physical_device;

	float queue_priority = 1.0f;
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	if (physical_device->m_indices.is_same_family())
	{
		queue_create_infos.push_back({
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = physical_device->m_indices.graphics_family.value(),
			.queueCount = 1,
			.pQueuePriorities = &queue_priority
			});
	}
	else
	{
		std::set<uint32_t> unique_families = { physical_device->m_indices.graphics_family.value(), physical_device->m_indices.present_family.value() };
		for (uint32_t queue_family : unique_families)
		{
			queue_create_infos.push_back({
				.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
				.queueFamilyIndex = queue_family,
				.queueCount = 1,
				.pQueuePriorities = &queue_priority
				});
		}
	}

	VkPhysicalDeviceFeatures device_features{};
	VkDeviceCreateInfo device_create_info{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size()),
		.pQueueCreateInfos = queue_create_infos.data(),
		.enabledLayerCount = static_cast<uint32_t>(layers.size()),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(Device_Extensions.size()),
		.ppEnabledExtensionNames = Device_Extensions.data(),
		.pEnabledFeatures = &device_features
	};

	VK_CHECK_RESULT(vkCreateDevice(physical_device->m_device, &device_create_info, nullptr, &m_device));
	vkGetDeviceQueue(m_device, physical_device->m_indices.graphics_family.value(), 0, &m_graphics_queue);
	vkGetDeviceQueue(m_device, physical_device->m_indices.present_family.value(), 0, &m_present_queue);
}

ENGINE_NAMESPACE_END