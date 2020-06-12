#include "Engine\Core\MiniginPCH.h"
#include "DataHolder.h"
#include "LevelDataParser.h"

void DataHolder::Init(int windowWidth, int windowHeight)
{
	LevelDataParser::ParseFile("../Resources/BBSprites/SeparatedLevelData.dat", "../Resources/BBSprites/SeparatedEnemyData.dat", m_Levels);
	for (Level& level : m_Levels)
	{
		level.WindowWidth = windowWidth;
		level.WindowHeight = windowHeight;
	}
}
