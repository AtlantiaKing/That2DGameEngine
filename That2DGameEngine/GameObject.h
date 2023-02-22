#pragma once
#include <memory>
#include "Transform.h"

namespace that
{
	class GameObject final
	{
	public:
		virtual void Update();
		virtual void Render() const;

		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;
	private:
	};
}
