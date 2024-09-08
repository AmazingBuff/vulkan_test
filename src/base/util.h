#pragma once

#include "macro.h"


NORETURN void fatal_error(const char* message);

NORETURN void fatal_error(const std::string& message);

NODISCARD std::vector<char> read_file(const std::string& filename);