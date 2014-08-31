#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\PlayerFactory.h"

#include "..\Events\ClientConnectedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownNet::Data;

namespace PinnedDownServer
{
	namespace Systems
	{
		class PlayerSystem : public GameSystem, public IEventListener
		{
		public:
			PlayerSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			int players;
			std::shared_ptr<PlayerFactory> playerFactory;

			void OnEvent(Event & event);

			void OnClientConnected(ClientConnectedEvent& clientConnectedEvent);
		};
	}
}