#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct ShipDefeatedEvent : public Event
		{
			static const HashedString ShipDefeatedEventType;

			const HashedString & GetEventType() const
			{
				return ShipDefeatedEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity shipEntity;

			explicit ShipDefeatedEvent(Entity shipEntity)
			{
				this->shipEntity = shipEntity;
			}
		};
	}
}