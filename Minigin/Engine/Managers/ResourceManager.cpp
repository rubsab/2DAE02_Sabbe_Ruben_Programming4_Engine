#include "MiniginPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <algorithm>

#include "../Graphics/Renderer.h"
#include "../Graphics/Texture2D.h"
#include "../Graphics/Font.h"

MyEngine::ResourceManager::~ResourceManager()
{
	for (IndexedTexture& texture : m_Textures)
	{
		Safe_Delete(texture.Texture);
	}
	for (IndexedFont& font : m_Fonts)
	{
		Safe_Delete(font.Font);
	}
}

void MyEngine::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0) 
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

MyEngine::Texture2D* MyEngine::ResourceManager::LoadTexture(const std::string& file)
{
	std::vector<IndexedTexture>::const_iterator it = std::find_if(m_Textures.begin(), m_Textures.end(), [file](const IndexedTexture& texture) {return texture.Path == file; });
	if (it != m_Textures.end())
	{
		return it->Texture;
	}
	const auto fullPath = m_DataPath + file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance()->GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}
	m_Textures.push_back(MyEngine::IndexedTexture{ new Texture2D(texture), file });
	return m_Textures.back().Texture;
}

MyEngine::Font* MyEngine::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	std::vector<IndexedFont>::const_iterator it = std::find_if(m_Fonts.begin(), m_Fonts.end(), [file](const IndexedFont& font) {return font.Path == file; });
	if (it != m_Fonts.end())
	{
		return it->Font;
	}
	m_Fonts.push_back(IndexedFont{ new Font(m_DataPath + file, size), file });
	return m_Fonts.back().Font;
}

