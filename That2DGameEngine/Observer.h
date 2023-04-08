#pragma once

namespace that
{
	template <typename T>
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const T& data) = 0;
	};
}

