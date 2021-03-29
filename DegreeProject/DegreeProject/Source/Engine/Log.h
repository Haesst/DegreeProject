#pragma once

#pragma warning(push)
#pragma warning(disable: 26451 26812 26495 6387)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#pragma warning(pop)

// Client log macros
#define LOG_ERROR(...)   ::Log::getLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)    ::Log::getLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)    ::Log::getLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)   ::Log::getLogger()->trace(__VA_ARGS__)
#define LOG_FATAL(...)   ::Log::getLogger()->fatal(__VA_ARGS__)

#ifdef ENABLE_ASSERTS
#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define ASSERT(x, ...) {}
#endif

class Log
{
public:
	static void init();

	inline static std::shared_ptr<spdlog::logger>& getLogger() { return m_Logger; }

private:
	static std::shared_ptr<spdlog::logger> m_Logger;
};