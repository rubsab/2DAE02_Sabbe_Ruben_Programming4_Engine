#pragma once
#include <vector>
#include "Engine\Helpers\Singleton.h"
#include "Engine\Scene\GameObject.h"
#include "Level.h"

class DataHolder final : public MyEngine::Singleton<DataHolder>
{
public:
	DataHolder() = default;
	~DataHolder();
	void Init(int windowWidth, int windowHeight);
	const Level& GetLevel(const int index) const { return m_Levels[index]; }
	std::vector<MyEngine::GameObject*>& GetEnemies() { return m_Enemies; }
	void AddEnemy(MyEngine::GameObject* enemy) { m_Enemies.push_back(enemy); }
	void RemoveEnemy(MyEngine::GameObject* enemy) { m_Enemies.erase(std::find(m_Enemies.begin(), m_Enemies.end(), enemy)); }
	void RemoveAllEnemies() { m_Enemies.clear(); }
	std::vector<MyEngine::GameObject*>& GetPlayers() { return m_Players; }
	void AddPlayer(MyEngine::GameObject* player) { m_Players.push_back(player); }
	void RemoveAllPlayers() { m_Players.clear(); }
	int GetScore(int index) const { return m_Scores[index]; }
private:
	std::vector<Level> m_Levels;
	std::vector<MyEngine::GameObject*> m_Enemies;
	std::vector<MyEngine::GameObject*> m_Players;
	int m_Scores[8] = { 1, 4, 2, 5, 6, 8, 3, 7 };
};

