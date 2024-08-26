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

		NODISCARD constexpr bool is_same_family() const
		{
			ASSERT(*this);
			return graphics_family.value() == present_family.value();
		}
	};

	struct SwapChainSupportDetails
	{
		std::optional<VkSurfaceCapabilitiesKHR>		capabilities;
		std::vector<VkSurfaceFormatKHR>				formats;
		std::vector<VkPresentModeKHR>				present_modes;

		NODISCARD constexpr operator bool() const
		{
			return capabilities.has_value() && !formats.empty() && !present_modes.empty();
		}
	};

public:
	explicit PhysicalDevice(const std::shared_ptr<Instance>& instance);
	~PhysicalDevice() override = default;
	void initialize() override;

	VK_TYPE_INIT(VkPhysicalDevice, m_device);
	VkPhysicalDeviceProperties			m_properties{};
	VkPhysicalDeviceFeatures			m_features{};
	QueueFamilyIndices					m_indices;
	SwapChainSupportDetails				m_support_details;
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
	VK_TYPE_INIT(VkQueue, m_graphics_queue);
	VK_TYPE_INIT(VkQueue, m_present_queue);
};

VK_NAMESPACE_END