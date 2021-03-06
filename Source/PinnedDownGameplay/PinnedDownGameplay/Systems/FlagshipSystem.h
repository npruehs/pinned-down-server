#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\CardFactory.h"
#include "Events\PlayerAddedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class FlagshipSystem : public GameSystem, public IEventListener
		{
		public:
			FlagshipSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<CardFactory> cardFactory;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(PlayerAddedEvent);
		};
	}
}