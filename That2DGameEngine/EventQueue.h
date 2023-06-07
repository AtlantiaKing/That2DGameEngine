#pragma once

#include "Singleton.h"
#include "Logger.h"
#include "CyclicBuffer.h"

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
		template <class T>
		void RemoveListener(EventListener<T>* listener);

		void NotifyListeners();
	private:
		bool PollEvent(std::unique_ptr<Event>& e);

		CyclicBuffer<std::unique_ptr<Event>, 10> m_Queue{};

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

		auto pEvent{ std::make_unique<T>(e) };

		m_Queue.Insert(std::move(pEvent));
	}

	template<class T>
	inline void EventQueue::RemoveListener(EventListener<T>* listener)
	{
		static_assert(std::is_base_of<that::Event, T>(), "T should derive from the base Event class");

		auto& listeners{ m_Listeners[T{}] };
		listeners.erase(std::find(begin(listeners), end(listeners), reinterpret_cast<EventListener<void>*>(listener)));
	}
}

