#pragma once
#include "Engine\Components\BaseComponent.h"

struct Level;
namespace MyEngine { class Scene; }

class PlayerMovementComponent final : public MyEngine::BaseComponent
{
public:
	PlayerMovementComponent(const Level& level);

	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;

	void Jump();
	void Shoot();
private:
	const Level& m_CurrentLevel;
	float m_JumpTime, m_ShootTime;
};

class BubbleBehaviourComponent final : public MyEngine::BaseComponent
{
public:
	BubbleBehaviourComponent(const Level& level, bool isLookingLeft, MyEngine::GameObject* pPlayer, MyEngine::Scene* pScene);
	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;
private:
	const Level& m_CurrentLevel;
	float m_SidewaysTimer, m_LifeTime;
	bool m_IsDirLeft;
	MyEngine::GameObject* m_pHeldEnemy, *m_pPlayer;
	MyEngine::Scene* m_pScene;
};

class FruitDropComponent final : public MyEngine::BaseComponent
{
public:
	FruitDropComponent(MyEngine::GameObject* pPlayer, const Level& level);
	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;
private:
	const Level& m_CurrentLevel;
	MyEngine::GameObject* m_pPlayer;
};

class EnemyBehaviourComponent final : public MyEngine::BaseComponent
{
public:
	EnemyBehaviourComponent(MyEngine::GameObject* pPlayer, int type, float delay);
	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;
	int GetType() const { return m_Type; }
private:
	int m_Type;
	float m_Delay;
	MyEngine::GameObject* m_pPlayer;
};