#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// Core log macros
#define LOG_CORE_ERROR(...)   Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_WARN(...) ::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_INFO(...)    ::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_TRACE(...)   ::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_FATAL(...)   ::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define LOG_ERROR(...)   ::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)    ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)    ::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)   ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_FATAL(...)   ::Log::GetClientLogger()->fatal(__VA_ARGS__)