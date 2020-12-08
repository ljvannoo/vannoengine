#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace VannoEngine {
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	void Log::Init() {
		spdlog::set_pattern("%^[%T] %n: %v%$");

		sCoreLogger = spdlog::stdout_color_mt("Engine");
		sCoreLogger->set_level(spdlog::level::trace);
		sClientLogger = spdlog::stdout_color_mt("Game");
		sClientLogger->set_level(spdlog::level::trace);
	}
}
