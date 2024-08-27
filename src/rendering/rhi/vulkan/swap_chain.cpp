#include "swap_chain.h"


VK_NAMESPACE_BEGIN

constexpr RHIFlag VK_CLASS(SwapChain)::flag() const
{
	return RHIFlag::e_swap_chain;
}


VK_NAMESPACE_END