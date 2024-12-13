#pragma once

#include "rendering/rhi/vulkan/types.h"
#include "rendering/resources/geometry/geometry_data.h"

ENGINE_NAMESPACE_BEGIN

struct VK_CLASS(Vertex) : Vertex
{
	NODISCARD static VkVertexInputBindingDescription get_binding_description();
	NODISCARD static std::array<VkVertexInputAttributeDescription, 3> get_attribute_descriptions();
};





ENGINE_NAMESPACE_END