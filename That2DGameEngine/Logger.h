#pragma once

#include <string>

namespace that
{
	class LogException {};

	class GameObject;

	class Logger
	{
	public:
		static void Log(const std::string& message);
		static void LogWarning(const std::string& message, const GameObject* pGameObject = nullptr);
		static void LogError(const std::string& message);
	private:
	};
}
