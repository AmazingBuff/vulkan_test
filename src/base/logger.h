#pragma once

class Logger
{
public:
	enum class Level : uint8_t
	{
		e_info,
		e_warning,
		e_error
	};

	static void log(const Level& level, const char* loc, const char* message);
};

#if defined(_DEBUG) || defined(DEBUG)
#define LOG_INFO(loc, message)		Logger::log(Logger::Level::e_info, loc, message)
#define LOG_WARNING(loc, message)	Logger::log(Logger::Level::e_warning, loc, message)
#define LOG_ERROR(loc, message)		Logger::log(Logger::Level::e_error, loc, message)
#else
#define LOG_INFO(loc, message)
#define LOG_WARNING(loc, message)
#define LOG_ERROR(loc, message)
#endif