#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Actions\EndTurnAction.h"
#include "Data\TurnPhase.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
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

			void OnEndTurn(EndTurnAction& endTurnAction);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);

			void SetTurnPhase(TurnPhase turnPhase);
		};
	}
}