#include "Engine\Core\MiniginPCH.h"
#include "DataHolder.h"
#include "LevelDataParser.h"

void DataHolder::Init()
{
	LevelDataParser::ParseFile("../Resources/BBSprites/SeparatedLevelData.dat", "../Resources/BBSprites/SeparatedEnemyData.dat", m_Levels);
}
