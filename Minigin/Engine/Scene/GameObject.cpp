#include "MiniginPCH.h"
#include "GameObject.h"
#include "../Managers/ResourceManager.h"
#include "../Graphics/Renderer.h"
#include "../Components/BaseComponent.h"
#include "../Components/TransformComponent.h"
#include <exception>
#include <glm\common.hpp>

MyEngine::GameObject::GameObject(const glm::fvec2& pos, const float angle, bool shouldRemoveOnSceneKill):
	m_State(-1),
	m_IsActive(true),
	m_ShouldDespawn(false),
	m_ShouldRemoveOnSceneKill(shouldRemoveOnSceneKill)

{
	AddComponent(new TransformComponent(pos, angle));
}

MyEngine::GameObject::~GameObject()
{
	for (BaseComponent* pComp : m_Components)
		Safe_Delete(pComp);
}

void MyEngine::GameObject::Update(const float deltaTime)
{
	if (!m_IsActive)
		return;
	for (BaseComponent* pComp : m_Components)
		pComp->Update(deltaTime);
}

void MyEngine::GameObject::FixedUpdate(const float fixedDeltaTime)
{
	if (!m_IsActive)
		return;
	for (BaseComponent* pComp : m_Components)
		pComp->FixedUpdate(fixedDeltaTime);
}

void MyEngine::GameObject::Render() const
{
	if (!m_IsActive)
		return;
	for (BaseComponent* pComp : m_Components)
		pComp->Render();
}

void MyEngine::GameObject::AddComponent(BaseComponent* pComponent)
{
	for (BaseComponent* pComp : m_Components)
	{
		if (typeid(*pComp) == typeid(*pComponent))
		{
			throw std::exception("AddComponent: This component already exists");
		}
	}
	m_Components.push_back(pComponent);
	pComponent->SetGameObject(this);
}
