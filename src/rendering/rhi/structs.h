#pragma once

#include "enums.h"

ENGINE_NAMESPACE_BEGIN

struct Offset2D;
struct Extent2D;
struct Rect2D;
struct Viewport;
struct StencilOpState;

// render pass
struct AttachmentDescription;
struct AttachmentReference;
struct SubpassDescription;
struct SubpassDependency;

// pipeline
struct PipelineShaderState;
struct PipelineDynamicState;
struct PipelineInputAssemblyState;
struct PipelineViewportState;
struct PipelineRasterizerState;
struct PipelineMultisampleState;
struct PipelineDepthStencilState;
struct PipelineColorBlendAttachmentState;
struct PipelineColorBlendState;

ENGINE_NAMESPACE_END