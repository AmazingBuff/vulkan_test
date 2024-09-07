#pragma once

#include "rendering/render_types.h"

#include "base/logger.h"

ENGINE_NAMESPACE_BEGIN

#define RENDERING_LOG_INFO(message)			LOG_INFO("Rendering", message)
#define RENDERING_LOG_WARNING(message)		LOG_WARNING("Rendering", message)
#define RENDERING_LOG_ERROR(message)		LOG_ERROR("Rendering", message)

ENGINE_NAMESPACE_END