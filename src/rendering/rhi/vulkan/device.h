#pragma once

#include "instance.h"

RHI_NAMESPACE_BEGIN

class VK_CLASS(PhysicalDevice) final : public RHI
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

	struct SwapChainDetails
	{
		std::optional<VkSurfaceFormatKHR>		format;
		std::optional<VkPresentModeKHR>			present_mode;
		std::optional<VkExtent2D>				extent;

		NODISCARD constexpr operator bool() const
		{
			return format.has_value() && present_mode.has_value() && extent.has_value();
		}
	};
public:
	VK_CLASS(PhysicalDevice)() = default;
	~VK_CLASS(PhysicalDevice)() override = default;
	void initialize() override;
	NODISCARD constexpr RHIFlag flag() const override;
public:
	VkPhysicalDeviceProperties			m_properties{};
	VkPhysicalDeviceFeatures			m_features{};
	QueueFamilyIndices					m_indices;
	SwapChainSupportDetails				m_support_details;
	SwapChainDetails					m_swap_chain_details;
private:
	void pick_physical_device();
	void choose_swap_chain_details();
private:
	VK_TYPE_INIT(VkPhysicalDevice,		m_device);

	friend class VK_CLASS(Device);
	friend class VK_CLASS(SwapChain);
};

class VK_CLASS(Device) final : public RHI
{
public:
	VK_CLASS(Device)() = default;
	~VK_CLASS(Device)() override;
	void initialize() override;
	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_logical_device();
private:
	VK_TYPE_INIT(VkDevice,		m_device);
	VK_TYPE_INIT(VkQueue,		m_graphics_queue);
	VK_TYPE_INIT(VkQueue,		m_present_queue);

	friend class VK_CLASS(SwapChain);
};

RHI_NAMESPACE_END