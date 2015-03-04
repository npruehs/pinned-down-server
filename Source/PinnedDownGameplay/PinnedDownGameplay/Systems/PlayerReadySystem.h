#pragma once

#include <map>

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Actions\PlayerReadyAction.h"

#include "Events\PlayerAddedEvent.h"
#include "Events\PlayerReadyStateResetEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class PlayerReadySystem : public GameSystem, public IEventListener
		{
		public:
			PlayerReadySystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::map<int, bool> playerReady;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(PlayerAddedEvent);
			EVENT_HANDLER_DECLARATION(PlayerReadyAction);
			EVENT_HANDLER_DECLARATION(PlayerReadyStateResetEvent);
			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);

			void ResetReadyState();
		};
	}
}