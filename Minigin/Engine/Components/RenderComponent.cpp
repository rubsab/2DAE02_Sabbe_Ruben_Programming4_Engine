#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Texture2D.h"
#include "../Scene/GameObject.h"
#include "TransformComponent.h"
#include "../Managers/ResourceManager.h"
#include <exception>
#include <SDL.h>
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
	for (const AnimatedTexture& tex : m_Textures)
	{
		if (tex.State == -1 || m_pGameObject->GetState() == tex.State)
		{
			const glm::vec2& pos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
			SDL_Rect dstRect{ static_cast<int>(pos.x), static_cast<int>(pos.y) };
			SDL_QueryTexture(tex.Texture->GetSDLTexture(), nullptr, nullptr, &dstRect.w, &dstRect.h);

			SDL_Rect srcRect = { int(tex.SrcPos.x * dstRect.w), int(tex.SrcPos.y * dstRect.h), int(tex.SrcDim.x * dstRect.w), int(tex.SrcDim.y * dstRect.h) };
			if (tex.IsAnimated)
			{
				srcRect = { srcRect.x + (tex.CurrentFrame % tex.Columns) * srcRect.w / (tex.Columns),
					srcRect.y + (tex.CurrentFrame / tex.Columns) * srcRect.h / (tex.Rows),
					srcRect.w / tex.Columns,
					srcRect.h / tex.Rows };
			}
			if (tex.DrawWidth != 0 && tex.DrawHeight != 0)
			{
				dstRect.w = tex.DrawWidth;
				dstRect.h = tex.DrawHeight;
			}
			SDL_Point pivot = { int(tex.Pivot.x * dstRect.w), int(tex.Pivot.y * dstRect.h) };
			glm::fvec2 worldOffset = { tex.Offset.x * dstRect.w, tex.Offset.y * dstRect.h };
			worldOffset = glm::rotate(worldOffset, -m_pGameObject->GetComponent<TransformComponent>()->GetRotation() * float(M_PI) / 180.0f);
			dstRect.x += int(worldOffset.x) - pivot.x;
			dstRect.y += int(worldOffset.y) + pivot.y;

			Renderer::GetInstance()->RenderTexture(*tex.Texture, &dstRect, &srcRect, m_pGameObject->GetComponent<TransformComponent>()->GetRotation() + tex.Angle, pivot, tex.IsMirrored);
		}
	}
}

void MyEngine::RenderComponent::AddTexture(const std::string& filePath, bool isAnimated, bool isReversed, const int rows, const int columns, const float frameTime, const int drawWidth, const int drawHeight, const glm::fvec2& pivot, const int activeState, const glm::fvec2 srcPos, const glm::fvec2 srcDim, const glm::fvec2 offSet, bool mirror, float angle)
{
	m_Textures.push_back({ ResourceManager::GetInstance()->LoadTexture(filePath), frameTime, 0.0f, angle, rows, columns, 0, activeState, drawWidth, drawHeight, isAnimated, isReversed, mirror, pivot, srcPos, srcDim, offSet });
}

void MyEngine::RenderComponent::AddTexture(Texture2D* texture, bool isAnimated, bool isReversed, const int rows, const int columns, const float frameTime, const int drawWidth, const int drawHeight, const glm::fvec2& pivot, const int activeState, const glm::fvec2 srcPos, const glm::fvec2 srcDim, const glm::fvec2 offSet, bool mirror, float angle)
{
	m_Textures.push_back({ texture, frameTime, 0.0f, angle, rows, columns, 0, activeState, drawWidth, drawHeight, isAnimated, isReversed, mirror, pivot, srcPos, srcDim, offSet });
}
