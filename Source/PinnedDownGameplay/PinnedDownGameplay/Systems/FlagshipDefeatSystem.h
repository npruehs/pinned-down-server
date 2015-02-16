#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "EntityRemovedEvent.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class FlagshipDefeatSystem : public GameSystem, public IEventListener
		{
		public:
			FlagshipDefeatSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			void OnEvent(Event & event);

			EVENT_HANDLER_DECLARATION(EntityRemovedEvent);
		};
	}
}