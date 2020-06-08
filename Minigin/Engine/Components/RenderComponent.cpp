#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Texture2D.h"
#include "../Scene/GameObject.h"
#include "TransformComponent.h"
#include "../Managers/ResourceManager.h"
#include <exception>
#include <SDL.h>
#include <SDL_mixer.h>
#include <Box2D.h>

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
	//Loop over textures -> if active state -> calculate dst & srcRect -> Draw
	for (size_t i{}; i < m_AmountTextures;i++)
	{
		if (m_Textures[i].State == -1 || m_pGameObject->GetState() == m_Textures[i].State)
		{
			glm::vec2 pos{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };
			SDL_Rect dstRect{ static_cast<int>(pos.x), static_cast<int>(pos.y) };
			SDL_QueryTexture(m_Textures[i].Texture->GetSDLTexture(), nullptr, nullptr, &dstRect.w, &dstRect.h);

			if (m_Textures[i].IsAnimated)
			{
				SDL_Rect srcRect = { (m_Textures[i].CurrentFrame % m_Textures[i].Columns) * dstRect.w / (m_Textures[i].Columns),
					(m_Textures[i].CurrentFrame / m_Textures[i].Columns) * dstRect.h / (m_Textures[i].Rows),
					dstRect.w / m_Textures[i].Columns,
					dstRect.h / m_Textures[i].Rows };
				DimensionsSet(i, dstRect);
				SDL_Point pivot = { int(m_Textures[i].Pivot.x * dstRect.w), int(m_Textures[i].Pivot.y * dstRect.h) };
				dstRect.x -= pivot.x;
				dstRect.y += pivot.y;
				Renderer::GetInstance()->RenderTexture(*m_Textures[i].Texture, &dstRect, &srcRect, m_pGameObject->GetComponent<TransformComponent>()->GetRotation(), pivot);
				continue;
			}
			DimensionsSet(i, dstRect);
			SDL_Point pivot = { int(m_Textures[i].Pivot.x * dstRect.w), int(m_Textures[i].Pivot.y * dstRect.h) };
			dstRect.x -= pivot.x;
			dstRect.y += pivot.y;
			Renderer::GetInstance()->RenderTexture(*m_Textures[i].Texture, &dstRect, nullptr, m_pGameObject->GetComponent<TransformComponent>()->GetRotation(), pivot);
		}
	}
}

MyEngine::RenderComponent::RenderComponent(const size_t maxAmountTextures)
	:m_Textures(maxAmountTextures)
{
}

void MyEngine::RenderComponent::AddTexture(const std::string& filePath, bool isAnimated, const int rows, const int columns, const float frameTime, const int drawWidth, const int drawHeight, const glm::fvec2& pivot, const int activeState)
{
	if (m_AmountTextures == m_Textures.size())
		throw std::exception("Max nr of textures reached");
	m_Textures[m_AmountTextures] = { ResourceManager::GetInstance()->LoadTexture(filePath), frameTime, 0.0f, rows, columns, 0, activeState, drawWidth, drawHeight, isAnimated, pivot };
	m_AmountTextures++;
}

void MyEngine::RenderComponent::DimensionsSet(size_t index, SDL_Rect& dstRect) const
{
	if (m_Textures[index].DrawWidth != 0 && m_Textures[index].DrawHeight != 0)
	{
		dstRect.w = m_Textures[index].DrawWidth;
		dstRect.h = m_Textures[index].DrawHeight;
	}
}
