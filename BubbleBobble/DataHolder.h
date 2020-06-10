#pragma once
#include <vector>
#include "Engine\Helpers\Singleton.h"
#include "Level.h"

class DataHolder final : public MyEngine::Singleton<DataHolder>
{
public:
	DataHolder() = default;
	~DataHolder() = default;
	void Init();
	const Level& GetLevel(const int index) const { return m_Levels[index]; }
private:
	std::vector<Level> m_Levels;
};

