#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct FightStartedEvent : public Event
		{
			static const HashedString FightStartedEventType;

			const HashedString & GetEventType() const
			{
				return FightStartedEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity playerShip;
			Entity enemyShip;

			explicit FightStartedEvent(Entity playerShip, Entity enemyShip)
			{
				this->playerShip = playerShip;
				this->enemyShip = enemyShip;

			}
		};
	}
}