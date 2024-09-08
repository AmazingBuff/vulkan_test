#pragma once

#ifdef VULKAN_RHI
#include "vulkan/instance.h"
#include "vulkan/device.h"
#include "vulkan/swap_chain.h"
#include "vulkan/pipeline.h"
#include "vulkan/command_buffer.h"
#include "vulkan/sync.h"
#endif


#ifdef VULKAN_RHI
#define RHI_USING_CLASS(name) VK_CLASS(name)
#else
#define RHI_USING_CLASS(name)
#endif

ENGINE_NAMESPACE_BEGIN

using Instance = RHI_USING_CLASS(Instance);
using PhysicalDevice = RHI_USING_CLASS(PhysicalDevice);
using Device = RHI_USING_CLASS(Device);
using SwapChain = RHI_USING_CLASS(SwapChain);
using PipelineLayout = RHI_USING_CLASS(PipelineLayout);
using RenderPass = RHI_USING_CLASS(RenderPass);
using Pipeline = RHI_USING_CLASS(Pipeline);
using CommandBuffer = RHI_USING_CLASS(CommandBuffer);
using Semaphore = RHI_USING_CLASS(Semaphore);
using Fence = RHI_USING_CLASS(Fence);

ENGINE_NAMESPACE_END