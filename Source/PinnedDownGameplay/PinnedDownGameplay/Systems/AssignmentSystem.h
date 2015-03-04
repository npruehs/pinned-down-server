#pragma once

#include <list>
#include <map>

#include "BidirectionalMap.h"
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
#include "Events\PlayerAddedEvent.h"
#include "Events\StarshipPlayedEvent.h"
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
			std::shared_ptr<BidirectionalMap<int, Entity>> clientToPlayerEntityIdMap;
			std::map<Entity, std::shared_ptr<std::list<Entity>>> currentAssignments;
			std::list<Entity> playerCards;
			std::list<Entity> enemyCards;
			TurnPhase currentTurnPhase;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(AssignCardAction);
			EVENT_HANDLER_DECLARATION(EndTurnAction);
			EVENT_HANDLER_DECLARATION(EnemyCardPlayedEvent);
			EVENT_HANDLER_DECLARATION(EntityRemovedEvent);
			EVENT_HANDLER_DECLARATION(FlagshipPlayedEvent);
			EVENT_HANDLER_DECLARATION(FightResolvedEvent);
			EVENT_HANDLER_DECLARATION(PlayerAddedEvent);
			EVENT_HANDLER_DECLARATION(ResolveFightAction);
			EVENT_HANDLER_DECLARATION(StarshipPlayedEvent);
			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);
		};
	}
}