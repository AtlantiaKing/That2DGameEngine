#pragma once

#include "Singleton.h"
#include "Logger.h"

#include "Event.h"
#include "EventListener.h"

#include <unordered_map>
#include <memory>
#include <sstream>

namespace that
{
	class EventQueue final : public Singleton<EventQueue>
	{
	public:
		EventQueue() = default;
		~EventQueue();
		EventQueue(const EventQueue&) = delete;
		EventQueue(EventQueue&&) = delete;
		EventQueue& operator= (const EventQueue&) = delete;
		EventQueue& operator= (const EventQueue&&) = delete;

		template <class T>
		void SendEvent(const T& e);
		template <class T>
		void AddListener(EventListener<T>* listener);

		void NotifyListeners();
	private:
		bool PollEvent(Event** e);

		const static unsigned int m_EventBufferSize{ 10 };
		std::unique_ptr<Event> m_EventQueue[m_EventBufferSize]{};
		unsigned int m_EventBufferStart{};
		unsigned int m_NrEventsQueued{};
		std::unordered_map<Event, std::vector<EventListener<void>*>> m_Listeners; 
	};

	template<class T>
	inline void that::EventQueue::AddListener(EventListener<T>* listener)
	{
		static_assert(std::is_base_of<that::Event, T>(), "T should derive from the base Event class");

		auto& listeners{ m_Listeners[T{}] };
		listeners.push_back(reinterpret_cast<EventListener<void>*>(listener));
	}

	template<class T>
	inline void that::EventQueue::SendEvent(const T& e)
	{
		static_assert(std::is_base_of<that::Event, T>(), "T should derive from the base Event class");
				
		if (m_NrEventsQueued >= m_EventBufferSize)
		{
			std::stringstream sstream{};
			sstream << "Too many events are being queued. Event " << e.GetName() << " was not added to the event queue.";
			Logger::LogWarning(sstream.str());
			return;
		}

		auto pEvent{ std::make_unique<T>(e) };

		const unsigned int curIdx{ (m_EventBufferStart + m_NrEventsQueued) % m_EventBufferSize };
		m_EventQueue[curIdx] = std::move(pEvent);
		++m_NrEventsQueued;
	}
}

