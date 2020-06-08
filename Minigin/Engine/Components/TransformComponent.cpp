#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "../Scene/GameObject.h"
#include "../Graphics/Renderer.h"
#include "../Managers/PhysicsManager.h"

void MyEngine::TransformComponent::Update(const float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
}

void MyEngine::TransformComponent::FixedUpdate(const float fixedDeltaTime)
{
	UNREFERENCED_PARAMETER(fixedDeltaTime);
}

void MyEngine::TransformComponent::Render() const
{

}

MyEngine::TransformComponent::TransformComponent(const glm::fvec2 pos, const float angle)
	:m_Position{ pos },
	m_Angle{ angle }
{
}

void MyEngine::TransformComponent::SetPosition(float x, float y)
{
	SetPosition({ x, y });
}

void MyEngine::TransformComponent::SetPosition(const glm::fvec2& pos)
{
	PhysicsComponent* pPhysics = m_pGameObject->GetComponent<PhysicsComponent>();
	if (pPhysics)
	{
		pPhysics->m_pBody->SetTransform({ pos.x / PhysicsManager::GetInstance()->GetPixelsPerMeter(), pos.y / PhysicsManager::GetInstance()->GetPixelsPerMeter() }, pPhysics->m_pBody->GetAngle());
		return;
	}
	m_Position = pos;
}

void MyEngine::TransformComponent::SetAngle(float angle)
{
	PhysicsComponent* pPhysics = m_pGameObject->GetComponent<PhysicsComponent>();
	if (pPhysics)
	{
		pPhysics->m_pBody->SetTransform(pPhysics->m_pBody->GetPosition(), m_Angle);
		return;
	}
	m_Angle = angle;
}
