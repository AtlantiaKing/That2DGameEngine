#pragma once

#include "Singleton.h"

#include <string>
#include <memory>
#include <unordered_map>

namespace that
{
	class Texture2D;
	class TextureManager final : public Singleton<TextureManager>
	{
	public:
		std::shared_ptr<Texture2D> LoadTexture(const std::string& file);
	private:
		friend class Singleton<TextureManager>;
		TextureManager() = default;

		std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_pTextures{};
	};
}
