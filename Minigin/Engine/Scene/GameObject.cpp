#include "MiniginPCH.h"
#include "GameObject.h"
#include "../Managers/ResourceManager.h"
#include "../Graphics/Renderer.h"
#include "../Components/BaseComponent.h"
#include <exception>

MyEngine::GameObject::~GameObject()
{
	for (BaseComponent* pComp : m_Components)
	{
		delete pComp;
	}
}

void MyEngine::GameObject::Update(const float deltaTime)
{
	for (BaseComponent* pComp : m_Components)
	{
		pComp->Update(deltaTime);
	}
}

void MyEngine::GameObject::Render() const
{
	for (BaseComponent* pComp : m_Components)
	{
		pComp->Render();
	}
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
