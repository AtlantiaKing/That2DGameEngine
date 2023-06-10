#pragma once

#include "Component.h"

namespace that
{
	class TextureRenderer;
}

namespace digdug
{
	class HighScoreLetter final : public that::Component
	{
	public:
		HighScoreLetter() = default;
		virtual ~HighScoreLetter() = default;

		HighScoreLetter(const HighScoreLetter& other) = delete;
		HighScoreLetter(HighScoreLetter&& other) = delete;
		HighScoreLetter& operator=(const HighScoreLetter& other) = delete;
		HighScoreLetter& operator=(HighScoreLetter&& other) = delete;

		virtual void Init() override;
		virtual void Update() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;

		void SetIndex(int index);
		void MarkerUp();
		void MarkerDown();

		char GetLetter() const;
	private:
		float m_BlinkTime{};
		const float m_TimeBetweenBlinks{ 0.1f };

		char m_Letter{ 'A' };
		int m_Index{};

		that::TextureRenderer* m_pRenderer{};
	};
}
