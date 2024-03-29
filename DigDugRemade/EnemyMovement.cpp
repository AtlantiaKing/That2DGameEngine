#include "EnemyMovement.h"

#include "GameObject.h"
#include "GridTransform.h"

void digdug::EnemyMovement::Update()
{
	auto pTransform{ GetOwner()->GetComponent<GridTransform>() };

	// Try to move
	if (pTransform->Move(m_Direction.x, m_Direction.y, true)) return;

	// Change direction if moving fails
	if (rand() % 1001 / 1000.0f > 0.3f)
	{
		m_Direction.x = rand() % 1001 / 1000.0f > 0.5f ? -1 : 1;
		m_Direction.y = 0;
	}
	else
	{
		m_Direction.y = rand() % 1001 / 1000.0f > 0.5f ? -1 : 1;
		m_Direction.x = 0;
	}
}