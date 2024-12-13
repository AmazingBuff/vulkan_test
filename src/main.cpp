#include "system/system.h"

using namespace ENGINE_NAMESPACE;

int main()
{
    g_system_context->initialize();
    g_system_context->run();

    return 0;
}