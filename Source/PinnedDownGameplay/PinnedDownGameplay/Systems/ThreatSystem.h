#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\TurnPhase.h"

#include "Events\TurnPhaseChangedEvent.h"

#include "..\Events\EnemyCardPlayedEvent.h"
#include "EntityRemovedEvent.h"
#include "..\Events\FlagshipPlayedEvent.h"
#include "..\Events\StarshipPlayedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Events;


namespace PinnedDownGameplay
{
	namespace Systems
	{
		class ThreatSystem : public GameSystem, public IEventListener
		{
		public:
			ThreatSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			TurnPhase currentTurnPhase;
			int threat;
			int locationThreat;
			int playerShips;

			const int initialThreat = 4;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(EnemyCardPlayedEvent);
			EVENT_HANDLER_DECLARATION(EntityRemovedEvent);
			EVENT_HANDLER_DECLARATION(FlagshipPlayedEvent);
			EVENT_HANDLER_DECLARATION(StarshipPlayedEvent);
			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);

			void SetThreat(int newThreat);
		};
	}
}