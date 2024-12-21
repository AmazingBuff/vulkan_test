#include "pipeline.h"
#include "benchmark/data_structure.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"
#include "rendering/render_resouces.h"
#include "rendering/rhi/vulkan/trans/enum_trans.h"
#include "rendering/rhi/vulkan/trans/structure_trans.h"
#include "rendering/resources/shader/shader_manager.h"
#include "rendering/resources/render_pass/render_pass_manager.h"
#include "rendering/resources/pipeline/pipeline_manager.h"

ENGINE_NAMESPACE_BEGIN

static VkShaderModule create_shader_module(VkDevice device, const std::shared_ptr<std::vector<char>>& code)
{
	VkShaderModuleCreateInfo create_info{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code->size(),
		.pCode = reinterpret_cast<const uint32_t*>(code->data())
	};

	VkShaderModule shader_module;
	VK_CHECK_RESULT(vkCreateShaderModule(device, &create_info, nullptr, &shader_module));
	return shader_module;
}

static VkRenderPass transfer_render_pass(VkDevice device, const RenderPassResource& render_pass_resource)
{
	std::vector<VkAttachmentDescription> attachments;
	std::vector<VkSubpassDescription> subpasses;
	std::vector<VkSubpassDependency> dependencies;

    auto& render_pass = render_pass_resource.render_pass;
	for (auto& attachment : render_pass->attachments.descriptions)
	{
		VkAttachmentDescription description{
			.format			= static_cast<VkFormat>(static_cast<int>(attachment.format)),
			.samples		= static_cast<VkSampleCountFlagBits>(static_cast<int>(attachment.samples)),
			.loadOp			= static_cast<VkAttachmentLoadOp>(static_cast<int>(attachment.load_op)),
			.storeOp		= static_cast<VkAttachmentStoreOp>(static_cast<int>(attachment.store_op)),
			.stencilLoadOp	= static_cast<VkAttachmentLoadOp>(static_cast<int>(attachment.stencil_load_op)),
			.stencilStoreOp = static_cast<VkAttachmentStoreOp>(static_cast<int>(attachment.stencil_store_op)),
			.initialLayout	= static_cast<VkImageLayout>(static_cast<int>(attachment.initial_layout)),
			.finalLayout	= static_cast<VkImageLayout>(static_cast<int>(attachment.final_layout)),
		};
		attachments.emplace_back(description);
	}

	for (auto& dependency : render_pass->subpasses.dependencies)
	{
		VkSubpassDependency depend{
			.srcSubpass = dependency.src_subpass,
			.dstSubpass = dependency.dst_subpass,
			.srcStageMask = static_cast<VkPipelineStageFlags>(static_cast<int>(dependency.src_stage_mask)),
			.dstStageMask = static_cast<VkPipelineStageFlags>(static_cast<int>(dependency.dst_stage_mask)),
			.srcAccessMask = static_cast<VkAccessFlags>(static_cast<int>(dependency.src_access_mask)),
			.dstAccessMask = static_cast<VkAccessFlags>(static_cast<int>(dependency.dst_access_mask)),
			.dependencyFlags = static_cast<VkDependencyFlags>(static_cast<int>(dependency.dependency_flags)),
		};
		dependencies.emplace_back(depend);
	}

	struct VkAttachmentReferences
	{
		std::vector<VkAttachmentReference> input_attachments;
		std::vector<VkAttachmentReference> color_attachments;
		std::vector<VkAttachmentReference> resolve_attachments;
		VkAttachmentReference depth_stencil_attachment{};
	};
	// make sure VkSubpassDescription must be valid before create render pass
	std::vector<VkAttachmentReferences> references(render_pass->subpasses.descriptions.size());
	for (size_t i = 0; i < render_pass->subpasses.descriptions.size(); ++i)
	{
		auto& subpass = render_pass->subpasses.descriptions[i];
		auto& reference = references[i];

		for (auto& input : subpass.input_attachments)
			reference.input_attachments.emplace_back(input.attachment, static_cast<VkImageLayout>(static_cast<int>(input.layout)));

		for (auto& color : subpass.color_attachments)
			reference.color_attachments.emplace_back(color.attachment, static_cast<VkImageLayout>(static_cast<int>(color.layout)));

		for (auto& resolve : subpass.resolve_attachments)
			reference.resolve_attachments.emplace_back(resolve.attachment, static_cast<VkImageLayout>(static_cast<int>(resolve.layout)));

		ASSERT(reference.resolve_attachments.empty() || reference.resolve_attachments.size() == reference.color_attachments.size());

		VkSubpassDescription description {
			.pipelineBindPoint = static_cast<VkPipelineBindPoint>(static_cast<int>(subpass.pipeline_bind_point)),
			.inputAttachmentCount = static_cast<uint32_t>(reference.input_attachments.size()),
			.pInputAttachments = reference.input_attachments.data(),
			.colorAttachmentCount = static_cast<uint32_t>(reference.color_attachments.size()),
			.pColorAttachments = reference.color_attachments.data(),
			.pResolveAttachments = reference.resolve_attachments.data(),
			.pDepthStencilAttachment = nullptr,
			.preserveAttachmentCount = static_cast<uint32_t>(subpass.preserve_attachments.size()),
			.pPreserveAttachments = subpass.preserve_attachments.data()
		};

		if (subpass.depth_stencil_attachment)
		{
			reference.depth_stencil_attachment = { subpass.depth_stencil_attachment.value().attachment,
				static_cast<VkImageLayout>(static_cast<int>(subpass.depth_stencil_attachment.value().layout)) };

			description.pDepthStencilAttachment = &reference.depth_stencil_attachment;
		}

		subpasses.emplace_back(description);
	}

	VkRenderPassCreateInfo render_pass_info{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = static_cast<uint32_t>(attachments.size()),
		.pAttachments = attachments.data(),
		.subpassCount = static_cast<uint32_t>(subpasses.size()),
		.pSubpasses = subpasses.data(),
		.dependencyCount = static_cast<uint32_t>(dependencies.size()),
		.pDependencies = dependencies.data()
	};

	VkRenderPass vk_render_pass;
	VK_CHECK_RESULT(vkCreateRenderPass(device, &render_pass_info, nullptr, &vk_render_pass));
	return vk_render_pass;
}

static VkPipeline transfer_pipeline(VkDevice device, 
									VkPipelineLayout pipeline_layout,
									VkRenderPass render_pass,
									const PipelineResource& pipeline_resource, 
									const std::shared_ptr<RenderResources>& render_resources, 
									const VkExtent2D& extent)
{
    auto& pipeline = pipeline_resource.pipeline;

    std::vector<VkShaderModule> shader_modules;
	std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
	for (auto& shader : pipeline->shader_state)
	{
		auto& modules = render_resources->get_shader_resource(shader.module);
		switch (static_cast<ShaderStageFlagBits>(shader.stage))
		{
        case ShaderStageFlagBits::e_vertex:
        {
            VkShaderModule vert_shader_module = create_shader_module(device, modules.vertex_shader);
            VkPipelineShaderStageCreateInfo vert_shader_stage_info{
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = VK_SHADER_STAGE_VERTEX_BIT,
                .module = vert_shader_module,
                .pName = "main"
            };
            shader_modules.emplace_back(vert_shader_module);
            shader_stages.emplace_back(vert_shader_stage_info);
            break;
        }
        case ShaderStageFlagBits::e_fragment:
        {
            VkShaderModule frag_shader_module = create_shader_module(device, modules.fragment_shader);
            VkPipelineShaderStageCreateInfo frag_shader_stage_info{
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
                .module = frag_shader_module,
                .pName = "main"
            };
            shader_modules.emplace_back(frag_shader_module);
            shader_stages.emplace_back(frag_shader_stage_info);
            break;
        }
		default:
			break;
		}
	}

	VkVertexInputBindingDescription binding_description = VK_CLASS(Vertex)::get_binding_description();
	auto attribute_descriptions = VK_CLASS(Vertex)::get_attribute_descriptions();

	VkPipelineVertexInputStateCreateInfo vertex_input_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &binding_description,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size()),
		.pVertexAttributeDescriptions = attribute_descriptions.data()
	};


	VkGraphicsPipelineCreateInfo pipeline_info{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = static_cast<uint32_t>(shader_stages.size()),
		.pStages = shader_stages.data(),
		.pVertexInputState = &vertex_input_info,
		.layout = pipeline_layout,
		.renderPass = render_pass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1
	};

    // input assembly state
	VkPipelineInputAssemblyStateCreateInfo input_assembly_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
	};
	if (pipeline->input_assembly_state)
	{
        auto& input_assembly_state = pipeline->input_assembly_state.value();

        input_assembly_info.topology = static_cast<VkPrimitiveTopology>(static_cast<int>(input_assembly_state.topology));
        input_assembly_info.primitiveRestartEnable = input_assembly_state.primitive_restart_enable;
        
		pipeline_info.pInputAssemblyState = &input_assembly_info;
	}

    // viewport state
	std::vector<VkViewport> viewports;
	std::vector<VkRect2D> scissors;
	VkPipelineViewportStateCreateInfo viewport_state_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
	};
	if (pipeline->viewport_state)
	{
		auto& viewport_state = pipeline->viewport_state.value();

		for (auto& viewport : viewport_state.viewports)
        {
            VkViewport vk_viewport{
				.x = viewport.x,
				.y = viewport.y,
                .width = viewport.width.value_or(static_cast<float>(extent.width)),
                .height = viewport.height.value_or(static_cast<float>(extent.height)),
				.minDepth = viewport.min_depth,
				.maxDepth = viewport.max_depth
            };
            viewports.emplace_back(vk_viewport);
		}

		for (auto& scissor : viewport_state.scissors)
		{
			VkRect2D vk_scissor{
				.offset = { scissor.offset.x, scissor.offset.y },
			};
			if (!scissor.extent)
				vk_scissor.extent = extent;
			else  
				vk_scissor.extent = { scissor.extent->width,scissor.extent->height };
            scissors.emplace_back(vk_scissor);
		}

        viewport_state_info.viewportCount = static_cast<uint32_t>(viewports.size());
        viewport_state_info.pViewports = viewports.data();
        viewport_state_info.scissorCount = static_cast<uint32_t>(scissors.size());
        viewport_state_info.pScissors = scissors.data();

		pipeline_info.pViewportState = &viewport_state_info;
	}

    // rasterization state
	VkPipelineRasterizationStateCreateInfo rasterizer_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO
	};
	if (pipeline->rasterization_state)
	{
        auto& rasterization_state = pipeline->rasterization_state.value();

		rasterizer_info.depthClampEnable = rasterization_state.depth_clamp_enable;
		rasterizer_info.rasterizerDiscardEnable = rasterization_state.rasterizer_discard_enable;
		rasterizer_info.polygonMode = static_cast<VkPolygonMode>(static_cast<int>(rasterization_state.polygon_mode));
		rasterizer_info.cullMode = static_cast<VkCullModeFlags>(static_cast<int>(rasterization_state.cull_mode));
		rasterizer_info.frontFace = static_cast<VkFrontFace>(static_cast<int>(rasterization_state.front_face));
		rasterizer_info.depthBiasEnable = rasterization_state.depth_bias_enable;
        rasterizer_info.depthBiasConstantFactor = rasterization_state.depth_bias_constant_factor;
        rasterizer_info.depthBiasClamp = rasterization_state.depth_bias_clamp;
		rasterizer_info.depthBiasSlopeFactor = rasterization_state.depth_bias_slope_factor;
		rasterizer_info.lineWidth = rasterization_state.line_width;

        pipeline_info.pRasterizationState = &rasterizer_info;
	}

    // multisample state
	VkPipelineMultisampleStateCreateInfo multisampling_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
	};
	if (pipeline->multisample_state)
	{
        auto& multisample_state = pipeline->multisample_state.value();

        multisampling_info.rasterizationSamples = static_cast<VkSampleCountFlagBits>(static_cast<int>(multisample_state.rasterization_samples));
        multisampling_info.sampleShadingEnable = multisample_state.sample_shading_enable;
        multisampling_info.minSampleShading = multisample_state.min_sample_shading;
        multisampling_info.alphaToCoverageEnable = multisample_state.alpha_to_coverage_enable;
        multisampling_info.alphaToOneEnable = multisample_state.alpha_to_one_enable;

        pipeline_info.pMultisampleState = &multisampling_info;
    }

    // depth stencil state
	VkPipelineDepthStencilStateCreateInfo depth_stencil_info{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO
	};
	if (pipeline->depth_stencil_state)
	{
        auto& depth_stencil_state = pipeline->depth_stencil_state.value();

        depth_stencil_info.depthTestEnable = depth_stencil_state.depth_test_enable;
        depth_stencil_info.depthWriteEnable = depth_stencil_state.depth_write_enable;
        depth_stencil_info.depthCompareOp = static_cast<VkCompareOp>(static_cast<int>(depth_stencil_state.depth_compare_op));
        depth_stencil_info.depthBoundsTestEnable = depth_stencil_state.depth_bounds_test_enable;
        depth_stencil_info.stencilTestEnable = depth_stencil_state.stencil_test_enable;
		depth_stencil_info.front = {
            .failOp = static_cast<VkStencilOp>(static_cast<int>(depth_stencil_state.front.fail_op)),
            .passOp = static_cast<VkStencilOp>(static_cast<int>(depth_stencil_state.front.pass_op)),
            .depthFailOp = static_cast<VkStencilOp>(static_cast<int>(depth_stencil_state.front.depth_fail_op)),
            .compareOp = static_cast<VkCompareOp>(static_cast<int>(depth_stencil_state.front.compare_op)),
            .compareMask = depth_stencil_state.front.compare_mask,
            .writeMask = depth_stencil_state.front.write_mask,
            .reference = depth_stencil_state.front.reference
		};
		depth_stencil_info.back = {
            .failOp = static_cast<VkStencilOp>(static_cast<int>(depth_stencil_state.back.fail_op)),
            .passOp = static_cast<VkStencilOp>(static_cast<int>(depth_stencil_state.back.pass_op)),
            .depthFailOp = static_cast<VkStencilOp>(static_cast<int>(depth_stencil_state.back.depth_fail_op)),
            .compareOp = static_cast<VkCompareOp>(static_cast<int>(depth_stencil_state.back.compare_op)),
            .compareMask = depth_stencil_state.back.compare_mask,
            .writeMask = depth_stencil_state.back.write_mask,
            .reference = depth_stencil_state.back.reference
        };
        depth_stencil_info.minDepthBounds = depth_stencil_state.min_depth_bounds;
        depth_stencil_info.maxDepthBounds = depth_stencil_state.max_depth_bounds;

        pipeline_info.pDepthStencilState = &depth_stencil_info;
    }

    // color blend state
    std::vector<VkPipelineColorBlendAttachmentState> color_blend_attachments;
	VkPipelineColorBlendStateCreateInfo color_blending_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO
	};
	if (pipeline->color_blend_state)
	{
        auto& color_blend_state = pipeline->color_blend_state.value();

		for (auto& attachment : color_blend_state.color_blend_attachments)
		{
			VkPipelineColorBlendAttachmentState color_blend_attachment{
				.blendEnable = attachment.blend_enable,
				.srcColorBlendFactor = static_cast<VkBlendFactor>(static_cast<int>(attachment.src_color_blend_factor)),
				.dstColorBlendFactor = static_cast<VkBlendFactor>(static_cast<int>(attachment.dst_color_blend_factor)),
				.colorBlendOp = static_cast<VkBlendOp>(static_cast<int>(attachment.color_blend_op)),
				.srcAlphaBlendFactor = static_cast<VkBlendFactor>(static_cast<int>(attachment.src_alpha_blend_factor)),
				.dstAlphaBlendFactor = static_cast<VkBlendFactor>(static_cast<int>(attachment.dst_alpha_blend_factor)),
				.alphaBlendOp = static_cast<VkBlendOp>(static_cast<int>(attachment.alpha_blend_op)),
				.colorWriteMask = static_cast<VkColorComponentFlags>(static_cast<int>(attachment.color_write_mask))
			};
			color_blend_attachments.emplace_back(color_blend_attachment);
		}

        color_blending_info.logicOpEnable = color_blend_state.logic_op_enable;
        color_blending_info.logicOp = static_cast<VkLogicOp>(static_cast<int>(color_blend_state.logic_op));
        color_blending_info.attachmentCount = static_cast<uint32_t>(color_blend_attachments.size());
        color_blending_info.pAttachments = color_blend_attachments.data();
		color_blending_info.blendConstants[0] = color_blend_state.blend_constants[0];
		color_blending_info.blendConstants[1] = color_blend_state.blend_constants[1];
		color_blending_info.blendConstants[2] = color_blend_state.blend_constants[2];
		color_blending_info.blendConstants[3] = color_blend_state.blend_constants[3];

        pipeline_info.pColorBlendState = &color_blending_info;
    }

    // dynamic state
	std::vector<VkDynamicState> dynamic_states;
	VkPipelineDynamicStateCreateInfo dynamic_state_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO
	};
	if (!pipeline->dynamic_state.empty())
	{
		for (auto& dynamic_state : pipeline->dynamic_state)
			dynamic_states.emplace_back(static_cast<VkDynamicState>(static_cast<int>(dynamic_state)));

		dynamic_state_info.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size());
		dynamic_state_info.pDynamicStates = dynamic_states.data();
		pipeline_info.pDynamicState = &dynamic_state_info;
	}
	
    VkPipeline vk_pipeline;
	VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &vk_pipeline));

    for (auto& shader_module : shader_modules)
        vkDestroyShaderModule(device, shader_module, nullptr);
	
	return vk_pipeline;
}


VK_CLASS(PipelineLayout)::~VK_CLASS(PipelineLayout)()
{
	vkDestroyPipelineLayout(g_system_context->g_render_system->m_drawable->m_device->m_device, m_pipeline_layout, nullptr);
}

constexpr NODISCARD RHIFlag VK_CLASS(PipelineLayout)::flag() const
{
	return RHIFlag::e_pipeline_layout;
}

void VK_CLASS(PipelineLayout)::initialize()
{
	create_pipeline_layout();
}

void VK_CLASS(PipelineLayout)::create_pipeline_layout()
{
	VkPipelineLayoutCreateInfo pipeline_layout_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 0,
		.pSetLayouts = nullptr,
		.pushConstantRangeCount = 0,
		.pPushConstantRanges = nullptr
	};

	VK_CHECK_RESULT(vkCreatePipelineLayout(g_system_context->g_render_system->m_drawable->m_device->m_device, &pipeline_layout_info, nullptr, &m_pipeline_layout));
}


VK_CLASS(RenderPass)::~VK_CLASS(RenderPass)()
{
	vkDestroyRenderPass(g_system_context->g_render_system->m_drawable->m_device->m_device, m_render_pass, nullptr);
}

void VK_CLASS(RenderPass)::initialize(const std::string_view& name)
{
	create_render_pass(name);
}


constexpr NODISCARD RHIFlag VK_CLASS(RenderPass)::flag() const
{
	return RHIFlag::e_render_pass;
}

void VK_CLASS(RenderPass)::create_render_pass(const std::string_view& name)
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
	auto swap_chain = g_system_context->g_render_system->m_drawable->m_swap_chain;

	auto& render_pass = g_system_context->g_render_system->m_render_resources->get_render_pass_resource(name);
	m_render_pass = transfer_render_pass(device, render_pass);
}



VK_CLASS(Pipeline)::~VK_CLASS(Pipeline)()
{
	vkDestroyPipeline(g_system_context->g_render_system->m_drawable->m_device->m_device, m_pipeline, nullptr);
}

void VK_CLASS(Pipeline)::initialize(const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout, const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass)
{
	create_pipeline(pipeline_layout, render_pass);
}

NODISCARD constexpr RHIFlag VK_CLASS(Pipeline)::flag() const
{
	return RHIFlag::e_pipeline;
}

void VK_CLASS(Pipeline)::create_pipeline(const std::shared_ptr<VK_CLASS(PipelineLayout)>& pipeline_layout, const std::shared_ptr<VK_CLASS(RenderPass)>& render_pass)
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
    auto& pipeline = g_system_context->g_render_system->m_render_resources->get_pipeline_resource("basic");

    m_pipeline = transfer_pipeline(device, 
		pipeline_layout->m_pipeline_layout, 
		render_pass->m_render_pass, pipeline, 
		g_system_context->g_render_system->m_render_resources, 
		g_system_context->g_render_system->m_drawable->m_swap_chain->m_details.extent.value());
}

ENGINE_NAMESPACE_END