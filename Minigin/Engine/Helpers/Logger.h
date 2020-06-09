#pragma once
#include <iostream>
namespace MyEngine
{
	enum class LogLevel
	{
		Info,
		Warning,
		Error
	};

	class Logger final
	{
	public:
		static void Init();
		static void Log(LogLevel level , const std::string& message);
		static void LogInfo(const std::string& message);
		static void LogWarning(const std::string& message);
		static void LogError(const std::string& message);
	private:
		static HANDLE m_ConsoleHandle;
		static bool m_IsInitialized;
	};
}