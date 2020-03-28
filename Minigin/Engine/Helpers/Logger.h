#pragma once
#include "Singleton.h"
#include <iostream>
namespace MyEngine
{
	enum class LogLevel
	{
		Info,
		Warning,
		Error
	};

	class Logger final : public Singleton<Logger>
	{
	public:
		void Initialize();
		void Log(LogLevel level , const std::string& message) const;
		void LogInfo(const std::string& message) const;
		void LogWarning(const std::string& message) const;
		void LogError(const std::string& message) const;
	private:
		HANDLE m_ConsoleHandle;
	};
}