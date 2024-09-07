#pragma once

#include "types.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(CommandBuffer) final : public RHI
{
public:
	VK_CLASS(CommandBuffer)() = default;
	~VK_CLASS(CommandBuffer)() override;
	void initialize() override;
	void begin_record_command() const;
	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_command_pool();
	void create_command_buffer();
private:
	VK_TYPE_INIT(VkCommandPool,		m_command_pool);
	VK_TYPE_INIT(VkCommandBuffer,	m_command_buffer);
};


ENGINE_NAMESPACE_END