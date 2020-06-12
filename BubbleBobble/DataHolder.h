#pragma once
#include <vector>
#include "Engine\Helpers\Singleton.h"
#include "Engine\Scene\GameObject.h"
#include "Level.h"

class DataHolder final : public MyEngine::Singleton<DataHolder>
{
public:
	DataHolder() = default;
	~DataHolder() = default;
	void Init(int windowWidth, int windowHeight);
	const Level& GetLevel(const int index) const { return m_Levels[index]; }
	std::vector<MyEngine::GameObject*>& GetEnemies() { return m_Enemies; }
	void AddEnemy(MyEngine::GameObject* enemy) { m_Enemies.push_back(enemy); }
	void RemoveEnemy(MyEngine::GameObject* enemy) { m_Enemies.erase(std::find(m_Enemies.begin(), m_Enemies.end(), enemy)); }
private:
	std::vector<Level> m_Levels;
	std::vector<MyEngine::GameObject*> m_Enemies;
};

