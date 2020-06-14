#include "Engine\Core\MiniginPCH.h"
#include "MyComponents.h"
#include "Engine\Components\Components.h"
#include "Level.h"
#include "Engine\Scene\GameObject.h"
#include "Engine\Helpers\Logger.h"
#include <string>
#include "DataHolder.h"
#include "Engine\Managers\ResourceManager.h"
#include "Engine\Scene\Scene.h"
#include "Factory.h"
#include "glm\glm.hpp"
#include "glm\gtx\norm.hpp"

inline glm::fvec2 ScreenBordersCheck(const glm::fvec2& pos)
{
	glm::fvec2 newPos = pos;
	const Level& level = DataHolder::GetInstance()->GetLevel(LevelManager::GetInstance()->GetCurrentLevel());
	if (newPos.y > level.WindowHeight)
		newPos.y -= level.WindowHeight;
	if (newPos.y < 0.0f)
		newPos.y += level.WindowHeight;
	return newPos;
}

PlayerBehaviourComponent::PlayerBehaviourComponent() :
	m_JumpTime(0.0f),
	m_ShootTime(0.0f),
	m_Score(0),
	m_Lives(4)
{
}

void PlayerBehaviourComponent::Update(const float) {}

void PlayerBehaviourComponent::FixedUpdate(const float fixedDeltaTime)
{
	int state = m_pGameObject->GetState();
	MyEngine::TransformComponent* trans = m_pGameObject->GetComponent<MyEngine::TransformComponent>();

	if (state == 4 || state == 5)
	{
		m_ShootTime -= fixedDeltaTime;
		if (m_ShootTime <= 0.0f)
			m_pGameObject->SetState(state - 2);
	}

	//Horizontal Movement
	if (state == 0 || state == 1)
	{
		if (currentLevel.IsWallInFront(trans->GetPosition(), bool(state)))
			m_pGameObject->SetState(state + 2);
		else
			trans->SetPosition(trans->GetPosition().x + -(state * 2 - 1) * fixedDeltaTime * 120.0f, trans->GetPosition().y);
	}

	//Vertical Movement
	if (m_JumpTime >= 0.0f)
	{
		m_JumpTime -= fixedDeltaTime;
		trans->SetPosition(trans->GetPosition().x, trans->GetPosition().y + fixedDeltaTime * 200.0f);
	}
	else if (!currentLevel.IsWallBelow(trans->GetPosition()))
		trans->SetPosition(trans->GetPosition().x, trans->GetPosition().y - fixedDeltaTime * 94.0f);
	trans->SetPosition(ScreenBordersCheck(trans->GetPosition()));

	std::string score = std::to_string(m_Score) + " " + std::to_string(m_Lives);
	m_pGameObject->GetComponent<TextComponent>()->SetText(score);
	if (m_pGameObject == DataHolder::GetInstance()->GetPlayers()[0])
		m_pGameObject->GetComponent<TextComponent>()->SetOffSet({ currentLevel.WindowWidth - trans->GetPosition().x, currentLevel.WindowHeight - trans->GetPosition().y - 25.0f });
	else
		m_pGameObject->GetComponent<TextComponent>()->SetOffSet({ currentLevel.WindowWidth - trans->GetPosition().x, -trans->GetPosition().y + 25.0f });
}

void PlayerBehaviourComponent::Render() const {}

void PlayerBehaviourComponent::Jump()
{
	if (currentLevel.IsWallBelow(m_pGameObject->GetComponent<MyEngine::TransformComponent>()->GetPosition()))
	{
		m_JumpTime += 0.6f;
		MyEngine::SoundManager::GetInstance()->Notify(MyEngine::Event(SoundEvents::Jump));
	}
}

void PlayerBehaviourComponent::Shoot()
{
	m_ShootTime = 0.25f;
	m_pGameObject->SetState((m_pGameObject->GetState() % 2) + 4);
	MyEngine::SoundManager::GetInstance()->Notify(MyEngine::Event(SoundEvents::BubbleFire));
}

void PlayerBehaviourComponent::Damage()
{
	m_Lives--;
	if (m_Lives <= 0)
	{
		m_pGameObject->SetActive(false);
		for (GameObject* pPlayer : DataHolder::GetInstance()->GetPlayers())
		{
			if (pPlayer != m_pGameObject && !pPlayer->IsActive())
			{
				LevelManager::GetInstance()->Notify(MyEngine::Event(LevelManager::LevelManagerEvent::GoBackToMenu));
				pPlayer->GetComponent<PlayerBehaviourComponent>()->Reset();
				Reset();
				return;
			}
		}
	}
	LevelManager::GetInstance()->Notify(MyEngine::Event(LevelManager::LevelManagerEvent::ReloadLevel));
}

void PlayerBehaviourComponent::Reset()
{
	m_Score = 0;
	m_Lives = 4;
}

BubbleBehaviourComponent::BubbleBehaviourComponent(bool isLookingLeft) :
	m_SidewaysTimer(0.3f),
	m_IsDirLeft(isLookingLeft),
	m_LifeTime(5.0f),
	m_NextLevelTimer(0.0f),
	m_pHeldEnemy(nullptr)
{
}

void BubbleBehaviourComponent::Update(const float) {}

void BubbleBehaviourComponent::FixedUpdate(const float fixedDeltaTime)
{
	m_LifeTime -= fixedDeltaTime;
	if (m_LifeTime <= 0.0f)
	{
		m_pGameObject->SetShouldDespawn(true);
		if (m_pHeldEnemy)
		{
			m_pHeldEnemy->GetComponent<MyEngine::TransformComponent>()->SetPosition(m_pGameObject->GetComponent<MyEngine::TransformComponent>()->GetPosition());
			m_pHeldEnemy->SetActive(true);
		}
	}
	MyEngine::TransformComponent* trans = m_pGameObject->GetComponent<MyEngine::TransformComponent>();

	if (m_SidewaysTimer > 0.0f && !currentLevel.IsWallInFront(trans->GetPosition(), m_IsDirLeft) && m_pGameObject->GetState() == 0)
	{
		m_SidewaysTimer -= fixedDeltaTime;
		trans->SetPosition(trans->GetPosition().x + -(int(m_IsDirLeft) * 2 - 1) * 600.0f * fixedDeltaTime, trans->GetPosition().y);
	}
	else if (!currentLevel.IsWallAbove(trans->GetPosition()))
	{
		m_SidewaysTimer = 0.f;
		trans->SetPosition(trans->GetPosition().x, trans->GetPosition().y + fixedDeltaTime * 50.0f);
		trans->SetPosition(ScreenBordersCheck(trans->GetPosition()));
	}

	if (m_pGameObject->GetState() == 1)
	{
		for (MyEngine::GameObject* pPlayer : DataHolder::GetInstance()->GetPlayers())
		{
			if (m_pGameObject->GetComponent<MyEngine::PhysicsComponent>()->IsOverlapping(pPlayer->GetComponent<MyEngine::PhysicsComponent>()))
			{
				SceneManager::GetInstance()->GetActiveScene()->Add(CreateFruitDrop(m_pHeldEnemy->GetComponent<EnemyBehaviourComponent>()->GetType(), m_pGameObject->GetComponent<TransformComponent>()->GetPosition()));
				m_pGameObject->SetShouldDespawn(true);
				DataHolder::GetInstance()->RemoveEnemy(m_pHeldEnemy);
				m_pHeldEnemy->SetShouldDespawn(true);
				if (DataHolder::GetInstance()->GetEnemies().size() == 0)
					LevelManager::GetInstance()->Notify(MyEngine::Event(LevelManager::LevelManagerEvent::IncreaseLevel));
				return;
			}
		}
		return;
	}
	for (MyEngine::GameObject* enemy : DataHolder::GetInstance()->GetEnemies())
	{
		if (!enemy->IsActive())
			continue;
		if (m_pGameObject->GetComponent<MyEngine::PhysicsComponent>()->IsOverlapping(enemy->GetComponent<MyEngine::PhysicsComponent>()))
		{
			m_pHeldEnemy = enemy;
			enemy->SetActive(false);
			m_pGameObject->GetComponent<MyEngine::RenderComponent>()->AddTexture(MyEngine::ResourceManager::GetInstance()->LoadTexture("bubbles.png"), true, false, 1, 4, 0.25f, currentLevel.WindowWidth / 16, int(currentLevel.WindowHeight / 12.5f), { 0.5f, 0.5f }, 1, { 0.0f, 0.2f + 0.1f * enemy->GetComponent<EnemyBehaviourComponent>()->GetType() }, { 1.0f, 0.1f });
			m_pGameObject->SetState(1);
			m_LifeTime = 3.0f;
			return;
		}
	}
}

void BubbleBehaviourComponent::Render() const {}

FruitDropComponent::FruitDropComponent(int enemyType) :
	m_Score(DataHolder::GetInstance()->GetScore(enemyType) * 1000),
	m_HitTimer(1.0f)
{}

void FruitDropComponent::Update(const float) {}

void FruitDropComponent::FixedUpdate(const float fixedDeltaTime)
{
	m_HitTimer -= fixedDeltaTime;
	TransformComponent* trans = m_pGameObject->GetComponent<TransformComponent>();
	if (!currentLevel.IsWallBelow(trans->GetPosition()))
	{
		trans->SetPosition(trans->GetPosition().x, trans->GetPosition().y - fixedDeltaTime * 60.0f);
		trans->SetPosition(ScreenBordersCheck(trans->GetPosition()));
	}
	if (m_HitTimer >= 0.0f)
		return;
	for (GameObject* pPlayer : DataHolder::GetInstance()->GetPlayers())
	{
		if (m_pGameObject->GetComponent<PhysicsComponent>()->IsOverlapping(pPlayer->GetComponent<PhysicsComponent>()))
		{
			pPlayer->GetComponent<PlayerBehaviourComponent>()->IncreaseScore(m_Score);
			m_pGameObject->SetShouldDespawn(true);
			SoundManager::GetInstance()->Notify(MyEngine::Event(SoundEvents::ItemPickup));
		}
	}

}

void FruitDropComponent::Render() const {}

EnemyBehaviourComponent::EnemyBehaviourComponent(int type, float delay) :
	m_Type(type),
	m_Delay(delay)
{
}

void EnemyBehaviourComponent::Update(const float) {}

void EnemyBehaviourComponent::FixedUpdate(const float fixedDeltaTime)
{
	m_AttackCooldown -= fixedDeltaTime;
	if (!m_ClosestPlayer)
		SearchPlayer();
	m_Delay -= fixedDeltaTime;
	if (m_Delay >= 0.0f)
		return;
	TransformComponent* closestTrans = m_ClosestPlayer->GetComponent<TransformComponent>();
	TransformComponent* trans = m_pGameObject->GetComponent<TransformComponent>();
	int state = m_pGameObject->GetState();
	//Attack
	float horDist;
	switch (m_Type)
	{
	default:
	case(EnemyType::ZenChan):
		switch (state)
		{
		case(0):
		case(1):
			horDist = (closestTrans->GetPosition().x - trans->GetPosition().x) * (state * 2 - 1);
			if (horDist < 30.0f && horDist > 0.0f && abs(trans->GetPosition().y - closestTrans->GetPosition().y) < 5.0f)
			{
				if (m_AttackCooldown <= 0.0f)
				{
					m_ClosestPlayer->GetComponent<PlayerBehaviourComponent>()->Damage();
					SearchPlayer();
					m_pGameObject->SetState(m_pGameObject->GetState() % 4);
				}
				return;
			}
			break;
		default:
			break;
		}
		break;
	case(EnemyType::Mighta):
		switch (state)
		{
		case(0):
		case(1):
			horDist = (trans->GetPosition().x - closestTrans->GetPosition().x) * (state * 2 - 1);
			if (horDist < 120.0f && horDist > 0.0f && abs(trans->GetPosition().y - closestTrans->GetPosition().y) < 5.0f)
			{
				if (m_AttackCooldown <= 0.0f)
				{
					CreateRock(trans->GetPosition(), bool(state));
					SearchPlayer();
					m_pGameObject->SetState(m_pGameObject->GetState() % 4);
					m_AttackCooldown = 1.5f;
				}
				return;
			}
			break;
		default:
			break;
		}
		break;
	}

	//Movement
	switch (m_Type)
	{
	default:
	case(EnemyType::Mighta):
	case(EnemyType::ZenChan):
		switch (state)
		{
		case(0):
		case(1):
			trans->SetPosition(trans->GetPosition().x + -(state * 2 - 1) * fixedDeltaTime * 120.0f, trans->GetPosition().y);
			if (currentLevel.IsWallInFront(trans->GetPosition(), bool(state)))
				m_pGameObject->SetState(1 - state);
			if (!currentLevel.IsWallBelow(trans->GetPosition()))
				m_pGameObject->SetState(state + 4);
			if ((closestTrans->GetPosition().y - trans->GetPosition().y > 40.0f) && currentLevel.CanJumpUp(trans->GetPosition()) && currentLevel.GetRow(trans->GetPosition()) > 5)
				Jump();
			break;
		case(2):
		case(3):
			m_JumpTime -= fixedDeltaTime;
			trans->SetPosition(trans->GetPosition().x, trans->GetPosition().y + fixedDeltaTime * 200.0f);
			if (m_JumpTime <= 0.0f)
				m_pGameObject->SetState(state + 2);
			break;
		case(4):
		case(5):
			trans->SetPosition(trans->GetPosition().x, trans->GetPosition().y - fixedDeltaTime * 94.0f);
			if (currentLevel.IsWallBelow(trans->GetPosition()))
			{
				SearchPlayer();
				m_pGameObject->SetState(m_pGameObject->GetState() % 2);
			}
			break;
		}
		break;
	}
	trans->SetPosition(ScreenBordersCheck(trans->GetPosition()));
}

void EnemyBehaviourComponent::Render() const {}

void EnemyBehaviourComponent::Jump()
{
	m_JumpTime += 0.6f;
	m_pGameObject->SetState(m_pGameObject->GetState() % 2 + 2);
}

void EnemyBehaviourComponent::SearchPlayer()
{
	m_ClosestPlayer = DataHolder::GetInstance()->GetPlayers()[0];
	if (glm::distance2(m_ClosestPlayer->GetComponent<TransformComponent>()->GetPosition(), m_pGameObject->GetComponent<TransformComponent>()->GetPosition()) > glm::distance2(DataHolder::GetInstance()->GetPlayers()[1]->GetComponent<TransformComponent>()->GetPosition(), m_pGameObject->GetComponent<TransformComponent>()->GetPosition()) && DataHolder::GetInstance()->GetPlayers()[1]->IsActive())
		m_ClosestPlayer = DataHolder::GetInstance()->GetPlayers()[1];

	if (m_ClosestPlayer->GetComponent<TransformComponent>()->GetPosition().x < m_pGameObject->GetComponent<TransformComponent>()->GetPosition().x)
		m_pGameObject->SetState(5);
	else
		m_pGameObject->SetState(4);
}

RockComponent::RockComponent(bool isGoingLeft)
	:m_IsGoingLeft(isGoingLeft)
{
}

void RockComponent::Update(const float)
{
}

void RockComponent::FixedUpdate(const float fixedDeltaTime)
{
	for (GameObject* pPlayer : DataHolder::GetInstance()->GetPlayers())
	{
		if (m_pGameObject->GetComponent<PhysicsComponent>()->IsOverlapping(pPlayer->GetComponent<PhysicsComponent>()))
		{
			m_pGameObject->SetShouldDespawn(true);
			pPlayer->GetComponent<PlayerBehaviourComponent>()->Damage();
			return;
		}
	}
	TransformComponent* trans = m_pGameObject->GetComponent<TransformComponent>();
	if (!currentLevel.IsWallBelow(trans->GetPosition()))
	{
		trans->SetPosition(trans->GetPosition().x, trans->GetPosition().y - fixedDeltaTime * 60.0f);
		trans->SetPosition(ScreenBordersCheck(trans->GetPosition()));
		return;
	}
	if (currentLevel.IsWallInFront(trans->GetPosition(), m_IsGoingLeft))
		m_pGameObject->SetShouldDespawn(true);
	trans->SetPosition(trans->GetPosition().x + -(m_IsGoingLeft * 2 - 1) * fixedDeltaTime * 90.0f, trans->GetPosition().y);
}

void RockComponent::Render() const
{
}
