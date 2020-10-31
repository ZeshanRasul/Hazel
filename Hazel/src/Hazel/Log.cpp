#include "Log.h"

namespace Hazel {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init()
	{
		// Pattern sets colour based on severity, timestamp, name of logger and log message
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		// Create new Core and Client logger and set their level to trace 
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	Log::Log()
	{

	}

	Log::~Log()
	{

	}

}