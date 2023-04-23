#pragma once

#include "Component.h"
#include "GridCollider.h"

namespace digdug
{
	class HealthComponent;

	class Pump : public that::Component, public that::Observer<CollisionData>
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
		virtual void OnDestroy() override;

		virtual void Notify(const CollisionData& data) override;
		void PumpToEnemy();
		
		void Enable();
		bool IsActive() const { return m_IsActive; }
	private:
		bool m_IsActive{};

		HealthComponent* m_pPumpTo{};

		// TODO: Needs to be the length of the pump
		const float m_AliveTime{ 0.75f };
		float m_AccuAliveTime{};

		const float m_TimeBetweenPumps{ 0.5f };
		float m_AccuPumpTime{};
		bool m_CanPump{ true };
	};
}
