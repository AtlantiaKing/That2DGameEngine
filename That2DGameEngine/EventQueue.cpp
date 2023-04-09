#include "EventQueue.h"

that::EventQueue::~EventQueue()
{

}

void that::EventQueue::NotifyListeners()
{
	Event* e{};
	while (PollEvent(&e))
	{
		auto& listeners{ m_Listeners[*e] };
		for (auto listener : listeners)
		{
			listener->OnEvent(static_cast<void*>(e));
		}
	}
}

bool that::EventQueue::PollEvent(Event** e)
{
	if (m_NrEventsQueued == 0) return false;

	*e = m_EventQueue[m_EventBufferStart].get();

	++m_EventBufferStart %= m_EventBufferSize;
	--m_NrEventsQueued;

	return true;
}
