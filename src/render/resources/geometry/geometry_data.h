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

ENGINE_NAMESPACE_END

template <>
struct std::hash<ENGINE_NAMESPACE_CONCAT(Vertex)>
{
    NODISCARD size_t operator()(const ENGINE_NAMESPACE_CONCAT(Vertex)& vertex) const
    {
        size_t seed = 0;
        seed = AMAZING_NAMESPACE_CONCAT(hash_combine(seed, hash<AMAZING_NAMESPACE_CONCAT(Vec3f)>()(vertex.position)));
        seed = AMAZING_NAMESPACE_CONCAT(hash_combine(seed, hash<AMAZING_NAMESPACE_CONCAT(Vec2f)>()(vertex.texcoord)));
        seed = AMAZING_NAMESPACE_CONCAT(hash_combine(seed, hash<AMAZING_NAMESPACE_CONCAT(Vec3f)>()(vertex.normal)));
        return seed;
    }
};