#pragma once
#include <regex>
#include <fstream>
#include "Engine\Core\MiniginPCH.h"
#include "Engine\Helpers\Logger.h"
#include "Level.h"

class LevelDataParser final
{
public:
	static void ParseFile(const std::string& path, std::vector<Level>& levels)
	{
		std::regex fileExtensionRegex{ ".+\\.dat$" };

		std::ifstream file;

		if (!std::regex_match(path, fileExtensionRegex))
		{
			MyEngine::Logger::LogError("File [" + path + "] is not a .dat extension");
			return;
		}

		file.open(path, std::ios::in | std::ios::binary);

		if (!file.is_open())
		{
			MyEngine::Logger::LogError("File [" + path + "] could not be opened");
			return;
		}

		while (!file.eof())
		{
			levels.push_back(Level());
			Level& level = levels.back();
			for (size_t lineCounter{}; lineCounter < 25; lineCounter++)
			{
				level.push_back(Line());
				Line& line = level.back();
				for (size_t charCounter{}; charCounter < 4; charCounter++)
				{
					char c{};
					file.read((char*)&c, sizeof(char));
					for (size_t cellCounter{}; cellCounter < 8; cellCounter++)
					{
						line.push_back(c & 1);
						c = c >> 1;
					}
				}
			}
		}

		file.close();
	}
};