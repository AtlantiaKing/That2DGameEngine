#pragma once

#include "Component.h"

#include "Subject.h"

#include "TextureMask.h"

namespace digdug
{
	class FireBreath final : public that::Component
	{
	public:
		FireBreath() = default;
		virtual ~FireBreath() = default;

		FireBreath(const FireBreath& other) = delete;
		FireBreath(FireBreath&& other) = delete;
		FireBreath& operator=(const FireBreath& other) = delete;
		FireBreath& operator=(FireBreath&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void OnEnable() override;

		that::Subject<FireBreath> OnDisable{};
	private:
		float m_PreAttackTime{};
		float m_TimeUntilAttack{ 2.0f };

		float m_UnmaskSpeed{ 1.0f };

		that::TextureMask* m_pMask{};
	};
}
