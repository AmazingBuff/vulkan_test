#pragma once

#ifdef VULKAN_RHI
#include "vulkan/instance.h"
#include "vulkan/device.h"
#include "vulkan/swap_chain.h"
#include "vulkan/descriptor_sets.h"
#include "vulkan/pipeline_resources.h"
#include "vulkan/command_buffer.h"
#include "vulkan/sync.h"
#include "vulkan/benchmark/buffer.h"
#endif


#ifdef VULKAN_RHI
#define RHI_USING_CLASS(name) VK_CLASS(name)
#else
#define RHI_USING_CLASS(name)
#endif

ENGINE_NAMESPACE_BEGIN

using Instance              =   RHI_USING_CLASS(Instance);
using PhysicalDevice        =   RHI_USING_CLASS(PhysicalDevice);
using Device                =   RHI_USING_CLASS(Device);
using SwapChain             =   RHI_USING_CLASS(SwapChain);
using PipelineLayout        =   RHI_USING_CLASS(PipelineLayout);
using DescriptorSet         =   RHI_USING_CLASS(DescriptorSet);
using RenderPass            =   RHI_USING_CLASS(RenderPass);
using Pipeline              =   RHI_USING_CLASS(Pipeline);
using PipelineResources     =   RHI_USING_CLASS(PipelineResources);
using CommandBuffer         =   RHI_USING_CLASS(CommandBuffer);
using Semaphore             =   RHI_USING_CLASS(Semaphore);
using Fence                 =   RHI_USING_CLASS(Fence);
using Buffer                =   RHI_USING_CLASS(Buffer);

ENGINE_NAMESPACE_END