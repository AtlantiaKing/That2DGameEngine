#include "EventQueue.h"
#include "Logger.h"
#include <sstream>

void that::EventQueue::SendMessage(const Event& e)
{
	if (m_NrEventsQueued >= m_EventBufferSize)
	{
		std::stringstream sstream{};
		sstream << "Too many events are being queued. Event " << e.GetName() << " was not added to the event queue.";
		Logger::LogWarning(sstream.str());
		return;
	}

	const unsigned int curIdx{ (m_EventBufferStart + m_NrEventsQueued) % m_EventBufferSize };
	m_EventQueue[curIdx] = e;
	++m_NrEventsQueued;
}

void that::EventQueue::NotifyListeners()
{
	Event e{};
	while (PollEvent(e))
	{
		auto& listeners{ m_Listeners[e] };
		for (const auto& listener : listeners)
		{
			listener();
		}
	}
}

void that::EventQueue::AddListener(const Event& e, const std::function<void()>& listener)
{
	auto& listeners{ m_Listeners[e] };
	listeners.push_back(listener);
}

bool that::EventQueue::PollEvent(Event& e)
{
	if (m_NrEventsQueued == 0) return false;

	e = m_EventQueue[m_EventBufferStart];

	++m_EventBufferStart %= m_EventBufferSize;
	--m_NrEventsQueued;

	return true;
}
