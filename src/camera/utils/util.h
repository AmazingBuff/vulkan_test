#pragma once

#include "camera/camera.h"
#include "base/logger.h"

ENGINE_NAMESPACE_BEGIN

#define CAMERA_LOG_INFO(message)		LOG_INFO("Camera", message)
#define CAMERA_LOG_WARNING(message)		LOG_WARNING("Camera", message)
#define CAMERA_LOG_ERROR(message)		LOG_ERROR("Camera", message)

ENGINE_NAMESPACE_END