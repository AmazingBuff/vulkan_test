#include "pipeline_resources.h"
#include "system/system.h"
#include "camera/camera.h"
#include "trans/enum_trans.h"
#include "trans/structure_trans.h"
#include "benchmark/resource_manager.h"
#include "render/renderer.h"
#include "render/drawable.h"
#include "render/render_resouces.h"
#include "render/resources/resource_types.h"


ENGINE_NAMESPACE_BEGIN


void VK_CLASS(PipelineResources)::initialize()
{
    create_pipeline_layout("basic");
    create_render_pass("basic");
    create_pipeline("basic");
    create_resource_manager();
}

const std::shared_ptr<VK_CLASS(Pipeline)>& VK_CLASS(PipelineResources)::get_pipeline(const std::string_view& name) const
{
    return m_pipelines.at(name.data());
}

const std::shared_ptr<VK_CLASS(PipelineLayout)>& VK_CLASS(PipelineResources)::get_pipeline_layout(const std::string_view& name) const
{
    return m_pipeline_layouts.at(name.data());
}

const std::shared_ptr<VK_CLASS(DescriptorSet)>& VK_CLASS(PipelineResources)::get_descriptor_set(const std::string_view& name) const
{
    return m_descriptor_sets.at(name.data());
}

const std::shared_ptr<VK_CLASS(RenderPass)>& VK_CLASS(PipelineResources)::get_render_pass(const std::string_view& name) const
{
    return m_render_passes.at(name.data());
}

const std::shared_ptr<VK_CLASS(Buffer)>& VK_CLASS(PipelineResources)::get_vertex_buffer() const
{
    return m_resource_manager->m_vertex_buffer;
}

const std::shared_ptr<VK_CLASS(Buffer)>& VK_CLASS(PipelineResources)::get_index_buffer() const
{
    return m_resource_manager->m_index_buffer;
}

const BufferInfo& VK_CLASS(PipelineResources)::get_vertex_info(const std::string_view& name) const
{
    return m_resource_manager->m_vertex_buffer->get_info(name);
}

const BufferInfo& VK_CLASS(PipelineResources)::get_index_info(const std::string_view& name) const
{
    return m_resource_manager->m_index_buffer->get_info(name);
}

void VK_CLASS(PipelineResources)::create_render_pass(const std::string& name)
{
    auto& render_pass_info =
        g_system_context->g_render_system->m_render_resources->get_render_pass_resource(name)->render_pass;

    std::shared_ptr<VK_CLASS(RenderPass)> render_pass = std::make_shared<VK_CLASS(RenderPass)>();
    render_pass->init(render_pass_info);
    m_render_passes.emplace(name, render_pass);
}

void VK_CLASS(PipelineResources)::create_pipeline_layout(const std::string& name)
{
    auto& pipeline_info =
        g_system_context->g_render_system->m_render_resources->get_pipeline_layout_resource(name)->pipeline_layout;

    std::shared_ptr<VK_CLASS(PipelineLayout)> layout = std::make_shared<VK_CLASS(PipelineLayout)>();
    layout->init(pipeline_info);

    std::shared_ptr<VK_CLASS(DescriptorSet)> descriptor_set = std::make_shared<VK_CLASS(DescriptorSet)>();
    descriptor_set->init(layout);

    m_descriptor_sets.emplace(name, descriptor_set);
    m_pipeline_layouts.emplace(name, layout);
}

void VK_CLASS(PipelineResources)::create_pipeline(const std::string& name)
{
    auto& pipeline_info = g_system_context->g_render_system->m_render_resources->get_pipeline_resource(name)->pipeline;
    auto layout_it = m_pipeline_layouts.find(pipeline_info->layout);
    if (layout_it == m_pipeline_layouts.end())
    {
        RENDERING_LOG_ERROR("pipeline layout not found");
        return;
    }
    auto& layout = layout_it->second;

    auto render_pass_it = m_render_passes.find(pipeline_info->render_pass);
    if (render_pass_it == m_render_passes.end())
    {
        RENDERING_LOG_ERROR("render pass not found");
        return;
    }
    auto& render_pass = render_pass_it->second;

    std::shared_ptr<VK_CLASS(Pipeline)> pipeline = std::make_shared<VK_CLASS(Pipeline)>();
    pipeline->init(pipeline_info, layout, render_pass);
    m_pipelines.emplace(name, pipeline);
}

void VK_CLASS(PipelineResources)::create_resource_manager()
{
    m_resource_manager = std::make_shared<VK_CLASS(ResourceManager)>();
    m_resource_manager->init();

    auto& model = g_system_context->g_render_system->m_render_resources->get_model_resource("viking_room");

    m_resource_manager->map_model("basic", model, false);

    static const std::unordered_map<std::string, std::string> name_to_res_name_map_1 =
    {
        {"sampler_s", "a"}
    };

    static const std::unordered_map<std::string, std::string> name_to_res_name_map_2 =
    {
        {"sampler_s", "b"}
    };

    for (auto& [name, pipeline_layout] : m_pipeline_layouts)
    {
        auto& uniform_buffers = pipeline_layout->m_shader_resources_layout.uniform_buffers;
        m_resource_manager->configure_uniform_buffer(name, uniform_buffers, m_descriptor_sets.at(name)->m_descriptor_sets);

        auto& sampled_images = pipeline_layout->m_shader_resources_layout.sampled_images;
        m_resource_manager->configure_image(sampled_images, name_to_res_name_map_1, m_descriptor_sets.at(name)->m_descriptor_sets);
    }
}

void VK_CLASS(PipelineResources)::update_uniform_buffer(const std::string_view& name, const GlobalRuntimeInfo& info) const
{
    uint32_t current_image = g_system_context->g_render_system->m_drawable->m_command_buffer->m_current_frame;
    VkExtent2D extent = g_system_context->g_render_system->m_drawable->m_swap_chain->m_details.extent.value();

    auto camera = g_system_context->g_camera_system;

    Transformation transformation;
    transformation.model = Eigen::Translation3f::Identity() * Eigen::AngleAxisf(info.current_time, Eigen::Vector3f::UnitZ());
    transformation.view = camera->look_at();
    transformation.projection = camera->perspective(static_cast<float>(extent.width) / static_cast<float>(extent.height));
    transformation.projection(1, 1) *= -1.0;

    m_resource_manager->map_uniform_buffer("basic", "trans", current_image, transformation);
    m_resource_manager->map_uniform_buffer("basic", "sp", current_image, info.current_time);
}


ENGINE_NAMESPACE_END