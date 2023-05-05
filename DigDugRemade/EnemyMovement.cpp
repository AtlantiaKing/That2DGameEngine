#include "EnemyMovement.h"

#include "GameObject.h"
#include "GridTransform.h"

void digdug::EnemyMovement::Update()
{
	auto pTransform{ GetOwner()->GetComponent<GridTransform>() };
	
	const auto& gridPos{ pTransform->GetCellPosition() };

	if (gridPos.x >= m_MaxX) m_Direction = -1;
	if (gridPos.x <= m_MinX) m_Direction = 1;

	pTransform->Move(m_Direction, 0);
}
