#pragma once

#if defined(_WIN32)
#define NOMINMAX
#define VK_USE_PLATFORM_WIN32_KHR
#endif

#include "fork/volk.h"
#include "rendering/rhi/rhi.h"

#define VK_TYPE_INIT(type, name)		type name = VK_NULL_HANDLE
#define VK_CLASS(name)					RHI_CLASS(Vulkan, name)

ENGINE_NAMESPACE_BEGIN

static const char* Shader_Compiler = "glslc.exe";
static const char* Shader_Compiler_Path = "D:/env/Vulkan/1.3.290.0/Bin/glslc.exe";

static const char* Application_Name = "Renderer";
static constexpr uint32_t Application_Version = VK_MAKE_VERSION(1, 0, 0);
static const char* Engine_Name = "Amazing";
static constexpr uint32_t Engine_Version = VK_MAKE_VERSION(1, 0, 0);
static constexpr uint32_t Vulkan_Version = VK_API_VERSION_1_0;

#if defined(_DEBUG) || defined(DEBUG)
static const char* Validation_Layers = "VK_LAYER_KHRONOS_validation";
#endif

#if defined(_WIN32) || defined(WIN32) 
static const std::vector<const char*> Platform_Extensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
#endif

static const std::vector<const char*> Device_Extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

// buffer
static constexpr size_t Default_Buffer_Size = 65536;


class VK_CLASS(Instance);
class VK_CLASS(PhysicalDevice);
class VK_CLASS(Device);
class VK_CLASS(SwapChain);
class VK_CLASS(PipelineLayout);
class VK_CLASS(DescriptorSet);
class VK_CLASS(RenderPass);
class VK_CLASS(Pipeline);
class VK_CLASS(PipelineResources);
class VK_CLASS(Framebuffer);
class VK_CLASS(CommandBuffer);
class VK_CLASS(Semaphore);
class VK_CLASS(Fence);
class VK_CLASS(Buffer);
class VK_CLASS(UniformBuffer);
class VK_CLASS(Image);
class VK_CLASS(ResourceManager);

ENGINE_NAMESPACE_END