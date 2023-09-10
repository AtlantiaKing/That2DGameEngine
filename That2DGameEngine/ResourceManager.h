#pragma once
#include <string>
#include <memory>
#include <unordered_map>

#include "Singleton.h"

struct SDL_Renderer;

namespace that
{
	class Texture2D;
	class Font;
	class Surface2D;
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init(const std::string& data);
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file) const;
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file, SDL_Renderer* pRenderer) const;
		std::shared_ptr<Font> LoadFont(const std::string& file, unsigned int size);
		std::shared_ptr<Surface2D> LoadSurface(const std::string& file) const;
	private:
		typedef std::pair<std::string, int> FontData;

		friend class Singleton<ResourceManager>;
		ResourceManager() = default;
		std::string m_dataPath;

		std::vector<std::pair<FontData, std::shared_ptr<Font>>> m_pFonts{};
	};
}
