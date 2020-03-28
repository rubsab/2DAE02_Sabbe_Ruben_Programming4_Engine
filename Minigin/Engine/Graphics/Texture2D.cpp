#include "MiniginPCH.h"
#include "Texture2D.h"
#include <SDL.h>

MyEngine::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Texture);
}

SDL_Texture* MyEngine::Texture2D::GetSDLTexture() const
{
	return m_Texture;
}

MyEngine::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_Texture = texture;
}

void MyEngine::Texture2D::SetTexture(SDL_Texture* texture)
{
	SDL_DestroyTexture(m_Texture);
	m_Texture = texture;
}
