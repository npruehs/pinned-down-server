#pragma once

#include <list>

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "..\Components\PlayerDeckComponent.h"

#include "Data\CardFactory.h"

#include "Events\PlayerAddedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class PlayerDeckSystem : public GameSystem, public IEventListener
		{
		public:
			PlayerDeckSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			const unsigned int handCardLimit = 2;

			std::list<Entity> players;

			std::shared_ptr<CardFactory> cardFactory;

			void OnEvent(Event & event);

			void OnPlayerAdded(PlayerAddedEvent& playerAddedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);

			void DrawToHandLimit(Entity playerEntity);
		};
	}
}