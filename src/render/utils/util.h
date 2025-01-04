#pragma once

#include "render/render_types.h"

#include "base/logger.h"

ENGINE_NAMESPACE_BEGIN

#define RENDERING_LOG_INFO(...)			LOG_INFO("Rendering", __VA_ARGS__)
#define RENDERING_LOG_WARNING(...)		LOG_WARNING("Rendering", __VA_ARGS__)
#define RENDERING_LOG_ERROR(...)		LOG_ERROR("Rendering", __VA_ARGS__)

ENGINE_NAMESPACE_END