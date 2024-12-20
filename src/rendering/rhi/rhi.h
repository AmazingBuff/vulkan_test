#pragma once
#include "rendering/utils/util.h"

#define RHI_CLASS(api, name)			api##name

ENGINE_NAMESPACE_BEGIN

enum class GraphicsAPI : uint8_t
{
	e_opengl,
	e_vulkan,
	e_directx11,
	e_directx12,
	e_metal
};

enum class RHIFlag : uint8_t
{
	e_instance,
	e_physical_device,
	e_device,
	e_swap_chain,
	e_framebuffer,
	e_pipeline_layout,
	e_render_pass,
	e_pipeline,
	e_command_buffer,
	e_semaphore,
	e_fence,
	e_buffer,
	e_vertex_buffer,
	e_index_buffer,
	e_uniform_buffer,
	e_pipeline_resources,
	e_unknown
};

static constexpr GraphicsAPI Hardware_API = GraphicsAPI::e_vulkan;
static constexpr uint32_t k_Max_Frames_In_Flight = 2;

class RHI
{
public:
	RHI() = default;
	virtual ~RHI() = default;

	template<typename Self, typename... Args>
	void init(this Self&& self, Args&&... args)
	{
		self.initialize(std::forward<Args>(args)...);
	}

	NODISCARD virtual constexpr RHIFlag flag() const = 0;
};

ENGINE_NAMESPACE_END