#pragma once

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#include "Core.h"

namespace PriMech {
	class PRIMECH_API Log
	{
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }



	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//core Log macros
#define PM_CORE_DEBUG(...)		::PriMech::Log::GetCoreLogger()->debug(__VA_ARGS__)
#define PM_CORE_TRACE(...)		::PriMech::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define PM_CORE_INFO(...)		::PriMech::Log::GetCoreLogger()->info(__VA_ARGS__)
#define PM_CORE_WARN(...)		::PriMech::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define PM_CORE_ERROR(...)		::PriMech::Log::GetCoreLogger()->error(__VA_ARGS__)
#define PM_CORE_CRITICAL(...)	::PriMech::Log::GetCoreLogger()->critical(__VA_ARGS__)

//client Log macros
#define PM_INFO(...)			::PriMech::Log::GetClientLogger()->info(__VA_ARGS__)
#define PM_WARN(...)			::PriMech::Log::GetClientLogger()->warn(__VA_ARGS__)
#define PM_ERROR(...)			::PriMech::Log::GetClientLogger()->error(__VA_ARGS__)
#define PM_CRITICAL(...)		::PriMech::Log::GetClientLogger()->critical(__VA_ARGS__)