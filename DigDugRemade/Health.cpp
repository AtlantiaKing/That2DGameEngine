#include "Health.h"

void digdug::Health::Hit()
{
	if (m_Health <= 0) return;
	
	--m_Health;

	if (m_Health <= 0)
	{
		int i{};
		i += 1;
	}
}
