#include "EventQueue.h"

that::EventQueue::~EventQueue()
{

}

void that::EventQueue::NotifyListeners()
{
	std::unique_ptr<Event> e{};
	while (PollEvent(e))
	{
		auto& listeners{ m_Listeners[*e] };
		for (auto listener : listeners)
		{
			listener->OnEvent(static_cast<void*>(e.get()));
		}
	}
}

bool that::EventQueue::PollEvent(std::unique_ptr<Event>& e)
{
	if (m_Queue.GetNrAssigned() == 0) return false;

	e = m_Queue.Pop() ;

	return true;
}
