#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "../Managers/SceneManager.h"
#include "Texture2D.h"

void MyEngine::Renderer::Init(SDL_Window * window)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void MyEngine::Renderer::Render() const
{
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance()->Render();
	
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

void MyEngine::Renderer::RenderTexture(const Texture2D& texture, const SDL_Rect* dstRect, const SDL_Rect* srcRect) const
{
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), srcRect, dstRect);
}
