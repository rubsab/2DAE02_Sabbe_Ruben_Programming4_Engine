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
	m_Score(0)
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

	std::string score = std::to_string(m_Score);
	m_pGameObject->GetComponent<TextComponent>()->SetText(score);
	if (m_pGameObject == DataHolder::GetInstance()->GetPlayers()[0])
		m_pGameObject->GetComponent<TextComponent>()->SetOffSet({ currentLevel.WindowWidth - trans->GetPosition().x - 45.0f - (score.size() - 1) * 44.f, currentLevel.WindowHeight - trans->GetPosition().y - 25.0f });
	else
		m_pGameObject->GetComponent<TextComponent>()->SetOffSet({ currentLevel.WindowWidth - trans->GetPosition().x - 45.0f - (score.size() - 1) * 44.f, -trans->GetPosition().y + 25.0f });
	//m_pGameObject->GetComponent<TextComponent>()->SetOffSet({ (score.size() - 1) * - 44.f, 0.0f });
}

void PlayerBehaviourComponent::Render() const {}

void PlayerBehaviourComponent::Jump()
{
	if (currentLevel.IsWallBelow(m_pGameObject->GetComponent<MyEngine::TransformComponent>()->GetPosition()))
		m_JumpTime += 0.51f;
}

void PlayerBehaviourComponent::Shoot()
{
	m_ShootTime = 0.25f;
	m_pGameObject->SetState((m_pGameObject->GetState() % 2) + 4);
}

BubbleBehaviourComponent::BubbleBehaviourComponent(bool isLookingLeft):
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
			m_pGameObject->GetComponent<MyEngine::RenderComponent>()->AddTexture(MyEngine::ResourceManager::GetInstance()->LoadTexture("bubbles.png"), true, false, 1, 4, 0.25f, currentLevel.WindowWidth / 16, int(currentLevel.WindowHeight / 12.5f), { 0.5f, 0.5f }, 1, { 0.0f, 0.2f /*+ 0.1f * int(enemy.Type)*/ }, { 1.0f, 0.1f });
			m_pGameObject->SetState(1);
			m_LifeTime = 3.0f;
			return;
		}
	}
}

void BubbleBehaviourComponent::Render() const {}

FruitDropComponent::FruitDropComponent(int enemyType):
	m_Score(DataHolder::GetInstance()->GetScore(enemyType) * 1000)
{}

void FruitDropComponent::Update(const float) {}

void FruitDropComponent::FixedUpdate(const float fixedDeltaTime) 
{
	TransformComponent* trans = m_pGameObject->GetComponent<TransformComponent>();
	if (!currentLevel.IsWallBelow(trans->GetPosition()))
	{
		trans->SetPosition(trans->GetPosition().x, trans->GetPosition().y - fixedDeltaTime * 60.0f);
		trans->SetPosition(ScreenBordersCheck(trans->GetPosition()));
	}
	for (GameObject* pPlayer : DataHolder::GetInstance()->GetPlayers())
	{
		if (m_pGameObject->GetComponent<PhysicsComponent>()->IsOverlapping(pPlayer->GetComponent<PhysicsComponent>()))
		{
			pPlayer->GetComponent<PlayerBehaviourComponent>()->IncreaseScore(m_Score);
			m_pGameObject->SetShouldDespawn(true);
		}
	}

}

void FruitDropComponent::Render() const {}

EnemyBehaviourComponent::EnemyBehaviourComponent(int type, float delay):
	m_Type(type),
	m_Delay(delay)
{}

void EnemyBehaviourComponent::Update(const float) {}

void EnemyBehaviourComponent::FixedUpdate(const float) {}

void EnemyBehaviourComponent::Render() const {}