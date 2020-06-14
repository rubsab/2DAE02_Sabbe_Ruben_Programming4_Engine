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
	void Damage();
	void Reset();
private:
	float m_JumpTime, m_ShootTime;
	int m_Score, m_Lives;
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
	float m_HitTimer;
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
	void Jump();
	void SearchPlayer();
	int m_Type;
	float m_Delay, m_JumpTime, m_AttackCooldown;
	MyEngine::GameObject* m_ClosestPlayer;
};

class RockComponent final : public MyEngine::BaseComponent
{
public:
	RockComponent(bool isGoingLeft);
	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;
private:
	bool m_IsGoingLeft;
};