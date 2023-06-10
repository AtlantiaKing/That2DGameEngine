#include "DisablePumpCommand.h"

#include "DigDug.h"

void digdug::DisablePumpCommand::Execute()
{
	m_pPlayer->Move(GetData());
}
