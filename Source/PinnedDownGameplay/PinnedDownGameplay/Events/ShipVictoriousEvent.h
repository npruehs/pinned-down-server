#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct ShipVictoriousEvent : public Event
		{
			static const HashedString ShipVictoriousEventType;

			const HashedString & GetEventType() const
			{
				return ShipVictoriousEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity shipEntity;

			explicit ShipVictoriousEvent(Entity shipEntity)
			{
				this->shipEntity = shipEntity;
			}
		};
	}
}