#include "pipeline.h"
#include "system/system.h"
#include "rendering/renderer.h"
#include "rendering/drawable.h"
#include "rendering/render_resouces.h"
#include "rendering/resources/shader/shader_manager.h"

ENGINE_NAMESPACE_BEGIN

static VkShaderModule create_shader_module(VkDevice device, const std::vector<char>& code)
{
	VkShaderModuleCreateInfo create_info{
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = code.size(),
		.pCode = reinterpret_cast<const uint32_t*>(code.data())
	};

	VkShaderModule shader_module;
	VK_CHECK_RESULT(vkCreateShaderModule(device, &create_info, nullptr, &shader_module));
	return shader_module;
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

void VK_CLASS(RenderPass)::initialize()
{
	create_render_pass();
}


constexpr NODISCARD RHIFlag VK_CLASS(RenderPass)::flag() const
{
	return RHIFlag::e_render_pass;
}

void VK_CLASS(RenderPass)::create_render_pass()
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
	auto swap_chain = g_system_context->g_render_system->m_drawable->m_swap_chain;

	VkAttachmentDescription color_attachment{
		.format = swap_chain->m_info.format.value(),
		.samples = VK_SAMPLE_COUNT_1_BIT,
		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
	};

	VkAttachmentReference color_attachment_ref{
		.attachment = 0,
		.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkSubpassDescription subpass{
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &color_attachment_ref
	};

	VkRenderPassCreateInfo render_pass_info{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = 1,
		.pAttachments = &color_attachment,
		.subpassCount = 1,
		.pSubpasses = &subpass,
	};

	VK_CHECK_RESULT(vkCreateRenderPass(device, &render_pass_info, nullptr, &m_render_pass));
}



VK_CLASS(Pipeline)::~VK_CLASS(Pipeline)()
{
	vkDestroyPipeline(g_system_context->g_render_system->m_drawable->m_device->m_device, m_pipeline, nullptr);
}

void VK_CLASS(Pipeline)::initialize()
{
	create_pipeline();
}

NODISCARD constexpr RHIFlag VK_CLASS(Pipeline)::flag() const
{
	return RHIFlag::e_pipeline;
}

void VK_CLASS(Pipeline)::create_pipeline()
{
	auto device = g_system_context->g_render_system->m_drawable->m_device->m_device;
	// todo: design a shader, pipeline layout and render pass manager which read configuration from a yaml file
	// here just temporarily hard code the shader, pipeline layout and render pass
	m_pipeline_layout = std::make_shared<VK_CLASS(PipelineLayout)>();
	m_pipeline_layout->initialize();
	m_render_pass = std::make_shared<VK_CLASS(RenderPass)>();
	m_render_pass->initialize();
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

	VkPipelineVertexInputStateCreateInfo vertex_input_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 0,
		.pVertexBindingDescriptions = nullptr,
		.vertexAttributeDescriptionCount = 0,
		.pVertexAttributeDescriptions = nullptr
	};

	VkPipelineInputAssemblyStateCreateInfo input_assembly_info{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE
	};

	auto& extent = g_system_context->g_render_system->m_drawable->m_swap_chain->m_info.extent.value();
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
		.layout = m_pipeline_layout->m_pipeline_layout,
		.renderPass = m_render_pass->m_render_pass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
		.basePipelineIndex = -1
	};

	VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipeline_info, nullptr, &m_pipeline));

	vkDestroyShaderModule(device, frag_shader_module, nullptr);
	vkDestroyShaderModule(device, vert_shader_module, nullptr);
}

ENGINE_NAMESPACE_END