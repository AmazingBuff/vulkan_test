#pragma once

#include "instance.h"
#include "fork/vk_mem_alloc.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(PhysicalDevice) final : public RHI
{
public:
	struct QueueFamilyIndices
	{
		std::optional<uint32_t> graphics_family;
		std::optional<uint32_t> present_family;

		NODISCARD constexpr explicit operator bool() const
		{
			return graphics_family.has_value() && present_family.has_value();
		}

		NODISCARD constexpr bool is_same_family() const
		{
			ASSERT(*this);
			return graphics_family.value() == present_family.value();
		}
	};

public:
	VK_CLASS(PhysicalDevice)() = default;
	~VK_CLASS(PhysicalDevice)() override = default;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_physical_device;
	}

	void initialize();
public:
	VkPhysicalDeviceProperties			m_properties{};
	VkPhysicalDeviceFeatures			m_features{};
	VkPhysicalDeviceMemoryProperties	m_memory_properties{};
	QueueFamilyIndices					m_indices;
private:
	void pick_physical_device();
private:
	VK_TYPE_INIT(VkPhysicalDevice,		m_device);

	friend class VK_CLASS(Device);
	friend class VK_CLASS(SwapChain);
	friend class VK_CLASS(CommandBuffer);
};

class VK_CLASS(Device) final : public RHI
{
public:
	VK_CLASS(Device)() = default;
	~VK_CLASS(Device)() override;
	NODISCARD constexpr RHIFlag flag() const override
	{
		return RHIFlag::e_device;
	}

	void initialize();
	// return false means that a new swap chain need to be created
	NODISCARD bool present() const;
	void wait_idle() const;
private:
	void create_logical_device();
	void create_vma_allocator();
private:
	VK_TYPE_INIT(VkDevice,			m_device);
	VK_TYPE_INIT(VkQueue,			m_graphics_queue);
	VK_TYPE_INIT(VkQueue,			m_present_queue);
	VK_TYPE_INIT(VmaAllocator,		m_allocator);

	friend class VK_CLASS(SwapChain);
	friend class VK_CLASS(PipelineLayout);
	friend class VK_CLASS(DescriptorSet);
	friend class VK_CLASS(RenderPass);
	friend class VK_CLASS(Pipeline);
	friend class VK_CLASS(PipelineResources);
	friend class VK_CLASS(Framebuffer);
	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(Semaphore);
	friend class VK_CLASS(Fence);
	friend class VK_CLASS(Buffer);
	friend class VK_CLASS(UniformBuffer);
	friend class VK_CLASS(Image);
	friend class VK_CLASS(Sampler);
	friend class VK_CLASS(ResourceManager);
};

ENGINE_NAMESPACE_END