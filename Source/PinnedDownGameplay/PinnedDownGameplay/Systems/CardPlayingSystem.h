#pragma once

#include "BidirectionalMap.h"
#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Actions\PlayCardAction.h"

#include "Events\PlayerAddedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class CardPlayingSystem : public GameSystem, public IEventListener
		{
		public:
			CardPlayingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<BidirectionalMap<int, Entity>> clientToPlayerEntityIdMap;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(PlayCardAction);
			EVENT_HANDLER_DECLARATION(PlayerAddedEvent);
		};
	}
}