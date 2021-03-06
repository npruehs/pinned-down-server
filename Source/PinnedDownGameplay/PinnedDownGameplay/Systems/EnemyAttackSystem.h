#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "..\Data\CardData.h"
#include "Data\CardFactory.h"
#include "..\Data\Deck.h"

#include "Events\ThreatChangedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownGameplay::Data;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class EnemyAttackSystem : public GameSystem, public IEventListener
		{
		public:
			EnemyAttackSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			int currentThreat;
			std::shared_ptr<CardFactory> cardFactory;
			std::shared_ptr<Deck> attackDeck;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(ThreatChangedEvent);
			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);
			
			void PrepareAttackDeck();
			void PlayEnemyCards();
		};
	}
}