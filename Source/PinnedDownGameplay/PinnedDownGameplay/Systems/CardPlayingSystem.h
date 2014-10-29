#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Actions\PlayCardAction.h"

#include "Data\TurnPhase.h"

#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;

namespace PinnedDownServer
{
	namespace Systems
	{
		class CardPlayingSystem : public GameSystem, public IEventListener
		{
		public:
			CardPlayingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			TurnPhase currentTurnPhase;

			void OnEvent(Event & event);

			void OnPlayCard(PlayCardAction& playCardAction);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
		};
	}
}