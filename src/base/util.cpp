#include "util.h"
#include "logger.h"

AMAZING_NAMESPACE_BEGIN

NORETURN void fatal_error(const char* message)
{
	throw std::runtime_error(message);
}

NORETURN void fatal_error(const std::string& message)
{
	throw std::runtime_error(message);
}

std::shared_ptr<std::vector<char>> read_file(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
		LOG_ERROR("Read File", "Failed to open file!");

	size_t file_size = file.tellg();
	std::shared_ptr<std::vector<char>> buffer = std::make_shared<std::vector<char>>(file_size);
	file.seekg(0);
	file.read(buffer->data(), static_cast<int64_t>(file_size));
	file.close();
	return buffer;
}

AMAZING_NAMESPACE_END