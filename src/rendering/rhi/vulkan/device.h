#pragma once

#include "instance.h"

VK_NAMESPACE_BEGIN

class PhysicalDevice final : public RHI
{
public:
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphics_family;
		std::optional<uint32_t> present_family;

		NODISCARD constexpr operator bool() const
		{
			return graphics_family.has_value() && present_family.has_value();
		}
	};

public:
	explicit PhysicalDevice(const std::shared_ptr<Instance>& instance);
	PhysicalDevice(const PhysicalDevice& other);
	~PhysicalDevice() override = default;
	void initialize() override;

	VK_TYPE_INIT(VkPhysicalDevice, m_device);
	VkPhysicalDeviceProperties			m_properties{};
	VkPhysicalDeviceFeatures			m_features{};
	QueueFamilyIndices					m_indices;
	int									m_score = 0;
private:
	void pick_physical_device();
private:
	std::shared_ptr<Instance>			m_instance;
};

class Device final : public RHI
{
public:
	explicit Device(const std::shared_ptr<Instance>& instance, 
		const std::shared_ptr<PhysicalDevice>& physical_device);
	~Device() override;
	void initialize() override;

private:
	void create_logical_device();
private:
	std::shared_ptr<Instance>			m_instance;
	std::shared_ptr<PhysicalDevice>		m_physical_device;
private:
	VK_TYPE_INIT(VkDevice, m_device);
	VK_TYPE_INIT(VkQueue, m_queue);
};

VK_NAMESPACE_END