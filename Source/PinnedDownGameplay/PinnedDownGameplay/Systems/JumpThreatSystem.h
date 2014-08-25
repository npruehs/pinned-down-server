#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\TurnPhase.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;

namespace PinnedDownServer
{
	namespace Systems
	{
		class JumpThreatSystem : public GameSystem, public IEventListener
		{
		public:
			JumpThreatSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			void OnEvent(Event & event);

			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
		};
	}
}