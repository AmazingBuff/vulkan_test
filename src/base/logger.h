#pragma once

#include <fmt/format.h>
#include "util.h"

class Logger
{
public:
	enum class Level : uint8_t
	{
		e_info,
		e_warning,
		e_error
	};

	template<typename... Args>
	static void log(const Level& level, const char* loc, const std::string& message, Args&&... args);
};

template<typename... Args>
void Logger::log(const Level& level, const char* loc, const std::string& message, Args&&... args)
{
	if constexpr (sizeof...(Args) != 0)
		message = fmt::vformat(message, std::forward<Args>(args)...);
	switch (level)
	{
	case Level::e_info:
		std::cout << "[" << loc << ", Info]: " << message << std::endl;
		break;
	case Level::e_warning:
		std::cerr << "[" << loc << ", Warning]: " << message << std::endl;
		break;
	case Level::e_error:
		fatal_error(message);
		break;
	}
}

#if defined(_DEBUG) || defined(DEBUG)
#define LOG_INFO(loc, ...)		Logger::log(Logger::Level::e_info, loc, __VA_ARGS__)
#define LOG_WARNING(loc, ...)	Logger::log(Logger::Level::e_warning, loc, __VA_ARGS__)
#define LOG_ERROR(loc, ...)		Logger::log(Logger::Level::e_error, loc, __VA_ARGS__)
#else
#define LOG_INFO(loc, message)
#define LOG_WARNING(loc, message)
#define LOG_ERROR(loc, message)
#endif