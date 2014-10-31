#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Actions\PlayCardAction.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;

namespace PinnedDownServer
{
	namespace Systems
	{
		class CardPlayingSystem : public GameSystem, public IEventListener
		{
		public:
			CardPlayingSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			void OnEvent(Event & event);

			void OnPlayCard(PlayCardAction& playCardAction);
		};
	}
}