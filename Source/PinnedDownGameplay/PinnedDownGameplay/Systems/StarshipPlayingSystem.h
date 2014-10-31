#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "..\Events\StarshipPlayedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;

namespace PinnedDownServer
{
	namespace Systems
	{
		class StarshipPlayingSystem : public GameSystem, public IEventListener
		{
		public:
			StarshipPlayingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			void OnEvent(Event & event);

			void OnStarshipPlayed(StarshipPlayedEvent& starshipPlayedEvent);
		};
	}
}