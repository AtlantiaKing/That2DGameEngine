#pragma once

#include "Component.h"
#include "Observer.h"

#include "PhysicsData.h"

namespace that
{
	class TextureRenderer;
	class TextureMask;
	class BoxCollider;
}

namespace digdug
{
	class HealthComponent;

	class Pump : public that::Component, public that::Observer<that::CollisionData>
	{
	public:
		Pump() = default;
		virtual ~Pump() = default;

		Pump(const Pump& other) = delete;
		Pump(Pump&& other) = delete;
		Pump& operator=(const Pump& other) = delete;
		Pump& operator=(Pump&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;

		bool IsPumpedInEnemy() const { return m_pPumpTo; }
		that::GameObject* GetTarget() const;

		virtual void Notify(const that::CollisionData& data) override;
		void PumpToEnemy();

	private:

		HealthComponent* m_pPumpTo{};
		that::TextureRenderer* m_pTexture{};
		that::TextureMask* m_pMask{};
		that::BoxCollider* m_pCollider{};

		const float m_AliveTime{ 0.35f };
		float m_AccuAliveTime{};
	};
}
