#pragma once

namespace that
{
	template <class T>
	class EventListener
	{
	public:
		virtual ~EventListener() = default;
		virtual void OnEvent(T* data) = 0;
	};
}
