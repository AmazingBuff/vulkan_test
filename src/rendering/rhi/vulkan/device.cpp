#include "device.h"

VK_NAMESPACE_BEGIN

static int rate_score(VkPhysicalDevice device, VkPhysicalDeviceProperties& properties, VkPhysicalDeviceFeatures& features)
{
	vkGetPhysicalDeviceProperties(device, &properties);
	vkGetPhysicalDeviceFeatures(device, &features);

	int score = 0;
	if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;
	else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
		score += 500;
	else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
		score += 250;
	else if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
		score += 100;

	score += properties.limits.maxImageDimension2D;

	if (!features.geometryShader)
		return 0;

	return score;
}

static PhysicalDevice::QueueFamilyIndices find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	std::vector<VkQueueFamilyProperties> queue_properties = vkEnumerateProperties(vkGetPhysicalDeviceQueueFamilyProperties, device);
	PhysicalDevice::QueueFamilyIndices indices;
	for (size_t i = 0; i < queue_properties.size(); i++)
	{
		if (queue_properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			indices.graphics_family = i;
		if (indices)
			break;
	}
	return indices;
}


PhysicalDevice::PhysicalDevice(const std::shared_ptr<Instance>& instance) : m_instance(instance) {}

PhysicalDevice::PhysicalDevice(const PhysicalDevice& other)
{
	m_instance = other.m_instance;
	m_device = other.m_device;
	m_properties = other.m_properties;
	m_features = other.m_features;
	m_indices = other.m_indices;
	m_score = other.m_score;
}

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
		physical_device.m_score = rate_score(device, physical_device.m_properties, physical_device.m_features);
		physical_device.m_indices = find_queue_families(device);
		physical_device.m_device = device;
		if (!physical_device.m_indices)
			physical_device.m_score = 0;
		candidates.emplace(physical_device.m_score, physical_device);
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
	VkDeviceQueueCreateInfo queue_create_info{
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = m_physical_device->m_indices.graphics_family.value(),
		.queueCount = 1,
		.pQueuePriorities = &queue_priority
	};

	VkPhysicalDeviceFeatures device_features{};
	VkDeviceCreateInfo device_create_info{
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &queue_create_info,
		.enabledLayerCount = static_cast<uint32_t>(layers.size()),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = 0,
		.ppEnabledExtensionNames = nullptr,
		.pEnabledFeatures = &device_features
	};

	VK_CHECK_RESULT(vkCreateDevice(m_physical_device->m_device, &device_create_info, nullptr, &m_device));
	vkGetDeviceQueue(m_device, m_physical_device->m_indices.graphics_family.value(), 0, &m_queue);
}

VK_NAMESPACE_END