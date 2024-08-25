#pragma once
#include "rendering/utils/util.h"

#define RHI_NAMESPACE_BEGIN				RENDERING_NAMESPACE_BEGIN			\
										NAMESPACE_BEGIN(HardwareInterface)	
#define RHI_NAMESPACE_END				NAMESPACE_END(HardwareInterface)	\
										RENDERING_NAMESPACE_END
#define RHI_NAMESPACE					NAMESPACE_CONCAT(RENDERING_NAMESPACE, NAMESPACE(HardwareInterface))
#define RHI_NAMESPACE_VARIABLE(var)		NAMESPACE_VARIABLE(RHI_NAMESPACE, var)

RHI_NAMESPACE_BEGIN

enum class GraphicsAPI : uint8_t
{
	e_opengl,
	e_vulkan,
	e_directx11,
	e_directx12,
	e_metal
};

static constexpr GraphicsAPI Hardware_API = GraphicsAPI::e_vulkan;
#define HARDWARE_API RHI_NAMESPACE_VARIABLE(Hardware_API)

class RHI
{
public:
	RHI() = default;
	virtual ~RHI() = default;
	virtual void initialize() = 0;
};

RHI_NAMESPACE_END