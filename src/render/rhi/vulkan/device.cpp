#include "device.h"
#include "system/system.h"
#include "render/renderer.h"
#include "render/drawable.h"

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

	score += static_cast<int>(physical_device.m_properties.limits.maxImageDimension2D);

	if (!physical_device.m_features.imageCubeArray || 
		!physical_device.m_features.geometryShader ||
		!physical_device.m_features.samplerAnisotropy)
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

VkFormat VK_CLASS(PhysicalDevice)::find_supported_format(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const
{
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(m_device, format, &props);
		if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			return format;
		else if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			 return format;
	}
	RENDERING_LOG_ERROR("failed to find supported format!");
	return VK_FORMAT_UNDEFINED;
}

void VK_CLASS(PhysicalDevice)::initialize()
{
	pick_physical_device();
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
		SwapChainSupportDetails support_details;
		if(extensions_supported)
			support_details = query_swap_chain_support(device, instance->m_surface);
		int score = rate_score(device, physical_device);
		if (!physical_device.m_indices || !extensions_supported || !support_details)
			score = 0;
		else if(physical_device.m_indices.is_same_family())
			score += 1000;
		candidates.emplace(score, physical_device);
	}

	if (candidates.rbegin()->first > 0)
	{
		*this = candidates.rbegin()->second;
		vkGetPhysicalDeviceMemoryProperties(m_device, &m_memory_properties);
	}
	else
		RENDERING_LOG_ERROR("no matched physical device!");
}

VK_CLASS(Device)::~VK_CLASS(Device)()
{
	vmaDestroyAllocator(m_allocator);
	vkDestroyDevice(m_device, nullptr);
}

bool VK_CLASS(Device)::present() const
{
	auto drawable = g_system_context->g_render_system->m_drawable;
	auto swap_chain = drawable->m_swap_chain;
	VkPresentInfoKHR present_info{
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores = &drawable->m_command_buffer->m_render_finished_semaphores[drawable->m_command_buffer->m_current_frame]->m_semaphore,
		.swapchainCount = 1,
		.pSwapchains = &swap_chain->m_swap_chain,
		.pImageIndices = &swap_chain->m_image_index
	};
	VkResult result = vkQueuePresentKHR(m_present_queue, &present_info);
	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		return false;
	else
		VK_CHECK_RESULT(result);

	return true;
}

void VK_CLASS(Device)::wait_idle() const
{
	VK_CHECK_RESULT(vkDeviceWaitIdle(m_device));
}

void VK_CLASS(Device)::initialize()
{
	create_logical_device();
	create_vma_allocator();
}

void VK_CLASS(Device)::create_logical_device()
{	
	std::vector<const char*> layers;
#if defined(_DEBUG) || defined(DEBUG)
	std::vector<VkLayerProperties> layer_properties = vkEnumerateProperties(vkEnumerateInstanceLayerProperties);
	if (std::ranges::any_of(layer_properties,
	                        [](const VkLayerProperties& property) -> bool
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

	VkPhysicalDeviceFeatures device_features{
        .imageCubeArray = VK_TRUE,
		.geometryShader = VK_TRUE,
        .samplerAnisotropy = VK_TRUE,
	};
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

void VK_CLASS(Device)::create_vma_allocator()
{
	VmaVulkanFunctions functions{
#define VMA_VULKAN_FUNCTION(name) name = name
		.VMA_VULKAN_FUNCTION(vkGetInstanceProcAddr),
		.VMA_VULKAN_FUNCTION(vkGetDeviceProcAddr),
		.VMA_VULKAN_FUNCTION(vkGetPhysicalDeviceProperties),
		.VMA_VULKAN_FUNCTION(vkGetPhysicalDeviceMemoryProperties),
		.VMA_VULKAN_FUNCTION(vkAllocateMemory),
		.VMA_VULKAN_FUNCTION(vkFreeMemory),
		.VMA_VULKAN_FUNCTION(vkMapMemory),
		.VMA_VULKAN_FUNCTION(vkUnmapMemory),
		.VMA_VULKAN_FUNCTION(vkFlushMappedMemoryRanges),
		.VMA_VULKAN_FUNCTION(vkInvalidateMappedMemoryRanges),
		.VMA_VULKAN_FUNCTION(vkBindBufferMemory),
		.VMA_VULKAN_FUNCTION(vkBindImageMemory),
		.VMA_VULKAN_FUNCTION(vkGetBufferMemoryRequirements),
		.VMA_VULKAN_FUNCTION(vkGetImageMemoryRequirements),
		.VMA_VULKAN_FUNCTION(vkCreateBuffer),
		.VMA_VULKAN_FUNCTION(vkDestroyBuffer),
		.VMA_VULKAN_FUNCTION(vkCreateImage),
		.VMA_VULKAN_FUNCTION(vkDestroyImage),
		.VMA_VULKAN_FUNCTION(vkCmdCopyBuffer),
#if VMA_DEDICATED_ALLOCATION || VMA_VULKAN_VERSION >= 1001000
		.VMA_VULKAN_FUNCTION(vkGetBufferMemoryRequirements2KHR),
		.VMA_VULKAN_FUNCTION(vkGetImageMemoryRequirements2KHR),
#endif
#if VMA_BIND_MEMORY2 || VMA_VULKAN_VERSION >= 1001000
		.VMA_VULKAN_FUNCTION(vkBindBufferMemory2KHR),
		.VMA_VULKAN_FUNCTION(vkBindImageMemory2KHR),
#endif
#if VMA_MEMORY_BUDGET || VMA_VULKAN_VERSION >= 1001000
		.VMA_VULKAN_FUNCTION(vkGetPhysicalDeviceMemoryProperties2KHR),
#endif
#if VMA_KHR_MAINTENANCE4 || VMA_VULKAN_VERSION >= 1003000
		.VMA_VULKAN_FUNCTION(vkGetDeviceBufferMemoryRequirements),
		.VMA_VULKAN_FUNCTION(vkGetDeviceImageMemoryRequirements),
#endif
#undef VMA_VULKAN_FUNCTION
	};

	auto instance = g_system_context->g_render_system->m_drawable->m_instance;
	auto physical_device = g_system_context->g_render_system->m_drawable->m_physical_device;

	VmaAllocatorCreateInfo allocatorCreateInfo = {
		.flags = VMA_ALLOCATOR_CREATE_EXTERNALLY_SYNCHRONIZED_BIT,
		.physicalDevice = physical_device->m_device,
		.device = m_device,
		.pVulkanFunctions = &functions,
		.instance = instance->m_instance,
		.vulkanApiVersion = VK_API_VERSION_1_0,
	};

	VK_CHECK_RESULT(vmaCreateAllocator(&allocatorCreateInfo, &m_allocator));
}

ENGINE_NAMESPACE_END