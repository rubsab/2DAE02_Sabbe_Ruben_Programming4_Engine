#pragma once
#include <regex>
#include <fstream>
#include "Engine\Core\MiniginPCH.h"
#include "Engine\Helpers\Logger.h"
#include "Level.h"

class LevelDataParser final
{
public:
	static void ParseFile(const std::string& levelPath, const std::string& enemyPath, std::vector<Level>& levels)
	{
		std::regex fileExtensionRegex{ ".+\\.dat$" };

		std::ifstream levelFile, enemyFile;

		if (!std::regex_match(levelPath, fileExtensionRegex))
		{
			MyEngine::Logger::LogError("File [" + levelPath + "] is not a .dat extension");
			return;
		}
		if (!std::regex_match(enemyPath, fileExtensionRegex))
		{
			MyEngine::Logger::LogError("File [" + enemyPath + "] is not a .dat extension");
			return;
		}

		levelFile.open(levelPath, std::ios::in | std::ios::binary);
		if (!levelFile.is_open())
		{
			MyEngine::Logger::LogError("File [" + levelPath + "] could not be opened");
			return;
		}
		enemyFile.open(enemyPath, std::ios::in | std::ios::binary);
		if (!enemyFile.is_open())
		{
			MyEngine::Logger::LogError("File [" + enemyPath + "] could not be opened");
			return;
		}

		for (int levelCounter{}; levelCounter < 100; levelCounter++)
		{
			levels.push_back(Level());
			Level& level = levels.back();
			for (size_t lineCounter{}; lineCounter < 25; lineCounter++)
			{
				level.first.push_back(Line());
				Line& line = level.first.back();
				for (size_t charCounter{}; charCounter < 4; charCounter++)
				{
					char c{};
					levelFile.read((char*)&c, sizeof(char));
					int nr = 0b10000000;
					for (size_t cellCounter{}; cellCounter < 8; cellCounter++)
					{
						line.push_back(c & nr);
						nr = nr >> 1;
					}
				}
			}
			char first, second, third;
			enemyFile.read((char*)&first, sizeof(char));
			while (first != 0)
			{
				enemyFile.read((char*)&second, sizeof(char));
				enemyFile.read((char*)&third, sizeof(char));
				level.second.push_back(Enemy());
				level.second.back().Type = EnemyType(first & 0b00000111);
				level.second.back().Col = int((first >> 3) & 0b00011111);
				level.second.back().Row = int((second >> 3) & 0b00011111);
				level.second.back().Delay = 0.017f * int((third << 1) & 0b00111111);
				enemyFile.read((char*)&first, sizeof(char));
			}
		}
		levelFile.close();
		enemyFile.close();
	}
};