#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct EffectPlayedEvent : public Event
		{
			static const HashedString EffectPlayedEventType;

			const HashedString & GetEventType() const
			{
				return EffectPlayedEventType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity effectEntity;
			Entity targetEntity;

			explicit EffectPlayedEvent(Entity effectEntity, Entity targetEntity)
			{
				this->effectEntity = effectEntity;
				this->targetEntity = targetEntity;
			}
		};
	}
}