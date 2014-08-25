#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Actions\AddThreatAction.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;

namespace PinnedDownServer
{
	namespace Systems
	{
		class ThreatSystem : public GameSystem, public IEventListener
		{
		public:
			ThreatSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			int threat;

			void OnEvent(Event & event);

			void OnAddThreat(AddThreatAction& addThreatAction);

			void SetThreat(int newThreat);
		};
	}
}