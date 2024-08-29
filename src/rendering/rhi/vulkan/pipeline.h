#pragma once

#include "types.h"
#include "utils/util.h"

RHI_NAMESPACE_BEGIN


class VK_CLASS(Pipeline) final : public RHI
{
public:
	VK_CLASS(Pipeline)() = default;
	~VK_CLASS(Pipeline)();
	void initialize() override;

	NODISCARD constexpr RHIFlag flag() const override;
private:
	void create_pipeline();
};


RHI_NAMESPACE_END