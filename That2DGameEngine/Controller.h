#pragma once

namespace that
{
	class Controller
	{
		class ControllerImplementation;
		ControllerImplementation* m_pImpl{};
	public:
		Controller(unsigned int controllerIdx);
		~Controller();

		void Update();
		void SetDeadZone(float percentage);

		bool OnButtonDown(unsigned int button) const;
		bool OnButtonUp(unsigned int button) const;
		bool OnButton(unsigned int button) const;
		float GetAxis(bool left, bool x) const;
	private:
	};
}

