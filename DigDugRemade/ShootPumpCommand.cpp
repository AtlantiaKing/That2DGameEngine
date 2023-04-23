#include "ShootPumpCommand.h"
#include "GameObject.h"
#include "Pump.h"

void digdug::ShootPumpCommand::Execute()
{
	Pump* pPump{ m_pPump->GetComponent<Pump>() };
	if (pPump->IsActive())
	{
		pPump->PumpToEnemy();
	}
	else
	{
		pPump->Enable();
	}
}
