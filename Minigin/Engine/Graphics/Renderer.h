#pragma once
#include "../Helpers/Singleton.h"
#include <glm/common.hpp>
#include "SDL_pixels.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;
struct SDL_Point;

namespace MyEngine
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window, const SDL_Color& backGroundColor);
		void Render() const;
		void Destroy();

		void RenderLine(const SDL_Point& p1, const SDL_Point& p2, const SDL_Point& pivot, const SDL_Color& color);
		void RenderTexture(const Texture2D& texture, const SDL_Rect* dstRect, const SDL_Rect* srcRect, const float angle, const SDL_Point& pivot) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
	private:
		SDL_Renderer* m_Renderer{};
		SDL_Color m_BackGroundColor;
	};
}

