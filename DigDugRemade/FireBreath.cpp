#include "FireBreath.h"

#include "Transform.h"
#include "TextureRenderer.h"

#include "GameObject.h"

#include "Timer.h"

void digdug::FireBreath::Init()
{
	m_pMask = GetOwner()->GetComponent<that::TextureMask>();
}

void digdug::FireBreath::Update()
{
	const float elapsedSec{ that::Timer::GetInstance().GetElapsed() };

	if (m_PreAttackTime < m_TimeUntilAttack)
	{
		m_PreAttackTime += elapsedSec;
		return;
	}
	
	const float newMask{ m_pMask->GetMask().x + m_UnmaskSpeed * elapsedSec };
	m_pMask->SetPercentage(true, newMask);

	if (newMask > 1.0f)
	{
		GetOwner()->SetActive(false);

		OnDisable.Notify(*this);
	}
}

void digdug::FireBreath::OnEnable()
{
	m_PreAttackTime = 0.0f;
	m_pMask->SetPercentage(true, 0.0f);
}
