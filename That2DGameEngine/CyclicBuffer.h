#pragma once

#include "Logger.h"

#include <sstream>

namespace that
{
	template <typename T, int size>
	class CyclicBuffer
	{
	public:
		void Insert(T&& item);
		void Insert(const T& item);
		T Pop();
		unsigned int GetNrAssigned() const { return m_NrItems; }
	private:
		T m_Buffer[size]{};

		unsigned int m_CurItemIdx{};
		unsigned int m_NrItems{};
	};
	template<typename T, int size>
	inline void CyclicBuffer<T, size>::Insert(T&& item)
	{
		if (m_NrItems >= size)
		{
			Logger::LogWarning("Too many items are being queued.");
			return;
		}

		const unsigned int curIdx{ (m_CurItemIdx + m_NrItems) % size };
		m_Buffer[curIdx] = std::move(item);
		++m_NrItems;
	}

	template<typename T, int size>
	inline void CyclicBuffer<T, size>::Insert(const T& item)
	{
		if (m_NrItems >= size)
		{
			Logger::LogError("Too many items are being queued.");
			return;
		}

		const unsigned int curIdx{ (m_CurItemIdx + m_NrItems) % size };
		m_Buffer[curIdx] = item;
		++m_NrItems;
	}

	template<typename T, int size>
	inline T CyclicBuffer<T, size>::Pop()
	{
		unsigned int curIdx{ m_CurItemIdx };

		++m_CurItemIdx %= size;
		--m_NrItems;

		return std::move(m_Buffer[curIdx]);
	}
}
