#pragma once

#include <list>

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
			std::shared_ptr<std::list<Entity>> enemyShips;

			explicit FightStartedEvent(Entity playerShip, std::shared_ptr<std::list<Entity>> enemyShips)
			{
				this->playerShip = playerShip;
				this->enemyShips = enemyShips;

			}
		};
	}
}