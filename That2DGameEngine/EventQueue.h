#pragma once

#include "Singleton.h"
#include "Event.h"
#include "Events.h"

#include <unordered_map>
#include <functional>

namespace that
{
	class EventQueue final : public Singleton<EventQueue>
	{
	public:
		EventQueue() = default;
		~EventQueue() = default;
		EventQueue(const EventQueue&) = delete;
		EventQueue(EventQueue&&) = delete;
		EventQueue& operator= (const EventQueue&) = delete;
		EventQueue& operator= (const EventQueue&&) = delete;

		void SendMessage(const Event& e);
		template <class T>
		void AddListener(const std::function<void()>& listener);
		void AddListener(const Event& e, const std::function<void()>& listener);

		void NotifyListeners();
	private:
		bool PollEvent(Event& e);

		const static unsigned int m_EventBufferSize{ 10 };
		Event m_EventQueue[m_EventBufferSize]{};
		unsigned int m_EventBufferStart{};
		unsigned int m_NrEventsQueued{};
		std::unordered_map<Event, std::vector<std::function<void()>>> m_Listeners; 
	};

	template<class T>
	inline void that::EventQueue::AddListener(const std::function<void()>& listener)
	{
		AddListener(T{}, listener);
	}
}

