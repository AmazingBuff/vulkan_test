#pragma once

#include "base/geometry.h"
#include "base/util.h"

ENGINE_NAMESPACE_BEGIN

struct Vertex
{
	Vec3f position;
	Vec2f texcoord;
	Vec3f normal;

	NODISCARD bool operator==(const Vertex& other) const
	{
		return position == other.position && texcoord == other.texcoord && normal == other.normal;
	}
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

template <>
struct std::hash<Amazing::Engine::Vertex>
{
    NODISCARD size_t operator()(const Amazing::Engine::Vertex& vertex) const
    {
		size_t seed = 0;
		seed = Amazing::hash_combine(seed, hash<Amazing::Vec3f>()(vertex.position));
		seed = Amazing::hash_combine(seed, hash<Amazing::Vec2f>()(vertex.texcoord));
		seed = Amazing::hash_combine(seed, hash<Amazing::Vec3f>()(vertex.normal));
		return seed;
    }
};