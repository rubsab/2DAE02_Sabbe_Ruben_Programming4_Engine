#pragma once
#include "Engine\Components\BaseComponent.h"

struct Level;
namespace MyEngine { class Scene; }

class PlayerBehaviourComponent final : public MyEngine::BaseComponent
{
public:
	PlayerBehaviourComponent();

	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;

	void IncreaseScore(int score) { m_Score += score; }

	void Jump();
	void Shoot();
private:
	float m_JumpTime, m_ShootTime;
	int m_Score;
};

class BubbleBehaviourComponent final : public MyEngine::BaseComponent
{
public:
	BubbleBehaviourComponent(bool isLookingLeft);
	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;
private:
	float m_SidewaysTimer, m_LifeTime, m_NextLevelTimer;
	bool m_IsDirLeft;
	MyEngine::GameObject* m_pHeldEnemy;
};

class FruitDropComponent final : public MyEngine::BaseComponent
{
public:
	FruitDropComponent(int enemyType);
	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;
private:
	int m_Score;
};

class EnemyBehaviourComponent final : public MyEngine::BaseComponent
{
public:
	EnemyBehaviourComponent(int type, float delay);
	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;
	int GetType() const { return m_Type; }
private:
	int m_Type;
	float m_Delay;
};