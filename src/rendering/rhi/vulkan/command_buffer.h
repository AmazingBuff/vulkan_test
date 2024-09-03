#pragma once

#include "types.h"

RHI_NAMESPACE_BEGIN

class VK_CLASS(CommandBuffer) final : public RHI
{
public:
	VK_CLASS(CommandBuffer)() = default;
	~VK_CLASS(CommandBuffer)() override;
	void initialize() override;
	void begin_record_command() const;
private:
	void create_command_pool();
	void create_command_buffer();
private:
	VK_TYPE_INIT(VkCommandPool,		m_command_pool);
	VK_TYPE_INIT(VkCommandBuffer,	m_command_buffer);
};


RHI_NAMESPACE_END