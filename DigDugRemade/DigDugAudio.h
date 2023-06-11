#pragma once

#include "Component.h"

namespace digdug
{
	class DigDugAudio final : public that::Component
	{
	public:
		DigDugAudio() = default;
		~DigDugAudio() = default;

		DigDugAudio(const DigDugAudio& other) = delete;
		DigDugAudio(DigDugAudio&& other) = delete;
		DigDugAudio& operator=(const DigDugAudio& other) = delete;
		DigDugAudio& operator=(DigDugAudio&& other) = delete;

		void SetWalking(int playerIdx, bool isWalking);

		virtual void Update() override;

	private:
		static const int m_NrPlayers{ 2 };
		bool m_ArePlayersWalking[m_NrPlayers]{};

		bool m_IsPlayingSound{};
	};
}
