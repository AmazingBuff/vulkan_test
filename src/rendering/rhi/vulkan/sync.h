#pragma once


#include "types.h"
#include "utils/util.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(Semaphore) final : public RHI
{
public:
	VK_CLASS(Semaphore)() = default;
	~VK_CLASS(Semaphore)() override;
	void initialize() override;

	NODISCARD constexpr RHIFlag flag() const override;
private:
	VK_TYPE_INIT(VkSemaphore, m_semaphore);

	friend class VK_CLASS(SwapChain);
	friend class VK_CLASS(CommandBuffer);
	friend class VK_CLASS(Device);
};


class VK_CLASS(Fence) final : public RHI
{
public:
	VK_CLASS(Fence)() = default;
	~VK_CLASS(Fence)() override;
	void initialize() override;
	void wait() const;
	void reset() const;

	NODISCARD constexpr RHIFlag flag() const override;
private:
	VK_TYPE_INIT(VkFence, m_fence);

	friend class VK_CLASS(CommandBuffer);
};

ENGINE_NAMESPACE_END