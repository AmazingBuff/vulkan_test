#include "model.h"

ENGINE_NAMESPACE_BEGIN

VK_CLASS(Model)::Node::Node() {}

VK_CLASS(Model)::Node::~Node()
{
    for (auto& child : children)
        delete child;
}

void VK_CLASS(Model)::Node::draw(VkCommandBuffer command_buffer) const
{
    vkCmdDrawIndexed(command_buffer, mesh.index_count, 1, mesh.first_index, 0, 0);
    for (auto& child : children)
        child->draw(command_buffer);
}

VK_CLASS(Model)::~VK_CLASS(Model)()
{
    for (auto& node : m_root_nodes)
        delete node;
}

void VK_CLASS(Model)::initialize() {}

void VK_CLASS(Model)::draw(const std::string_view& name, VkCommandBuffer command_buffer) const
{
    auto& vertex_info = m_vertex_buffer->get_info(name);
    auto& index_info = m_index_buffer->get_info(name);

    vkCmdBindVertexBuffers(command_buffer, 0, 1, &m_vertex_buffer->m_buffer, &vertex_info.offset);
    vkCmdBindIndexBuffer(command_buffer, m_index_buffer->m_buffer, index_info.offset, VK_INDEX_TYPE_UINT32);

    for (auto& node : m_root_nodes)
        node->draw(command_buffer);
}


ENGINE_NAMESPACE_END
