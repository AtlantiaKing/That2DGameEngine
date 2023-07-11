#include "FireBreath.h"

#include "Transform.h"
#include "TextureComponent.h"
#include "BoxCollider.h"
#include "TextureMask.h"
#include "HealthComponent.h"
#include "AudioSource.h"

#include "GameObject.h"

#include "ColliderLayers.h"

#include "Timer.h"
void digdug::FireBreath::Init()
{
	m_pMask = GetOwner()->GetComponent<that::TextureMask>();

	m_pTexture = GetOwner()->GetComponent<that::TextureComponent>();

	m_pCollider = GetOwner()->GetComponent<that::BoxCollider>();
	m_pCollider->OnCollision.AddListener(this);
}

void digdug::FireBreath::Update()
{
	const float elapsedSec{ that::Timer::GetInstance().GetElapsed() };

	// Scale the collider according to the mask
	const float textureSize{ m_pTexture->GetScaledTextureSize().x };
	const float colliderSize{ m_pMask->GetMask().x * textureSize };
	m_pCollider->SetSize(colliderSize, m_pCollider->GetSize().y);
	m_pCollider->SetCenter(-textureSize / 2.0f + colliderSize / 2.0f, 0.0f);

	if (m_PreAttackTime < m_TimeUntilAttack)
	{
		m_PreAttackTime += elapsedSec;

		if (m_PreAttackTime > m_TimeUntilAttack) GetOwner()->GetComponent<that::AudioSource>()->Play();
		return;
	}
	
	const float newMask{ m_pMask->GetMask().x + m_UnmaskSpeed * elapsedSec };
	m_pMask->SetPercentage(true, newMask);

	if (newMask > 1.0f)
	{
		GetOwner()->SetActive(false);
	}
}

void digdug::FireBreath::OnEnable()
{
	m_PreAttackTime = 0.0f;
	m_pMask->SetPercentage(true, 0.0f);
}

void digdug::FireBreath::OnDisable()
{
	GetOwner()->GetComponent<that::AudioSource>()->Stop();
}

void digdug::FireBreath::OnDestroy()
{
	m_pCollider->OnCollision.RemoveListener(this);
}

void digdug::FireBreath::Notify(const that::CollisionData& collision)
{
	if (collision.pOther->GetLayer() == DIGDUG_LAYER)
	{
		collision.pOther->GetOwner()->GetComponent<HealthComponent>()->Hit();
	}
}
