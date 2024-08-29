#include "pipeline.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"


RHI_NAMESPACE_BEGIN

VK_CLASS(Pipeline)::~VK_CLASS(Pipeline)()
{

}

void VK_CLASS(Pipeline)::initialize()
{
	create_pipeline();
}

NODISCARD constexpr RHIFlag VK_CLASS(Pipeline)::flag() const
{
	return RHIFlag::e_pipeline;
}

void VK_CLASS(Pipeline)::create_pipeline()
{

}

RHI_NAMESPACE_END


