#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Texture2D.h"
#include "../Helpers/Structs.h"
#include "../Scene/GameObject.h"
#include "TransformComponent.h"
#include <exception>

void MyEngine::RenderComponent::Update(const float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
}

void MyEngine::RenderComponent::FixedUpdate(const float fixedDeltaTime)
{
	UNREFERENCED_PARAMETER(fixedDeltaTime);
}


void MyEngine::RenderComponent::Render() const
{
	for (size_t i{}; i < m_AmountTextures; i++)
	{
		Vector2 pos{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };
		Renderer::GetInstance()->RenderTexture(*m_Textures[i], pos.x, pos.y);
	}
}

MyEngine::RenderComponent::~RenderComponent()
{
	for (size_t i{}; i < m_AmountTextures; i++)
	{
		if (m_Textures[i])
		{
			delete m_Textures[i];
			m_Textures[i] = nullptr;
		}
	}
}

MyEngine::RenderComponent::RenderComponent(const size_t maxAmountTextures)
	:m_Textures(maxAmountTextures)
{
}

void MyEngine::RenderComponent::AddTexture(MyEngine::Texture2D* texture)
{
	if (m_AmountTextures == m_Textures.size())
		throw std::exception("Max nr of textures reached");
	m_Textures[m_AmountTextures] = texture;
	m_AmountTextures++;
}
