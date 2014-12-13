#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "EntityInitializedEvent.h"
#include "EntityRemovedEvent.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class CardStateSystem : public GameSystem, public IEventListener
		{
		public:
			CardStateSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(EntityInitializedEvent);
			EVENT_HANDLER_DECLARATION(EntityRemovedEvent);
		};
	}
}