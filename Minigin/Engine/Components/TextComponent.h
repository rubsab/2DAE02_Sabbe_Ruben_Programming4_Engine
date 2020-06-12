#pragma once
#include "BaseComponent.h"
#include "SDL_pixels.h"
#include <glm\common.hpp>

namespace MyEngine
{
	class Font;
	class Texture2D;
	struct ColorRGBA;
	class TextComponent final : public BaseComponent
	{
	public:
		//Inherited
		void Update(const float deltaTime) override;
		void FixedUpdate(const float fixedDeltaTime) override;
		void Render() const override;

		TextComponent(const std::string& text, Font* font, SDL_Color color, const glm::fvec2& pivot = { 0.5f, 0.5f }, const float angle = 0.0f, const glm::fvec2& offSet = { 0.0f, 0.0f });

		//Getters & Setters
		Texture2D* GetTexture() const;
		void SetText(const std::string& text);
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		Font* m_pFont;
		Texture2D* m_pTexture;
		SDL_Color m_Color;
		float m_Angle;
		glm::fvec2 m_Pivot, m_Offset;
	};
}

