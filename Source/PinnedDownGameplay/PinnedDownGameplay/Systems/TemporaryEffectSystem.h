#pragma once

#include <list>

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\TurnPhase.h"

#include "..\Events\BonusPowerChangedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownNet::Data;
using namespace PinnedDownNet::Events;

namespace PinnedDownGameplay
{
	namespace Systems
	{
		class TemporaryEffectSystem : public GameSystem, public IEventListener
		{
		public:
			TemporaryEffectSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::list<Entity> buffedEntities;

			void OnEvent(Event & event);

			void OnBonusPowerChanged(BonusPowerChangedEvent& bonusPowerChangedEvent);
			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);
		};
	}
}