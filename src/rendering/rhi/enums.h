#pragma once

#include "base/macro.h"
#include "base/template.h"

ENGINE_NAMESPACE_BEGIN

// fork
enum class SampleCountFlagBits;
enum class PipelineStageFlagBits;
enum class AccessFlagBits;
enum class DependencyFlagBits;
enum class ShaderStageFlagBits;
enum class CullModeFlagBits;
enum class ColorComponentFlagBits;

using SampleCountFlags = BitFlag<SampleCountFlagBits>;
using PipelineStageFlags = BitFlag<PipelineStageFlagBits>;
using AccessFlags = BitFlag<AccessFlagBits>;
using DependencyFlags = BitFlag<DependencyFlagBits>;
using ShaderStageFlags = BitFlag<ShaderStageFlagBits>;
using CullModeFlags = BitFlag<CullModeFlagBits>;
using ColorComponentFlags = BitFlag<ColorComponentFlagBits>;

enum class FormatEnum;
enum class AttachmentLoadOpEnum;
enum class AttachmentStoreOpEnum;
enum class ImageLayoutEnum;
enum class PipelineBindPointEnum;
enum class DynamicStateEnum;
enum class PrimitiveTopologyEnum;
enum class PolygonModeEnum;
enum class FrontFaceEnum;
enum class CompareOpEnum;
enum class StencilOpEnum;
enum class BlendFactorEnum;
enum class BlendOpEnum;
enum class LogicOpEnum;

using Format = Enum<FormatEnum>;
using AttachmentLoadOp = Enum<AttachmentLoadOpEnum>;
using AttachmentStoreOp = Enum<AttachmentStoreOpEnum>;
using ImageLayout = Enum<ImageLayoutEnum>;
using PipelineBindPoint = Enum<PipelineBindPointEnum>;
using DynamicState = Enum<DynamicStateEnum>;
using PrimitiveTopology = Enum<PrimitiveTopologyEnum>;
using PolygonMode = Enum<PolygonModeEnum>;
using FrontFace = Enum<FrontFaceEnum>;
using CompareOp = Enum<CompareOpEnum>;
using StencilOp = Enum<StencilOpEnum>;
using BlendFactor = Enum<BlendFactorEnum>;
using BlendOp = Enum<BlendOpEnum>;
using LogicOp = Enum<LogicOpEnum>;

// self define
enum class AttachmentTypeEnum;
using AttachmentType = Enum<AttachmentTypeEnum>;

ENGINE_NAMESPACE_END