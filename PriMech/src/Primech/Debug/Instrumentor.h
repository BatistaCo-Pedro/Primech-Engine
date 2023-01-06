#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace PriMech {
	struct ProfileResult {
		std::string name;
		long long start, end;
		uint32_t threadID;
	};

	struct InstrumentationSession {
		std::string name;
	};

	class Instrumentor
	{
	public:
		Instrumentor() : currentSession_(nullptr), profileCount_(0) {}

		void BeginSession(const std::string& name, const std::string& filepath = "results.json") {
			outputStream_.open(filepath);
			WriteHeader();
			currentSession_ = new InstrumentationSession{ name };
		}

		void EndSession() {
			WriteFooter();
			outputStream_.close();
			delete currentSession_;
			currentSession_ = nullptr;
			profileCount_ = 0;
		}

		void WriteProfile(const ProfileResult& result) {
			if (profileCount_++ > 0) {
				outputStream_ << ",";
			}
			std::string name = result.name;
			std::replace(name.begin(), name.end(), '"', '\'');

			outputStream_ << "{";
			outputStream_ << "\"cat\":\"function\",";
			outputStream_ << "\"dur\":" << (result.end - result.start) << ",";
			outputStream_ << "\"name\":\"" << name << "\",";
			outputStream_ << "\"ph\":\"X\",";
			outputStream_ << "\"pid\":0,";
			outputStream_ << "\"tid\":" << result.threadID << ",";
			outputStream_ << "\"ts\":" << result.start;
			outputStream_ << "}";

			outputStream_.flush();
		}

		void WriteHeader() {
			outputStream_ << "{\"otherData\": {}, \"traceEvents\":[";
			outputStream_.flush();
		}

		void WriteFooter() {
			outputStream_ << "]}";
			outputStream_.flush();
		}

		static Instrumentor& Get() {
			static Instrumentor instance;
			return instance;
		}
	private:
		InstrumentationSession* currentSession_;
		std::ofstream outputStream_;
		int profileCount_;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* name) : name_(name), stopped_(false) {
			startTimepoint_ = std::chrono::high_resolution_clock::now();
		}
		~InstrumentationTimer() {
			if (!stopped_)
				Stop();
		}

		void Stop() {
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint_).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ name_, start, end, threadID });

			stopped_ = true;
		}
	private:
		const char* name_;
		std::chrono::time_point<std::chrono::steady_clock> startTimepoint_;
		bool stopped_;
	};
}
#define PM_PROFILE 1
#if PM_PROFILE
	#define PM_PROFILE_BEGIN_SESSION(name, filepath)	::PriMech::Instrumentor::Get().BeginSession(name, filepath)
	#define PM_PROFILE_END_SESSION()					::PriMech::Instrumentor::Get().EndSession()
	#define PM_PROFILE_SCOPE(name)						::PriMech::InstrumentationTimer timer##__LINE__(name);
	#define PM_PROFILE_FUNCTION()						PM_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define PM_PROFILE_BEGIN_SESSION(name, filepath)					
	#define PM_PROFILE_END_SESSION()
	#define PM_PROFILE_SCOPE(name)
	#define PM_PROFILE_FUNCTION()
#endif