#pragma once

#include "types.h"
#include "rendering/resources/geometry/geometry_data.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(Vertex) : public Vertex
{
public:
	static VkVertexInputBindingDescription						m_binding_description;
	static std::array<VkVertexInputAttributeDescription, 3>		m_attribute_descriptions;
};





ENGINE_NAMESPACE_END