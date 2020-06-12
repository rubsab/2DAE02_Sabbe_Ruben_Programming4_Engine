#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "../Managers/SceneManager.h"
#include "../Managers/PhysicsManager.h"
#include "Texture2D.h"

void MyEngine::Renderer::Init(SDL_Window * window, const SDL_Color& backGroundColor)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
	m_BackGroundColor = backGroundColor;
}

void MyEngine::Renderer::Render() const
{
	SDL_SetRenderDrawColor(m_Renderer, m_BackGroundColor.r, m_BackGroundColor.g, m_BackGroundColor.b, m_BackGroundColor.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance()->Render();
	//PhysicsManager::GetInstance()->Draw();
	
	SDL_RenderPresent(m_Renderer);
}

void MyEngine::Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void MyEngine::Renderer::RenderLine(const SDL_Point& p1, const SDL_Point& p2, const SDL_Point& pivot, const SDL_Color& color)
{
	SDL_SetRenderDrawColor(Renderer::GetInstance()->GetSDLRenderer(), color.r, color.g, color.b, color.a);
	int w, h;
	SDL_GetRendererOutputSize(m_Renderer, &w, &h);
	SDL_RenderDrawLine(Renderer::GetInstance()->GetSDLRenderer(), p1.x, h - (2 * pivot.y - p1.y), p2.x, h - (2 * pivot.y - p2.y));
}

void MyEngine::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect* dstRect, const SDL_Rect* srcRect, const float angle, const SDL_Point& pivot, bool isMirrored) const
{
	int w, h;
	SDL_GetRendererOutputSize(m_Renderer, &w, &h);
	SDL_Rect rect{ dstRect->x, h - dstRect->y, dstRect->w, dstRect->h };
	SDL_Point newPivot{ pivot.x, - pivot.y };
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), srcRect, &rect, angle, &pivot, SDL_RendererFlip(isMirrored));
}
