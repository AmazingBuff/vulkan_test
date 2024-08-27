#pragma once

#include "instance.h"
#include "device.h"
#include "swap_chain.h"

RHI_NAMESPACE_BEGIN

using Instance = VK_NAMESPACE::VK_CLASS(Instance);
using PhysicalDevice = VK_NAMESPACE::VK_CLASS(PhysicalDevice);
using Device = VK_NAMESPACE::VK_CLASS(Device);
using SwapChain = VK_NAMESPACE::VK_CLASS(SwapChain);

RHI_NAMESPACE_END