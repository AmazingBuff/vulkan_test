#pragma once

#ifdef VULKAN_RHI
#include "vulkan/instance.h"
#include "vulkan/device.h"
#include "vulkan/swap_chain.h"
#include "vulkan/pipeline.h"
#include "vulkan/command_buffer.h"

#define RHI_USING_CLASS(name) VK_CLASS(name)

#endif
RENDERING_NAMESPACE_BEGIN

using Instance = RHI_NAMESPACE::RHI_USING_CLASS(Instance);
using PhysicalDevice = RHI_NAMESPACE::RHI_USING_CLASS(PhysicalDevice);
using Device = RHI_NAMESPACE::RHI_USING_CLASS(Device);
using SwapChain = RHI_NAMESPACE::RHI_USING_CLASS(SwapChain);
using PipelineLayout = RHI_NAMESPACE::RHI_USING_CLASS(PipelineLayout);
using RenderPass = RHI_NAMESPACE::RHI_USING_CLASS(RenderPass);
using Pipeline = RHI_NAMESPACE::RHI_USING_CLASS(Pipeline);
using CommandBuffer = RHI_NAMESPACE::RHI_USING_CLASS(CommandBuffer);

RENDERING_NAMESPACE_END

#undef RHI_USING_CLASS