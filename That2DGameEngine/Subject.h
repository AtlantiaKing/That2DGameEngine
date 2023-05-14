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
		~Subject();

		void AddListener(Observer<T>* observer)
		{
			m_Observers.push_back(observer);
		}
		void RemoveListener(Observer<T>* observer)
		{
			const auto& it{ std::find(begin(m_Observers), end(m_Observers), observer) };
			if(it != end(m_Observers)) m_Observers.erase(it);
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

	template<class T>
	inline Subject<T>::~Subject()
	{
		for (auto& observer : m_Observers)
		{
			observer->OnSubjectDestroy();
		}
	}
}

