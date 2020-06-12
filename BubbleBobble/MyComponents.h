#pragma once
#include "Engine\Components\BaseComponent.h"

struct Level;
namespace MyEngine { class Scene; }

class PlayerMovementComponent final : public MyEngine::BaseComponent
{
public:
	PlayerMovementComponent();

	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;

	void Jump();
	void Shoot();
private:
	float m_JumpTime, m_ShootTime;
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
	FruitDropComponent();
	void Update(const float) override;
	void FixedUpdate(const float fixedDeltaTime) override;
	void Render() const override;
private:
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