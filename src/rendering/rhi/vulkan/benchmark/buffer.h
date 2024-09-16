#pragma once

#include "data_structure.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(Buffer) : public RHI
{
public:
	VK_CLASS(Buffer)() = default;
	~VK_CLASS(Buffer)() override;
	void initialize() override;
	NODISCARD constexpr RHIFlag flag() const override;

protected:
	VK_TYPE_INIT(VkBuffer, m_buffer);
	VK_TYPE_INIT(VkDeviceMemory, m_buffer_memory);
};


class VK_CLASS(VertexBuffer) : public VK_CLASS(Buffer)
{
public:
	VK_CLASS(VertexBuffer)() = default;
	~VK_CLASS(VertexBuffer)() override;
	void initialize() override;
	NODISCARD constexpr RHIFlag flag() const override;

private:
	std::vector<VK_CLASS(Vertex)> m_vertices;
};



ENGINE_NAMESPACE_END