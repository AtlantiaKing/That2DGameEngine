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

		const glm::vec2& GetLocalPosition() const;
		const glm::vec2& GetWorldPosition();
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec2& position);
		void SetWorldPosition(float x, float y);
		void SetWorldPosition(const glm::vec2& position);
		void Translate(float x, float y);

		float GetLocalRotation(bool isDegrees = true) const;
		float GetWorldRotation(bool isDegrees = true);
		void SetLocalRotation(float rotation, bool isDegrees = true);
		void SetWorldRotation(float rotation, bool isDegrees = true);
		void Rotate(float angle, bool isDegrees = true);

		const glm::vec2& GetLocalScale() const;
		const glm::vec2& GetWorldScale();
		void SetLocalScale(float scale);
		void SetLocalScale(const glm::vec2& scale);
		void SetWorldScale(float scale);
		void SetWorldScale(const glm::vec2& scale);

		bool IsDirty() const { return m_HasChanged; }
		void EnableChangedFlag();
	private:
		void UpdateTransform();

		glm::vec2 m_LocalPosition{};
		glm::vec2 m_WorldPosition{};

		float m_LocalRotation{};
		float m_WorldRotation{};

		glm::vec2 m_LocalScale{ 1.0f };
		glm::vec2 m_WorldScale{ 1.0f };

		bool m_HasChanged{};
	};
}

