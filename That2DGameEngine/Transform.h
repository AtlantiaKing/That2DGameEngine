#pragma once

#include "Component.h"
#include <glm/glm.hpp>

namespace that
{
	class Transform final : public Component
	{
	public:
		Transform() = default;
		virtual ~Transform() = default;

		Transform(const Transform& other) = delete;
		Transform(Transform&& other) = delete;
		Transform& operator=(const Transform& other) = delete;
		Transform& operator=(Transform&& other) = delete;

		const glm::vec2& GetPosition() const { return m_Position; }
		void SetPosition(float x, float y);
		void SetPosition(const glm::vec2& position);
		void Translate(float x, float y);
	private:
		glm::vec2 m_Position{};
	};
}

