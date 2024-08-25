// Tell SDL not to mess with main()
#define SDL_MAIN_HANDLED

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <iostream>
#include <vector>
#include "rendering/rhi/vulkan/utils/util.h"
#include "rendering/rhi/vulkan/device.h"
using namespace VK_NAMESPACE;
using namespace WINDOW_NAMESPACE;

int main()
{
	std::shared_ptr<Window> window = std::make_shared<Window>();
    window->initialize();
    std::shared_ptr<Instance> instance = std::make_shared<Instance>(window);
    instance->initialize();
    std::vector<VkExtensionProperties> extension_properties = vkEnumerateProperties(vkEnumerateInstanceExtensionProperties, nullptr);
    std::shared_ptr<PhysicalDevice> physical_device = std::make_shared<PhysicalDevice>(instance);
    physical_device->initialize();
    std::shared_ptr<Device> device = std::make_shared<Device>(instance, physical_device);
    device->initialize();
    
//
//    // This is where most initializtion for a program should be performed
//
//    // Poll for user input.
//    bool stillRunning = true;
//    while (stillRunning) {
//
//        SDL_Event event;
//        while (SDL_PollEvent(&event)) {
//
//            switch (event.type) {
//
//            case SDL_QUIT:
//                stillRunning = false;
//                break;
//
//            default:
//                // Do nothing.
//                break;
//            }
//        }
//
//        SDL_Delay(10);
//    }
//
//    // Clean up.
//    instance.destroySurfaceKHR(surface);
//    SDL_DestroyWindow(window);
//    SDL_Quit();
//    instance.destroy();

    return 0;
}