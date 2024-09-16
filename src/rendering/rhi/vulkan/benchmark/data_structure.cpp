#include "data_structure.h"

ENGINE_NAMESPACE_BEGIN

VkVertexInputBindingDescription VK_CLASS(Vertex)::get_binding_description()
{
	VkVertexInputBindingDescription binding_description{
		.binding = 0,
		.stride = sizeof(Vertex),
		.inputRate = VK_VERTEX_INPUT_RATE_VERTEX
	};

	return binding_description;
}

std::array<VkVertexInputAttributeDescription, 3> VK_CLASS(Vertex)::get_attribute_descriptions()
{
	std::array<VkVertexInputAttributeDescription, 3> attribute_descriptions{
		VkVertexInputAttributeDescription{
			.location = 0,
			.binding = 0,
#ifdef HIGH_PRECISION_FLOAT
			.format = VK_FORMAT_R64G64B64_SFLOAT,
#else
			.format = VK_FORMAT_R32G32B32_SFLOAT,
#endif
			.offset = offsetof(Vertex, position)
		},
		VkVertexInputAttributeDescription{
			.location = 1,
			.binding = 0,
#ifdef HIGH_PRECISION_FLOAT
			.format = VK_FORMAT_R64G64_SFLOAT,
#else
			.format = VK_FORMAT_R32G32_SFLOAT,
#endif
			.offset = offsetof(Vertex, texcoord)
		},
		VkVertexInputAttributeDescription{
			.location = 2,
			.binding = 0,
#ifdef HIGH_PRECISION_FLOAT
			.format = VK_FORMAT_R64G64B64_SFLOAT,
#else
			.format = VK_FORMAT_R32G32B32_SFLOAT,
#endif
			.offset = offsetof(Vertex, normal)
		}
	};

	return attribute_descriptions;
}

ENGINE_NAMESPACE_END
