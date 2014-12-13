#pragma once

#include <list>

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\TurnPhase.h"

#include "EntityInitializedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class JumpEnemyRemovalSystem : public GameSystem, public IEventListener
		{
		public:
			JumpEnemyRemovalSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::list<Entity> enemyShips;

			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(EntityInitializedEvent);
			EVENT_HANDLER_DECLARATION(TurnPhaseChangedEvent);
		};
	}
}