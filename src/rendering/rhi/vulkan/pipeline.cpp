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

static VkPipeline transfer_pipeline(VkDevice device, const PipelineResources& pipeline_resources)
{
	return nullptr;
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
	// todo: design a shader, pipeline layout and render pass manager which read configuration from a yaml file
	// here just temporarily hard code the shader, pipeline layout and render pass
	auto& shaders = g_system_context->g_render_system->m_render_resources->get_shader_resource("triangle");

	VkShaderModule vert_shader_module = create_shader_module(device, shaders.vertex_shader);
	VkShaderModule frag_shader_module = create_shader_module(device, shaders.fragment_shader);

	std::vector<VkPipelineShaderStageCreateInfo> shader_stages;
	VkPipelineShaderStageCreateInfo vert_shader_stage_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vert_shader_module,
		.pName = "main"
	};
	shader_stages.emplace_back(vert_shader_stage_info);

	VkPipelineShaderStageCreateInfo frag_shader_stage_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = frag_shader_module,
		.pName = "main"
	};
	shader_stages.emplace_back(frag_shader_stage_info);

	std::vector<VkDynamicState> dynamic_states = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
	VkPipelineDynamicStateCreateInfo dynamic_state_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
		.pDynamicStates = dynamic_states.data()
	};

	VkVertexInputBindingDescription binding_description = VK_CLASS(Vertex)::get_binding_description();
	auto attribute_descriptions = VK_CLASS(Vertex)::get_attribute_descriptions();

	VkPipelineVertexInputStateCreateInfo vertex_input_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 1,
		.pVertexBindingDescriptions = &binding_description,
		.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size()),
		.pVertexAttributeDescriptions = attribute_descriptions.data()
	};

	VkPipelineInputAssemblyStateCreateInfo input_assembly_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE
	};

	auto& extent = g_system_context->g_render_system->m_drawable->m_swap_chain->m_details.extent.value();
	VkViewport viewport{
		.x = 0.0f,
		.y = 0.0f,
		.width = static_cast<float>(extent.width),
		.height = static_cast<float>(extent.height),
		.minDepth = 0.0f,
		.maxDepth = 1.0f
	};

	VkRect2D scissor{
		.offset = { 0, 0 },
		.extent = extent
	};

	VkPipelineViewportStateCreateInfo viewport_state_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.pViewports = &viewport,
		.scissorCount = 1,
		.pScissors = &scissor
	};

	VkPipelineRasterizationStateCreateInfo rasterizer_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.depthBiasConstantFactor = 0.0f,
		.depthBiasClamp = 0.0f,
		.depthBiasSlopeFactor = 0.0f,
		.lineWidth = 1.0f,
	};

	VkPipelineMultisampleStateCreateInfo multisampling_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE,
		.minSampleShading = 1.0f,
		.pSampleMask = nullptr,
		.alphaToCoverageEnable = VK_FALSE,
		.alphaToOneEnable = VK_FALSE
	};

	VkPipelineDepthStencilStateCreateInfo depth_stencil_info{};

	VkPipelineColorBlendAttachmentState color_blend_attachment{
		.blendEnable = VK_FALSE,
		.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
		.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
		.colorBlendOp = VK_BLEND_OP_ADD,
		.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
		.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
		.alphaBlendOp = VK_BLEND_OP_ADD,
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
	};

	VkPipelineColorBlendStateCreateInfo color_blending_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &color_blend_attachment,
		.blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
	};

	VkGraphicsPipelineCreateInfo pipeline_info{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		.stageCount = static_cast<uint32_t>(shader_stages.size()),
		.pStages = shader_stages.data(),
		.pVertexInputState = &vertex_input_info,
		.pInputAssemblyState = &input_assembly_info,
		.pViewportState = &viewport_state_info,
		.pRasterizationState = &rasterizer_info,
		.pMultisampleState = &multisampling_info,
		.pDepthStencilState = nullptr,
		.pColorBlendState = &color_blending_info,
		.pDynamicState = &dynamic_state_info,
		.layout = pipeline_layout->m_pipeline_layout,
		.renderPass = render_pass->m_render_pass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1
	};

	VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline));

	vkDestroyShaderModule(device, frag_shader_module, nullptr);
	vkDestroyShaderModule(device, vert_shader_module, nullptr);
}

ENGINE_NAMESPACE_END