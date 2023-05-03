#pragma once

#include "Component.h"
#include "glm/vec2.hpp"

namespace that
{
	class TextureRenderer;

	class TextureMask final : public Component
	{
	public:
		TextureMask() = default;
		virtual ~TextureMask() = default;

		TextureMask(const TextureMask& other) = delete;
		TextureMask(TextureMask&& other) = delete;
		TextureMask& operator=(const TextureMask& other) = delete;
		TextureMask& operator=(TextureMask&& other) = delete;

		void SetPercentage(bool x, float percentage);
		glm::vec2 GetMask() const;

		virtual void Init() override;
		virtual void Update() override;
	private:
		TextureRenderer* m_pTexture{};

		float m_XMask{ 1.0f };
		float m_YMask{ 1.0f };
	};
}

