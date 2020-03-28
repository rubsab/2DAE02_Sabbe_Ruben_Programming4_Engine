#pragma once
#include "BaseComponent.h"
#include "SDL_pixels.h"

namespace MyEngine
{
	class Font;
	class Texture2D;	
	class TextComponent final : public BaseComponent
	{
	public:
		//Inherited
		virtual void Update(const float deltaTime) override;
		virtual void Render() const override;

		TextComponent(const std::string& text, Font* font, SDL_Color color);
		~TextComponent();

		//Getters & Setters
		Texture2D* GetTexture() const;
		void SetText(const std::string& text);
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		Font* m_pFont;
		Texture2D* m_pTexture;
		SDL_Color m_Color;
	};
}

