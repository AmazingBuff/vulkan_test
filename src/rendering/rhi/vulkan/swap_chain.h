#pragma once

#include "types.h"
#include "utils/util.h"

VK_NAMESPACE_BEGIN

class VK_CLASS(SwapChain) final : public RHI
{
public:


	NODISCARD constexpr RHIFlag flag() const override;
};

VK_NAMESPACE_END