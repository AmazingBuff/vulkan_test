#pragma once

#include "macro.h"

AMAZING_NAMESPACE_BEGIN

NORETURN void fatal_error(const char* message);

NORETURN void fatal_error(const std::string& message);

NODISCARD std::shared_ptr<std::vector<char>> read_file(const std::string& filename);

NODISCARD size_t aligned_size(size_t size, size_t alignment);

AMAZING_NAMESPACE_END