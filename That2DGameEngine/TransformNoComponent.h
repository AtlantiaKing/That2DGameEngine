#pragma once
#include <glm/glm.hpp>

namespace that
{
	class TransformNoComponent final
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_position;
	};
}
