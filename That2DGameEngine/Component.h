#pragma once

#include <memory>

namespace that
{
	class GameObject;
	class Transform;

	class Component
	{
	public:
		Component() = default;
		virtual ~Component() = default;

		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Init() {};
		virtual void OnFrameStart() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Render() const {};
		virtual void OnGUI() {};
		virtual void OnDestroy() {};
		virtual void OnDisable() {};
		virtual void OnEnable() {};

		void SetEnabled(bool enabled);
		void Destroy();

		friend GameObject;

		Transform* GetTransform() const;
		GameObject* GetOwner() const { return m_pOwner; };
		bool IsEnabled() const { return m_Enabled; }
		bool IsMarkedAsDead() const { return m_IsMarkedDead; };

	protected:
		bool m_Enabled{ true };

	private:
		void SetOwner(GameObject* pParent);

		GameObject* m_pOwner{};
		bool m_IsMarkedDead{};
	};
}
