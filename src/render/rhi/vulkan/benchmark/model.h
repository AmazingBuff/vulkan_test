#pragma once

#include "buffer.h"
#include "image.h"

ENGINE_NAMESPACE_BEGIN

class VK_CLASS(Model) final : public RHI
{
public:
    VK_CLASS(Model)() = default;
    ~VK_CLASS(Model)() override;
    NODISCARD constexpr RHIFlag flag() const override { return RHIFlag::e_model; }

    void initialize();
    void draw(const std::string_view& name, VkCommandBuffer command_buffer) const;

private:
    struct Mesh
    {
        uint32_t first_index;
        uint32_t index_count;
        uint32_t first_vertex;
        uint32_t vertex_count;
    };

    class Node
    {
    public:
        Node();
        ~Node();

        void draw(VkCommandBuffer command_buffer) const;

    private:
        std::string        name;
        Node*              parent = nullptr;
        std::vector<Node*> children;
        Mesh               mesh;
    };

private:
    std::vector<Node*>                m_root_nodes;
    std::shared_ptr<VK_CLASS(Buffer)> m_vertex_buffer;
    std::shared_ptr<VK_CLASS(Buffer)> m_index_buffer;
};

ENGINE_NAMESPACE_END
