#pragma once
#include "Engine\Helpers\Observer.h"

class LevelManager final : public MyEngine::Observer<LevelManager>
{
public:
	enum LevelManagerEvent
	{
		IncreaseLevel,
		GoBackToMenu
	};

	void Notify(const MyEngine::Event event, const int value = 0, const MyEngine::GameObject* object = nullptr) override;
	int GetCurrentLevel() const { return m_CurrentLevel; }
private:
	int m_CurrentLevel{ 0 };
};