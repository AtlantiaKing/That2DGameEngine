#include "ShootPumpCommand.h"
#include "GameObject.h"
#include "Pump.h"

void digdug::ShootPumpCommand::Execute()
{
	if (m_pPump->IsActive())
	{
		Pump* pPump{ m_pPump->GetComponent<Pump>() };
		pPump->PumpToEnemy();
	}
	else
	{
		m_pPump->SetActive(true);
	}
}
