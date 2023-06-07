#include "ShootPumpCommand.h"

#include "DigDug.h"

void digdug::ShootPumpCommand::Execute()
{
	m_pPlayer->Pump(false);
}
