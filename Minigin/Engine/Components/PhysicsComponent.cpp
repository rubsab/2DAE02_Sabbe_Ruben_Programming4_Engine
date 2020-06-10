#include "MiniginPCH.h"
#include "PhysicsComponent.h"
#include "../Scene/GameObject.h"
#include "../Managers/PhysicsManager.h"
#include "TransformComponent.h"
#include "../Graphics/Renderer.h"
#include "SDL_rect.h"

MyEngine::PhysicsComponent::PhysicsComponent(const PhysicsType physicsType, const glm::fvec2& pos, const float angle, const float halfWidth, const float halfHeight, const float density, const float friction, const float restitution, unsigned short categoryBits, unsigned short maskBits)
{
	b2BodyDef body;
	body.position.Set(pos.x / PhysicsManager::GetInstance()->GetPixelsPerMeter(), pos.y / PhysicsManager::GetInstance()->GetPixelsPerMeter());
	body.type = b2BodyType(physicsType);
	body.angle = angle * float(M_PI) / 180.0f;
	m_pBody = PhysicsManager::GetInstance()->CreateBody(body);

	b2PolygonShape polyShape;
	b2FixtureDef fixtureDef;

	polyShape.SetAsBox(halfWidth / PhysicsManager::GetInstance()->GetPixelsPerMeter(), halfHeight / PhysicsManager::GetInstance()->GetPixelsPerMeter());
	fixtureDef.shape = &polyShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.filter.categoryBits = categoryBits;
	fixtureDef.filter.maskBits = unsigned short(~maskBits);
	m_pBody->CreateFixture(&fixtureDef);
}

void MyEngine::PhysicsComponent::Update(const float)
{
}

void MyEngine::PhysicsComponent::FixedUpdate(const float)
{
	TransformComponent* transform = m_pGameObject->GetComponent<TransformComponent>();
	transform->m_Position = { m_pBody->GetPosition().x * PhysicsManager::GetInstance()->GetPixelsPerMeter(), m_pBody->GetPosition().y * PhysicsManager::GetInstance()->GetPixelsPerMeter() };
	transform->m_Angle = m_pBody->GetAngle() * 180.0f / float(M_PI);
}

void MyEngine::PhysicsComponent::Render() const
{
	if (!PhysicsManager::GetInstance()->IsDebugDrawingActive())
		return;

	b2PolygonShape* shape = dynamic_cast<b2PolygonShape*>(m_pBody->GetFixtureList()[0].GetShape());
	if (!shape)
		return;

	for (int32 i{}; i < shape->m_count - 1; i++)
	{
		Renderer::GetInstance()->RenderLine({ int(m_pBody->GetWorldPoint(shape->m_vertices[i]).x * PhysicsManager::GetInstance()->GetPixelsPerMeter()), int(m_pBody->GetWorldPoint(shape->m_vertices[i]).y * PhysicsManager::GetInstance()->GetPixelsPerMeter()) }, { int(m_pBody->GetWorldPoint(shape->m_vertices[i + 1]).x * PhysicsManager::GetInstance()->GetPixelsPerMeter()), int(m_pBody->GetWorldPoint(shape->m_vertices[i + 1]).y * PhysicsManager::GetInstance()->GetPixelsPerMeter()) }, { int(m_pBody->GetPosition().x * PhysicsManager::GetInstance()->GetPixelsPerMeter()), int(m_pBody->GetPosition().y * PhysicsManager::GetInstance()->GetPixelsPerMeter()) },	PhysicsManager::GetInstance()->GetDebugColor());
	}
	Renderer::GetInstance()->RenderLine({ int(m_pBody->GetWorldPoint(shape->m_vertices[0]).x * PhysicsManager::GetInstance()->GetPixelsPerMeter()), int(m_pBody->GetWorldPoint(shape->m_vertices[0]).y * PhysicsManager::GetInstance()->GetPixelsPerMeter()) }, { int(m_pBody->GetWorldPoint(shape->m_vertices[shape->m_count - 1]).x * PhysicsManager::GetInstance()->GetPixelsPerMeter()), int(m_pBody->GetWorldPoint(shape->m_vertices[shape->m_count - 1]).y * PhysicsManager::GetInstance()->GetPixelsPerMeter()) }, { int(m_pBody->GetPosition().x * PhysicsManager::GetInstance()->GetPixelsPerMeter()), int(m_pBody->GetPosition().y * PhysicsManager::GetInstance()->GetPixelsPerMeter()) }, PhysicsManager::GetInstance()->GetDebugColor());
}

bool MyEngine::PhysicsComponent::IsOverlapping(const PhysicsComponent* other) const
{
	return b2TestOverlap(m_pBody->GetFixtureList()[0].GetShape(), 0, other->m_pBody->GetFixtureList()[0].GetShape(), 0, m_pBody->GetTransform(), other->m_pBody->GetTransform());
}
