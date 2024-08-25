#pragma once

#include "window/window.h"
#include "base/logger.h"

WINDOW_NAMESPACE_BEGIN

#define WINDOW_LOG_INFO(message)		LOG_INFO("Window", message)
#define WINDOW_LOG_WARNING(message)		LOG_WARNING("Window", message)
#define WINDOW_LOG_ERROR(message)		LOG_ERROR("Window", message)

WINDOW_NAMESPACE_END