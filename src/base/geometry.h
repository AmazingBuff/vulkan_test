//
// Created by AmazingBuff on 2024/12/20.
//

#pragma once

#include "macro.h"
#include <Eigen/core>
#include <Eigen/Geometry>

AMAZING_NAMESPACE_BEGIN

using Vec2f = Eigen::Vector2<Float>;
using Vec3f = Eigen::Vector3<Float>;
using Vec4f = Eigen::Vector4<Float>;

using Vec2i = Eigen::Vector2<int32_t>;
using Vec2u = Eigen::Vector2<uint32_t>;

using Mat3f = Eigen::Matrix<Float, 3, 3>;
using Mat4f = Eigen::Matrix<Float, 4, 4>;

using Affine3f = Eigen::Affine3f;


AMAZING_NAMESPACE_END
