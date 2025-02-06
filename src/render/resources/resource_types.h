#pragma once

#include "geometry/geometry_data.h"

ENGINE_NAMESPACE_BEGIN

struct PipelineLayoutInfo;
struct RenderPassInfo;
struct PipelineInfo;

struct ShaderResource
{
    std::shared_ptr<std::vector<char>> vertex_shader;
    std::shared_ptr<std::vector<char>> fragment_shader;
    // more for geometry shader, tessellation shader, compute shader, etc.

    NODISCARD explicit operator bool() const
    {
        return vertex_shader && fragment_shader && !vertex_shader->empty() && !fragment_shader->empty();
    }
};

struct PipelineLayoutResource
{
    std::shared_ptr<PipelineLayoutInfo> pipeline_layout;
};

struct RenderPassResource
{
    std::shared_ptr<RenderPassInfo> render_pass;
};

struct PipelineResource
{
    std::shared_ptr<PipelineInfo> pipeline;
};

struct TextureResource
{
    int      width = 0;
    int      height = 0;
    int      channels = 0;
    uint8_t* data = nullptr;

    NODISCARD explicit operator bool() const { return width > 0 && height > 0 && channels > 0 && data != nullptr; }
};

struct ModelResource
{
    std::vector<Vertex>                                                 vertices;
    std::vector<uint32_t>                                               indices;
    std::unordered_map<std::string, std::shared_ptr<TextureResource>>   textures;

    NODISCARD explicit operator bool() const { return !vertices.empty(); }
};

ENGINE_NAMESPACE_END