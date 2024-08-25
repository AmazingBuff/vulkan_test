#pragma once
#include "base/macro.h"


NAMESPACE_BEGIN(Amazing)
NAMESPACE_BEGIN(Rendering)
NAMESPACE_BEGIN(HardwareInterface)

class RHI
{
public:
	RHI() = default;
	virtual ~RHI() = default;
	virtual void initialize() = 0;
};

NAMESPACE_END(HardwareInterface)
NAMESPACE_END(Rendering)
NAMESPACE_END(Amazing)