#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "..\Components\PlayerDeckComponent.h"
#include "Data\CardFactory.h"
#include "Events\PlayerAddedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;

namespace PinnedDownServer
{
	namespace Systems
	{
		class PlayerDeckSystem : public GameSystem, public IEventListener
		{
		public:
			PlayerDeckSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<CardFactory> cardFactory;

			void OnEvent(Event & event);

			void OnPlayerAdded(PlayerAddedEvent& playerAddedEvent);
		};
	}
}