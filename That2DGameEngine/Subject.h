#pragma once

#include "Observer.h"

#include <vector>
#include <functional>

namespace that
{
	template <class T>
	class Subject final
	{
	public:
		Subject() = default;
		~Subject() = default;

		void AddListener(Observer<T>* observer)
		{
			m_Observers.push_back(observer);
		}

		void Notify(const T& data)
		{
			for (const auto& observer : m_Observers)
			{
				observer->Notify(data);
			}
		}
	private:
		std::vector<Observer<T>*> m_Observers{};
	};
}

