#include "ShootPumpCommand.h"
#include "GameObject.h"
#include "Pump.h"

void digdug::ShootPumpCommand::Execute()
{
	m_pPump->GetComponent<Pump>()->Enable();
}
