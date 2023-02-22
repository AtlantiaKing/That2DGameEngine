#pragma once
#include <vector>
#include <memory>

namespace that
{
	class Component;

	class GameObject final : public std::enable_shared_from_this<GameObject>
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

		template <class T>
		std::shared_ptr<T> GetComponent();
		template <class T>
		std::shared_ptr<T> AddComponent();
	private:
		std::vector<std::shared_ptr<Component>> m_pComponents{};
	};

	template<class T>
	inline std::shared_ptr<T> GameObject::GetComponent()
	{
		for (const std::shared_ptr<Component>& pComponent : m_pComponents)
		{
			std::shared_ptr<T> derivedComponent{ std::dynamic_pointer_cast<T>(pComponent) };

			if (derivedComponent) return derivedComponent;
		}

		return nullptr;
	}

	template<class T>
	inline std::shared_ptr<T> GameObject::AddComponent()
	{
		return std::make_shared<T>(shared_from_this());
	}
}
