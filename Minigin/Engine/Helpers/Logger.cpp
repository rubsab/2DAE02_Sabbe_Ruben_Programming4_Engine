#include "MiniginPCH.h"
#include "Logger.h"
#include <consoleapi2.h>
#include <processenv.h>

void MyEngine::Logger::Initialize()
{
	//Source for this initialize: OverlordEngine
#if defined(DEBUG) | defined(_DEBUG)
	//Create Console Window
	if (AllocConsole())
	{
		//Redirect std in, err and out to this console window
		FILE* pCout;
		freopen_s(&pCout, "CONIN$", "r", stdin);
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		freopen_s(&pCout, "CONOUT$", "w", stderr);

		//Fix known issues with creating new window (clear buffers)
		std::wcout.clear();
		std::cout.clear();
		std::wcerr.clear();
		std::cerr.clear();
		std::wcin.clear();
		std::cin.clear();

		//Set Handle
		m_ConsoleHandle = GetStdHandle((DWORD)-11);

		//Disable Close-Button
		HWND hwnd = GetConsoleWindow();
		if (hwnd != NULL)
		{
			HMENU hMenu = GetSystemMenu(hwnd, FALSE);
			if (hMenu != NULL) DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
		}
	}
#endif
}

void MyEngine::Logger::Log(LogLevel level, const std::string& message) const
{
	switch (level)
	{
	case LogLevel::Info:
		LogInfo(message);
		break;
	case LogLevel::Warning:
		LogWarning(message);
		break;
	case LogLevel::Error:
		LogError(message);
		break;
	default:
		LogInfo(message);
		break;
	}
}

void MyEngine::Logger::LogInfo(const std::string& message) const
{
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	std::cout << "[INFO] : " << message << std::endl;
	std::cout.flush();
}

void MyEngine::Logger::LogWarning(const std::string& message) const
{
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
	std::cout << "[WARNING] : " << message << std::endl;
	std::cout.flush();
}

void MyEngine::Logger::LogError(const std::string& message) const
{
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED);
	std::cout << "[ERROR] : " << message << std::endl;
	std::cout.flush();
}
