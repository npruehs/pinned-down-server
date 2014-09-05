#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "..\Data\CardData.h"
#include "Data\CardFactory.h"
#include "..\Data\Deck.h"

#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Data;

namespace PinnedDownServer
{
	namespace Systems
	{
		class EnemyAttackSystem : public GameSystem, public IEventListener
		{
		public:
			EnemyAttackSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<CardFactory> cardFactory;
			std::shared_ptr<Deck> attackDeck;

			void OnEvent(Event & event);

			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);

			void PrepareAttackDeck();
			void PlayEnemyCards();
		};
	}
}