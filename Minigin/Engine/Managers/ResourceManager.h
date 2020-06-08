#pragma once
#include "../Helpers/Singleton.h"
#include <vector>

namespace MyEngine
{
	class Texture2D;
	class Font;
	struct IndexedTexture
	{
		Texture2D* Texture;
		const std::string Path;
	};
	struct IndexedFont
	{
		Font* Font;
		const std::string Path;
	};
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		ResourceManager() = default;
		~ResourceManager();
		void Init(const std::string& data);
		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, unsigned int size);
	private:
		friend class TextComponent;
		std::string m_DataPath;
		std::vector<IndexedTexture> m_Textures;
		std::vector<IndexedFont> m_Fonts;
	};
}
