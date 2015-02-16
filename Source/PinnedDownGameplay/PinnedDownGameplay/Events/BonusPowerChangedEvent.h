#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct BonusPowerChangedEvent : public Event
		{
			static const HashedString BonusPowerChangedEventType;

			const HashedString & GetEventType() const
			{
				return BonusPowerChangedEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity entity;

			explicit BonusPowerChangedEvent(Entity entity)
			{
				this->entity = entity;
			}
		};
	}
}