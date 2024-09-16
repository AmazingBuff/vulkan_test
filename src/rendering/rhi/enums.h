#pragma once

#include "base/macro.h"
#include "base/template.h"

ENGINE_NAMESPACE_BEGIN

// fork
enum class SampleCountFlagBits;
enum class PipelineStageFlagBits;
enum class AccessFlagBits;
enum class DependencyFlagBits;

using SampleCountFlags = Enum<SampleCountFlagBits>;
using PipelineStageFlags = Enum<PipelineStageFlagBits>;
using AccessFlags = Enum<AccessFlagBits>;
using DependencyFlags = Enum<DependencyFlagBits>;

enum class Format;
enum class AttachmentLoadOp;
enum class AttachmentStoreOp;
enum class ImageLayout;
enum class PipelineBindPoint;

// self define


ENGINE_NAMESPACE_END