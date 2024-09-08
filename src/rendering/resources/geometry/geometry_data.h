#pragma once

#include "base/macro.h"
#include "system/structure.h"
#include <Eigen/Core>

ENGINE_NAMESPACE_BEGIN

using Vec2 = Eigen::Vector2<Float>;
using Vec3 = Eigen::Vector3<Float>;
using Vec4 = Eigen::Vector4<Float>;

struct Vertex
{
	Vec3 position;
	Vec2 texcoord;
	Vec3 normal;
};


ENGINE_NAMESPACE_END