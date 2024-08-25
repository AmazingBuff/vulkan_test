#include "logger.h"

void Logger::log(const Level& level, const char* loc, const char* message)
{
	switch (level)
	{
	case Level::e_info:
		std::cout << "[" << loc << ", Info]: " << message << std::endl;
		break;
	case Level::e_warning:
		std::cerr << "[" << loc << ", Warning]: " << message << std::endl;
		break;
	case Level::e_error:
		std::cerr << "[" << loc << ", Error]: " << message << std::endl;
		break;
	}
}