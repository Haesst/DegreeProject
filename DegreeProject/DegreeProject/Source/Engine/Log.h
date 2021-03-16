#pragma once

#pragma warning(push)
#pragma warning(disable: 26451 26812 26495 6387)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

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

// Client log macros
#define LOG_ERROR(...)   ::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)    ::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)    ::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)   ::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_FATAL(...)   ::Log::GetClientLogger()->fatal(__VA_ARGS__)

#ifdef ENABLE_ASSERTS
#define ASSERT(x, ...) { if(!(x)) { ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
#define ASSERT(x, ...)
#endif