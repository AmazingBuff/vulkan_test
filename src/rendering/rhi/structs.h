#pragma once

#include "base/macro.h"

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
struct Attachments;
struct Subpasses;
struct RenderPassInfo;

// pipeline
struct PipelineShaderState;
struct PipelineInputAssemblyState;
struct PipelineViewportState;
struct PipelineRasterizationState;
struct PipelineMultisampleState;
struct PipelineDepthStencilState;
struct PipelineColorBlendAttachmentState;
struct PipelineColorBlendState;
struct PipelineInfo;

ENGINE_NAMESPACE_END