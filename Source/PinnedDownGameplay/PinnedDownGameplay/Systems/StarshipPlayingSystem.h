#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\TurnPhase.h"

#include "..\Events\StarshipPlayedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class StarshipPlayingSystem : public GameSystem, public IEventListener
		{
		public:
			StarshipPlayingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			TurnPhase currentTurnPhase;

			void OnEvent(Event & event);

			void OnStarshipPlayed(StarshipPlayedEvent& starshipPlayedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
		};
	}
}