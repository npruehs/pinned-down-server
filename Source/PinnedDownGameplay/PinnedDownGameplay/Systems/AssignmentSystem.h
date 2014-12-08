#pragma once

#include <list>
#include <map>

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Actions\AssignCardAction.h"
#include "Actions\EndTurnAction.h"
#include "Actions\ResolveFightAction.h"

#include "Data\TurnPhase.h"

#include "..\Events\EnemyCardPlayedEvent.h"
#include "EntityRemovedEvent.h"
#include "Events\FightResolvedEvent.h"
#include "..\Events\FlagshipPlayedEvent.h"
#include "..\Events\StarshipPlayedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class AssignmentSystem : public GameSystem, public IEventListener
		{
		public:
			AssignmentSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::map<Entity, std::shared_ptr<std::list<Entity>>> currentAssignments;
			std::list<Entity> playerCards;
			std::list<Entity> enemyCards;
			TurnPhase currentTurnPhase;

			void OnEvent(Event & event);

			void OnAssignCard(AssignCardAction& assignCardAction);
			void OnEndTurn(EndTurnAction& endTurnAction);
			void OnEnemyCardPlayed(EnemyCardPlayedEvent& enemyCardPlayedEvent);
			void OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent);
			void OnFlagshipPlayed(FlagshipPlayedEvent& flagshipPlayedEvent);
			void OnFightResolved(FightResolvedEvent& fightResolvedEvent);
			void OnResolveFight(ResolveFightAction& resolveFightAction);
			void OnStarshipPlayed(StarshipPlayedEvent& starshipPlayedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
		};
	}
}