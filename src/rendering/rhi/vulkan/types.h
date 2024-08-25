#pragma once

#if defined(_WIN32)
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "utils/volk.h"
#include "rendering/rhi/rhi.h"

#define VK_NAMESPACE_BEGIN				RHI_NAMESPACE_BEGIN			\
										NAMESPACE_BEGIN(Vulkan)

#define VK_NAMESPACE_END				NAMESPACE_END(Vulkan)		\
										RHI_NAMESPACE_END
#define VK_NAMESPACE 					NAMESPACE_CONCAT(RHI_NAMESPACE, NAMESPACE(Vulkan))
#define VK_NAMESPACE_VARIABLE(var)		NAMESPACE_VARIABLE(VK_NAMESPACE, var)

#define VK_TYPE_INIT(type, name)	type name = VK_NULL_HANDLE

VK_NAMESPACE_BEGIN

static const char* Application_Name = "Renderer";
static constexpr uint32_t Application_Version = VK_MAKE_VERSION(1, 0, 0);
static const char* Engine_Name = "Amazing";
static constexpr uint32_t Engine_Version = VK_MAKE_VERSION(1, 0, 0);
static constexpr uint32_t Vulkan_Version = VK_API_VERSION_1_3;

#if defined(_DEBUG) || defined(DEBUG)
static const char* Validation_Layers = "VK_LAYER_KHRONOS_validation";
#endif

#if defined(_WIN32) || defined(WIN32) 
static const std::vector<const char*> Platform_Extensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
#endif

VK_NAMESPACE_END