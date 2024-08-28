#pragma once

#include "types.h"
#include "utils/util.h"

VK_NAMESPACE_BEGIN


class VK_CLASS(Pipeline) final : public RHI
{
public:
	VK_CLASS(Pipeline)() = default;
	~VK_CLASS(Pipeline)();
	void initialize() override;

	NODISCARD constexpr RHIFlag flag() const override;
};


VK_NAMESPACE_END