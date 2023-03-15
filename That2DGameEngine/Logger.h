#pragma once

#include <string>

namespace that
{
	class LogException {};

	class Logger
	{
	public:
		static void Log(const std::string& message);
		static void LogWarning(const std::string& message);
		static void LogError(const std::string& message);
	private:
	};
}
