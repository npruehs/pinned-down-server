#pragma once

#include "Game.h"
#include "GameSystem.h"
#include "IEventListener.h"

#include "Data\CardFactory.h"

#include "Events\TurnPhaseChangedEvent.h"

using namespace PinnedDownCore;
using namespace PinnedDownNet::Events;

namespace PinnedDownServer
{
	namespace Systems
	{
		class EnemyAttackSystem : public GameSystem, public IEventListener
		{
		public:
			EnemyAttackSystem();

			void InitSystem(PinnedDownCore::Game* game);

		private:
			std::shared_ptr<CardFactory> cardFactory;

			void OnEvent(Event & event);

			void OnTurnPhaseChanged(TurnPhaseChangedEvent& turnPhaseChangedEvent);

			void PlayEnemyCards();
		};
	}
}