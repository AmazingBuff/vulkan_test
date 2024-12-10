#pragma once

#include "base/macro.h"
#include "base/template.h"

ENGINE_NAMESPACE_BEGIN

// fork
enum class SampleCountFlagBits;
enum class PipelineStageFlagBits;
enum class AccessFlagBits;
enum class DependencyFlagBits;

using SampleCountFlags = BitFlag<SampleCountFlagBits>;
using PipelineStageFlags = BitFlag<PipelineStageFlagBits>;
using AccessFlags = BitFlag<AccessFlagBits>;
using DependencyFlags = BitFlag<DependencyFlagBits>;

enum class FormatEnum;
enum class AttachmentLoadOpEnum;
enum class AttachmentStoreOpEnum;
enum class ImageLayoutEnum;
enum class PipelineBindPointEnum;

using Format = Enum<FormatEnum>;
using AttachmentLoadOp = Enum<AttachmentLoadOpEnum>;
using AttachmentStoreOp = Enum<AttachmentStoreOpEnum>;
using ImageLayout = Enum<ImageLayoutEnum>;
using PipelineBindPoint = Enum<PipelineBindPointEnum>;

// self define
enum class AttachmentTypeEnum;
using AttachmentType = Enum<AttachmentTypeEnum>;

ENGINE_NAMESPACE_END