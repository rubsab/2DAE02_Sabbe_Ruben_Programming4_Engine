#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Texture2D.h"
#include "../Helpers/Structs.h"
#include "../Scene/GameObject.h"
#include "TransformComponent.h"
#include "../Managers/ResourceManager.h"
#include <exception>
#include <SDL.h>

void MyEngine::RenderComponent::Update(const float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
}

void MyEngine::RenderComponent::FixedUpdate(const float fixedDeltaTime)
{
	for (AnimatedTexture& animText : m_Textures)
	{
		if (animText.IsAnimated)
		{
			animText.CurrentTime += fixedDeltaTime;
			while (animText.CurrentTime >= animText.FrameTime)
			{
				animText.CurrentTime -= animText.FrameTime;
				animText.CurrentFrame++;
				animText.CurrentFrame %= animText.Rows * animText.Columns;
			}
		}
	}
}


void MyEngine::RenderComponent::Render() const
{
	for (size_t i{}; i < m_AmountTextures; i++)
	{
		Vector2 pos{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };
		SDL_Rect dstRect{ static_cast<int>(pos.x), static_cast<int>(pos.y) };
		SDL_QueryTexture(m_Textures[i].Texture->GetSDLTexture(), nullptr, nullptr, &dstRect.w, &dstRect.h);
		SDL_Rect srcRect = {(m_Textures[i].CurrentFrame % m_Textures[i].Columns) * dstRect.w / (m_Textures[i].Columns),
			(m_Textures[i].CurrentFrame / m_Textures[i].Columns) * dstRect.h / (m_Textures[i].Rows),
			dstRect.w / m_Textures[i].Columns,
			dstRect.h / m_Textures[i].Rows};
		Renderer::GetInstance()->RenderTexture(*m_Textures[i].Texture, &dstRect, &srcRect);
	}
}

MyEngine::RenderComponent::~RenderComponent()
{
}

MyEngine::RenderComponent::RenderComponent(const size_t maxAmountTextures)
	:m_Textures(maxAmountTextures)
{
}

void MyEngine::RenderComponent::AddTexture(const std::string& filePath, bool isAnimated, const int rows, const int columns, const float frameTime)
{
	if (m_AmountTextures == m_Textures.size())
		throw std::exception("Max nr of textures reached");
	m_Textures[m_AmountTextures] = { ResourceManager::GetInstance()->LoadTexture(filePath), rows, columns, 0, frameTime, 0.0f, isAnimated };
	m_AmountTextures++;
}
