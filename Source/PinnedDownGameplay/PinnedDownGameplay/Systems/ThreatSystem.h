#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\TurnPhase.h"

#include "Events\TurnPhaseChangedEvent.h"

#include "..\Events\EnemyCardPlayedEvent.h"
#include "..\Events\StarshipPlayedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownServer::Events;


namespace PinnedDownServer
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
			
			void OnEvent(Event & event);

			void OnEnemyCardPlayed(EnemyCardPlayedEvent& enemyCardPlayedEvent);
			void OnStarshipPlayed(StarshipPlayedEvent& starshipPlayedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);

			void SetThreat(int newThreat);
		};
	}
}