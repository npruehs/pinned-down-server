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

			void OnEvent(Event & event);

			void OnEnemyCardPlayed(EnemyCardPlayedEvent& enemyCardPlayedEvent);
			void OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent);
			void OnFlagshipPlayed(FlagshipPlayedEvent& flagshipPlayedEvent);
			void OnStarshipPlayed(StarshipPlayedEvent& starshipPlayedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);

			void SetThreat(int newThreat);
		};
	}
}