#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "EntityRemovedEvent.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
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

			void OnEntityRemoved(EntityRemovedEvent& entityRemovedEvent);
		};
	}
}