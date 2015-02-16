#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct EnemyCardPlayedEvent : public Event
		{
			static const HashedString EnemyCardPlayedEventType;

			const HashedString & GetEventType() const
			{
				return EnemyCardPlayedEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity cardEntity;

			explicit EnemyCardPlayedEvent() : EnemyCardPlayedEvent(INVALID_ENTITY_ID)
			{
			}

			explicit EnemyCardPlayedEvent(Entity cardEntity)
			{
				this->cardEntity = cardEntity;
			}
		};
	}
}