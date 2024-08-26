#include "device.h"

VK_NAMESPACE_BEGIN

static int rate_score(PhysicalDevice& physical_device)
{
	vkGetPhysicalDeviceProperties(physical_device.m_device, &physical_device.m_properties);
	vkGetPhysicalDeviceFeatures(physical_device.m_device, &physical_device.m_features);

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

static void find_queue_families(PhysicalDevice& physical_device, VkSurfaceKHR surface)
{
	std::vector<VkQueueFamilyProperties> queue_properties = vkEnumerateProperties(vkGetPhysicalDeviceQueueFamilyProperties, physical_device.m_device);
	for (size_t i = 0; i < queue_properties.size(); i++)
	{
		if (queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			physical_device.m_indices.graphics_family = i;

		VkBool32 present_support = false;
		VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceSupportKHR(physical_device.m_device, i, surface, &present_support));
		if (present_support)
			physical_device.m_indices.present_family = i;

		if (physical_device.m_indices)
			return;
	}
}

static bool check_device_extension_support(VkPhysicalDevice device)
{
	std::vector<VkExtensionProperties> available_extensions = vkEnumerateProperties(vkEnumerateDeviceExtensionProperties, device, nullptr);
	std::set<std::string> required_extensions(Device_Extensions.begin(), Device_Extensions.end());
	for (auto& extension : available_extensions)
		required_extensions.erase(extension.extensionName);
	return required_extensions.empty();
}

static void query_swap_chain_support(PhysicalDevice& physical_device, VkSurfaceKHR surface)
{
	VkSurfaceCapabilitiesKHR capabilities;
	VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device.m_device, surface, &capabilities));
	physical_device.m_support_details.capabilities = std::move(capabilities);
	physical_device.m_support_details.formats = vkEnumerateProperties(vkGetPhysicalDeviceSurfaceFormatsKHR, physical_device.m_device, surface);
	physical_device.m_support_details.present_modes = vkEnumerateProperties(vkGetPhysicalDeviceSurfacePresentModesKHR, physical_device.m_device, surface);
}


PhysicalDevice::PhysicalDevice(const std::shared_ptr<Instance>& instance) : m_instance(instance) {}

void PhysicalDevice::initialize()
{
	pick_physical_device();
}

void PhysicalDevice::pick_physical_device()
{
	std::vector<VkPhysicalDevice> devices = vkEnumerateProperties(vkEnumeratePhysicalDevices, m_instance->get_instance());

	std::multimap<int, PhysicalDevice> candidates;
	for (auto& device : devices)
	{
		PhysicalDevice physical_device(m_instance);
		physical_device.m_device = device;
		find_queue_families(physical_device, m_instance->get_surface());
		bool extensions_supported = check_device_extension_support(device);
		if(extensions_supported)
			query_swap_chain_support(physical_device, m_instance->get_surface());
		int score = rate_score(physical_device);
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

Device::Device(const std::shared_ptr<Instance>& instance, 
	const std::shared_ptr<PhysicalDevice>& physical_device) 
	: m_instance(instance), m_physical_device(physical_device) {}

Device::~Device()
{
	vkDestroyDevice(m_device, nullptr);
}

void Device::initialize()
{
	create_logical_device();
}

void Device::create_logical_device()
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
	
	float queue_priority = 1.0f;
	std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
	if (m_physical_device->m_indices.is_same_family())
	{
		queue_create_infos.push_back({
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = m_physical_device->m_indices.graphics_family.value(),
			.queueCount = 1,
			.pQueuePriorities = &queue_priority
			});
	}
	else
	{
		std::set<uint32_t> unique_families = { m_physical_device->m_indices.graphics_family.value(), m_physical_device->m_indices.present_family.value() };
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

	VK_CHECK_RESULT(vkCreateDevice(m_physical_device->m_device, &device_create_info, nullptr, &m_device));
	vkGetDeviceQueue(m_device, m_physical_device->m_indices.graphics_family.value(), 0, &m_graphics_queue);
	vkGetDeviceQueue(m_device, m_physical_device->m_indices.present_family.value(), 0, &m_present_queue);
}

VK_NAMESPACE_END