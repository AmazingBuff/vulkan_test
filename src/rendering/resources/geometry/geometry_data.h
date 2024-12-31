#pragma once

#include "base/geometry.h"

ENGINE_NAMESPACE_BEGIN

struct Vertex
{
	Vec3f position;
	Vec2f texcoord;
	Vec3f normal;
};

struct Transformation
{
	alignas(16) Affine3f model;
	alignas(16) Affine3f view;
	alignas(16) Affine3f projection;
};

Affine3f look_at(const Vec3f& eye, const Vec3f& target, const Vec3f& up);

Affine3f perspective(Float fov, Float aspect, Float z_near, Float z_far);

ENGINE_NAMESPACE_END