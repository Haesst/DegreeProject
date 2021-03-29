#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::m_Logger;

void Log::init()
{
	spdlog::set_pattern("%^[%T] %n: %v%$");

	m_Logger = spdlog::stdout_color_mt("GAME");
	m_Logger->set_level(spdlog::level::trace);
}