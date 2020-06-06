#include "MiniginPCH.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/Font.h"
#include "../Graphics/Texture2D.h"
#include "../Scene/GameObject.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "../Managers/ResourceManager.h"

void MyEngine::TextComponent::Update(const float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
}

void MyEngine::TextComponent::FixedUpdate(const float fixedDeltaTime)
{
	UNREFERENCED_PARAMETER(fixedDeltaTime);
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTexture->SetTexture(texture);
		m_NeedsUpdate = false;
	}
}

void MyEngine::TextComponent::Render() const
{
	glm::vec2 pos{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };
	SDL_Rect dstRect{ static_cast<int>(pos.x), static_cast<int>(pos.y) };
	SDL_QueryTexture(m_pTexture->GetSDLTexture(), nullptr, nullptr, &dstRect.w, &dstRect.h);
	Renderer::GetInstance()->RenderTexture(*m_pTexture, &dstRect);
}

MyEngine::TextComponent::TextComponent(const std::string& text, Font* font, SDL_Color color)
	: m_NeedsUpdate(true), m_Text(text), m_pFont(font), m_pTexture(nullptr), m_Color{ color }
{
	if (m_NeedsUpdate)
	{
		//const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance()->GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_pTexture = new Texture2D(texture);
		ResourceManager::GetInstance()->m_Textures.push_back(IndexedTexture{ m_pTexture, ""});
		m_NeedsUpdate = false;
	}
}

MyEngine::TextComponent::~TextComponent()
{
}

MyEngine::Texture2D* MyEngine::TextComponent::GetTexture() const
{
	return m_pTexture;
}

void MyEngine::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
