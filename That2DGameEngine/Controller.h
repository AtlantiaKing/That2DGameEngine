#pragma once

#include <memory>

namespace that
{
	class Controller
	{
		class ControllerImplementation;
		std::unique_ptr<ControllerImplementation> m_pImpl{};
	public:
		Controller(unsigned int controllerIdx);
		~Controller();

		void Update();
		void SetDeadZone(float percentage);

		bool OnButtonDown(unsigned int button) const;
		bool OnButtonUp(unsigned int button) const;
		bool OnButton(unsigned int button) const;
		float GetAxis(unsigned int thumbStick, bool x) const;
		float GetAxis(unsigned int button) const;
	private:
	};
}

