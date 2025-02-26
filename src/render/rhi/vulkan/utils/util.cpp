#include "util.h"

ENGINE_NAMESPACE_BEGIN

const char* vk_result_convert(VkResult res)
{
    switch (res)
    {
#define SWITCH_BRANCH(expr) case expr : return #expr
        SWITCH_BRANCH(VK_SUCCESS);
        SWITCH_BRANCH(VK_NOT_READY);
        SWITCH_BRANCH(VK_TIMEOUT);
        SWITCH_BRANCH(VK_EVENT_SET);
        SWITCH_BRANCH(VK_EVENT_RESET);
        SWITCH_BRANCH(VK_INCOMPLETE);
        SWITCH_BRANCH(VK_ERROR_OUT_OF_HOST_MEMORY);
        SWITCH_BRANCH(VK_ERROR_OUT_OF_DEVICE_MEMORY);
        SWITCH_BRANCH(VK_ERROR_INITIALIZATION_FAILED);
        SWITCH_BRANCH(VK_ERROR_DEVICE_LOST);
        SWITCH_BRANCH(VK_ERROR_MEMORY_MAP_FAILED);
        SWITCH_BRANCH(VK_ERROR_LAYER_NOT_PRESENT);
        SWITCH_BRANCH(VK_ERROR_EXTENSION_NOT_PRESENT);
        SWITCH_BRANCH(VK_ERROR_FEATURE_NOT_PRESENT);
        SWITCH_BRANCH(VK_ERROR_INCOMPATIBLE_DRIVER);
        SWITCH_BRANCH(VK_ERROR_TOO_MANY_OBJECTS);
        SWITCH_BRANCH(VK_ERROR_FORMAT_NOT_SUPPORTED);
        SWITCH_BRANCH(VK_ERROR_FRAGMENTED_POOL);
        SWITCH_BRANCH(VK_ERROR_UNKNOWN);
        SWITCH_BRANCH(VK_ERROR_OUT_OF_POOL_MEMORY);
        SWITCH_BRANCH(VK_ERROR_INVALID_EXTERNAL_HANDLE);
        SWITCH_BRANCH(VK_ERROR_FRAGMENTATION);
        SWITCH_BRANCH(VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
        SWITCH_BRANCH(VK_PIPELINE_COMPILE_REQUIRED);
        SWITCH_BRANCH(VK_ERROR_SURFACE_LOST_KHR);
        SWITCH_BRANCH(VK_ERROR_NATIVE_WINDOW_IN_USE_KHR);
        SWITCH_BRANCH(VK_SUBOPTIMAL_KHR);
        SWITCH_BRANCH(VK_ERROR_OUT_OF_DATE_KHR);
        SWITCH_BRANCH(VK_ERROR_INCOMPATIBLE_DISPLAY_KHR);
        SWITCH_BRANCH(VK_ERROR_VALIDATION_FAILED_EXT);
        SWITCH_BRANCH(VK_ERROR_INVALID_SHADER_NV);
        SWITCH_BRANCH(VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR);
        SWITCH_BRANCH(VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR);
        SWITCH_BRANCH(VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR);
        SWITCH_BRANCH(VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR);
        SWITCH_BRANCH(VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR);
        SWITCH_BRANCH(VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR);
        SWITCH_BRANCH(VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT);
        SWITCH_BRANCH(VK_ERROR_NOT_PERMITTED_KHR);
        SWITCH_BRANCH(VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT);
        SWITCH_BRANCH(VK_THREAD_IDLE_KHR);
        SWITCH_BRANCH(VK_THREAD_DONE_KHR);
        SWITCH_BRANCH(VK_OPERATION_DEFERRED_KHR);
        SWITCH_BRANCH(VK_OPERATION_NOT_DEFERRED_KHR);
        SWITCH_BRANCH(VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR);
        SWITCH_BRANCH(VK_ERROR_COMPRESSION_EXHAUSTED_EXT);
        SWITCH_BRANCH(VK_INCOMPATIBLE_SHADER_BINARY_EXT);
        SWITCH_BRANCH(VK_RESULT_MAX_ENUM);
#undef SWITCH_BRANCH
    default:
        return nullptr;
    }
}


SwapChainSupportDetails query_swap_chain_support(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
{
    SwapChainSupportDetails support_details;
    VkSurfaceCapabilitiesKHR capabilities;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &capabilities));
    support_details.capabilities = capabilities;
    support_details.formats = vkEnumerateProperties(vkGetPhysicalDeviceSurfaceFormatsKHR, physical_device, surface);
    support_details.present_modes = vkEnumerateProperties(vkGetPhysicalDeviceSurfacePresentModesKHR, physical_device, surface);
    return support_details;
}

ENGINE_NAMESPACE_END