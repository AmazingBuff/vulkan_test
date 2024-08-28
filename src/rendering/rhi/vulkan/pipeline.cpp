#include "pipeline.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/rhi/drawable.h"


VK_NAMESPACE_BEGIN

VK_CLASS(Pipeline)::~VK_CLASS(Pipeline)
{

}

void VK_CLASS(Pipeline)::initialize()
{

}


NODISCARD constexpr RHIFlag VK_CLASS(Pipeline)::flag() const
{
	return RHIFlag::e_pipeline;
}

VK_NAMESPACE_END


