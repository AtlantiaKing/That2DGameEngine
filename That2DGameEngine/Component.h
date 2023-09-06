#pragma once

#include <memory>

#include "Reflection.h"
#include "TypeRegister.h"

#define HASH_FUNCTION(NAME) public:\
virtual size_t GetHash() const override { return reflection::Reflection::GetType<NAME>().hash; } 

#define COMPONENT(NAME) public:\
HASH_FUNCTION(NAME)\
private:\
inline static that::reflection::TypeRegister<NAME> g_Registration{};

#define ENABLE_SERIALIZE_VAR(TYPE) friend struct TYPE##Variables;
#define SERIALIZE_VAR_START(TYPE) struct TYPE##Variables final {
#define SERIALIZABLE_VAR(TYPE, VARIABLE) REGISTER_VARIABLE(TYPE##::##VARIABLE, VARIABLE, TYPE, offsetof(TYPE, VARIABLE))
#define SERIALIZE_VAR_END };

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

		virtual size_t GetHash() const { throw std::runtime_error{ "Cannot get hash from a base component" }; }

	protected:
		bool m_Enabled{ true };

	private:
		void SetOwner(GameObject* pParent);

		GameObject* m_pOwner{};
		bool m_IsMarkedDead{};
	};
}
