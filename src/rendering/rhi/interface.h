#pragma once

#include "vulkan/instance.h"
#include "vulkan/device.h"
#include "vulkan/swap_chain.h"
#include "vulkan/pipeline.h"

RENDERING_NAMESPACE_BEGIN

using Instance = RHI_NAMESPACE::VK_CLASS(Instance);
using PhysicalDevice = RHI_NAMESPACE::VK_CLASS(PhysicalDevice);
using Device = RHI_NAMESPACE::VK_CLASS(Device);
using SwapChain = RHI_NAMESPACE::VK_CLASS(SwapChain);
using Pipeline = RHI_NAMESPACE::VK_CLASS(Pipeline);

RENDERING_NAMESPACE_END