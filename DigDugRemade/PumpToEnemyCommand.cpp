#include "PumpToEnemyCommand.h"

#include "DigDug.h"

void digdug::PumpToEnemyCommand::Execute()
{
	m_pPlayer->Pump(true);
}
