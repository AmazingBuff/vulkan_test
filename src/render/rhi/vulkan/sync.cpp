#include "sync.h"
#include "system/system.h"
#include "render/renderer.h"
#include "render/drawable.h"

ENGINE_NAMESPACE_BEGIN

VK_CLASS(Semaphore)::~VK_CLASS(Semaphore)()
{
    vkDestroySemaphore(g_system_context->g_render_system->m_drawable->m_device->m_device, m_semaphore, nullptr);
}

void VK_CLASS(Semaphore)::initialize()
{
    VkSemaphoreCreateInfo semaphore_create_info{
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
    };

    VK_CHECK_RESULT(vkCreateSemaphore(g_system_context->g_render_system->m_drawable->m_device->m_device, &semaphore_create_info, nullptr, &m_semaphore));
}



VK_CLASS(Fence)::~VK_CLASS(Fence)()
{
    vkDestroyFence(g_system_context->g_render_system->m_drawable->m_device->m_device, m_fence, nullptr);
}

void VK_CLASS(Fence)::reset() const
{
    VK_CHECK_RESULT(vkResetFences(g_system_context->g_render_system->m_drawable->m_device->m_device, 1, &m_fence));
}

void VK_CLASS(Fence)::wait() const
{
    VK_CHECK_RESULT(vkWaitForFences(g_system_context->g_render_system->m_drawable->m_device->m_device, 1, &m_fence, VK_TRUE, UINT64_MAX));
}

void VK_CLASS(Fence)::initialize()
{
    VkFenceCreateInfo fence_create_info{
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    VK_CHECK_RESULT(vkCreateFence(g_system_context->g_render_system->m_drawable->m_device->m_device, &fence_create_info, nullptr, &m_fence));
}


ENGINE_NAMESPACE_END


