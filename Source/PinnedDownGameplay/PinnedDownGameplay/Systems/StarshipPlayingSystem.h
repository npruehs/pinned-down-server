#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\TurnPhase.h"

#include "..\Actions\PlayStarshipAction.h"
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

			EVENT_HANDLER_DECLARATION(PlayStarshipAction);
			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);
		};
	}
}