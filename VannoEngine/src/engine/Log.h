#pragma once

#include <memory>

#include "core.h"
#include "spdlog/spdlog.h"

namespace VannoEngine {
	class ENGINE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }
	
	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	};
}

// Core log macros
#define VE_CORE_TRACE(...)	::VannoEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VE_CORE_INFO(...)	::VannoEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VE_CORE_WARN(...)	::VannoEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VE_CORE_ERROR(...)	::VannoEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VE_CORE_FATAL(...)	::VannoEngine::Log::GetCoreLogger()->fatal(__VA_ARGS_

// Client log macros
#define VE_TRACE(...)	::VannoEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VE_INFO(...)	::VannoEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define VE_WARN(...)	::VannoEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VE_ERROR(...)	::VannoEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define VE_FATAL(...)	::VannoEngine::Log::GetClientLogger()->fatal(__VA_ARGS_
