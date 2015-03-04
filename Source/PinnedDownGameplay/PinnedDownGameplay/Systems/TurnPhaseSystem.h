#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\TurnPhase.h"

#include "..\Events\AllPlayersReadyEvent.h"
#include "Events\GameStartedEvent.h"
#include "..\Events\TurnPhaseEndedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class TurnPhaseSystem : public GameSystem, public IEventListener
		{
		public:
			TurnPhaseSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			TurnPhase currentPhase;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(AllPlayersReadyEvent);
			EVENT_HANDLER_DECLARATION(GameStartedEvent);
			EVENT_HANDLER_DECLARATION(TurnPhaseEndedEvent);

			void SetTurnPhase(TurnPhase turnPhase);
		};
	}
}