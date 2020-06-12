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
#include "glm/gtx/rotate_vector.hpp"

void MyEngine::RenderComponent::Update(const float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
}

void MyEngine::RenderComponent::FixedUpdate(const float fixedDeltaTime)
{
	for (AnimatedTexture& animText : m_Textures)
	{
		if (animText.IsAnimated && animText.State == m_pGameObject->GetState())
		{
			animText.CurrentTime += fixedDeltaTime;
			while (animText.CurrentTime >= animText.FrameTime)
			{
				animText.CurrentTime -= animText.FrameTime;
				if (animText.IsReversed)
				{
					animText.CurrentFrame--;
					if (animText.CurrentFrame < 0)
						animText.CurrentFrame += animText.Rows * animText.Columns;
				}
				else
				{
					animText.CurrentFrame++;
					animText.CurrentFrame %= animText.Rows * animText.Columns;
				}
			}
		}
		else
		{
			animText.CurrentTime = 0.0f;
			animText.CurrentFrame = 0;
			if (animText.IsReversed)
				animText.CurrentFrame = animText.Rows * animText.Columns;
		}
	}
}


void MyEngine::RenderComponent::Render() const
{
	//Loop over textures -> if active state -> calculate dst & srcRect -> Draw
	for (size_t i{}; i < m_Textures.size(); i++)
	{
		if (m_Textures[i].State == -1 || m_pGameObject->GetState() == m_Textures[i].State)
		{
			glm::vec2 pos{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };
			SDL_Rect dstRect{ static_cast<int>(pos.x), static_cast<int>(pos.y) };
			SDL_QueryTexture(m_Textures[i].Texture->GetSDLTexture(), nullptr, nullptr, &dstRect.w, &dstRect.h);

			SDL_Rect srcRect = { int(m_Textures[i].SrcPos.x * dstRect.w), int(m_Textures[i].SrcPos.y * dstRect.h), int(m_Textures[i].SrcDim.x * dstRect.w), int(m_Textures[i].SrcDim.y * dstRect.h) };
			if (m_Textures[i].IsAnimated)
			{
				srcRect = { srcRect.x + (m_Textures[i].CurrentFrame % m_Textures[i].Columns) * srcRect.w / (m_Textures[i].Columns),
					srcRect.y + (m_Textures[i].CurrentFrame / m_Textures[i].Columns) * srcRect.h / (m_Textures[i].Rows),
					srcRect.w / m_Textures[i].Columns,
					srcRect.h / m_Textures[i].Rows };
			}
			DimensionsSet(i, dstRect);
			SDL_Point pivot = { int(m_Textures[i].Pivot.x * dstRect.w), int(m_Textures[i].Pivot.y * dstRect.h) };
			glm::fvec2 worldOffset = { m_Textures[i].Offset.x * dstRect.w, m_Textures[i].Offset.y * dstRect.h };
			worldOffset = glm::rotate(worldOffset, -m_pGameObject->GetComponent<TransformComponent>()->GetRotation() * float(M_PI) / 180.0f);
			dstRect.x += int(worldOffset.x);
			dstRect.y += int(worldOffset.y);
			dstRect.x -= pivot.x;
			dstRect.y += pivot.y;

			Renderer::GetInstance()->RenderTexture(*m_Textures[i].Texture, &dstRect, &srcRect, m_pGameObject->GetComponent<TransformComponent>()->GetRotation(), pivot, m_Textures[i].IsMirrored);
		}
	}
}

void MyEngine::RenderComponent::AddTexture(const std::string& filePath, bool isAnimated, bool isReversed, const int rows, const int columns, const float frameTime, const int drawWidth, const int drawHeight, const glm::fvec2& pivot, const int activeState, const glm::fvec2 srcPos, const glm::fvec2 srcDim, const glm::fvec2 offSet, bool mirror)
{
	m_Textures.push_back({ ResourceManager::GetInstance()->LoadTexture(filePath), frameTime, 0.0f, rows, columns, 0, activeState, drawWidth, drawHeight, isAnimated, isReversed, mirror, pivot, srcPos, srcDim, offSet });
}

void MyEngine::RenderComponent::AddTexture(Texture2D* texture, bool isAnimated, bool isReversed, const int rows, const int columns, const float frameTime, const int drawWidth, const int drawHeight, const glm::fvec2& pivot, const int activeState, const glm::fvec2 srcPos, const glm::fvec2 srcDim, const glm::fvec2 offSet, bool mirror)
{
	m_Textures.push_back({ texture, frameTime, 0.0f, rows, columns, 0, activeState, drawWidth, drawHeight, isAnimated, isReversed, mirror, pivot, srcPos, srcDim, offSet });
}

void MyEngine::RenderComponent::DimensionsSet(size_t index, SDL_Rect& dstRect) const
{
	if (m_Textures[index].DrawWidth != 0 && m_Textures[index].DrawHeight != 0)
	{
		dstRect.w = m_Textures[index].DrawWidth;
		dstRect.h = m_Textures[index].DrawHeight;
	}
}
