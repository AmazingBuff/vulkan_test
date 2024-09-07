// Tell SDL not to mess with main()
#define SDL_MAIN_HANDLED

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <iostream>
#include <vector>
#include "system/system.h"

int main()
{
    ENGINE_NAMESPACE::g_system_context->initialize();

    return 0;
}