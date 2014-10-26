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

#include "Events\CardCreatedEvent.h"
#include "Events\CardRemovedEvent.h"
#include "Events\FightResolvedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;

namespace PinnedDownServer
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
			void OnCardCreated(CardCreatedEvent& cardCreatedEvent);
			void OnCardRemoved(CardRemovedEvent& cardRemovedEvent);
			void OnEndTurn(EndTurnAction& endTurnAction);
			void OnFightResolved(FightResolvedEvent& fightResolvedEvent);
			void OnResolveFight(ResolveFightAction& resolveFightAction);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
		};
	}
}