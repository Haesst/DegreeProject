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

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return m_Logger; }

private:
	static std::shared_ptr<spdlog::logger> m_Logger;
};

// Client log macros
#define LOG_ERROR(...)   ::Log::GetLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)    ::Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)    ::Log::GetLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)   ::Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_FATAL(...)   ::Log::GetLogger()->fatal(__VA_ARGS__)

#ifdef ENABLE_ASSERTS
#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define ASSERT(x, ...) {}
#endif