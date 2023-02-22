#pragma once

#include "Component.h"
#include "Texture2D.h"

namespace that
{
	class Transform;

	class TextureRenderer final : public Component
	{
	public:
		TextureRenderer() = default;
		virtual ~TextureRenderer() = default;

		TextureRenderer(const TextureRenderer& other) = delete;
		TextureRenderer(TextureRenderer&& other) = delete;
		TextureRenderer& operator=(const TextureRenderer& other) = delete;
		TextureRenderer& operator=(TextureRenderer&& other) = delete;

		void SetTexture(std::shared_ptr<Texture2D> pTexture);

		void Render() const;
	private:
		std::shared_ptr<Texture2D> m_pTexture{};
	};
}

