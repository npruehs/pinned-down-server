#pragma once

#include "EntityManager.h"
#include "Event.h"

using namespace PinnedDownCore;

namespace PinnedDownGameplay
{
	namespace Events
	{
		struct PlayEffectAction : public Event
		{
			static const HashedString PlayEffectActionType;

			const HashedString & GetEventType() const
			{
				return PlayEffectActionType;
			}

			const NetRole GetNetRole() const
			{
				return NetRole::Server;
			}

			Entity effectEntity;
			Entity targetEntity;

			explicit PlayEffectAction(Entity effectEntity, Entity targetEntity)
			{
				this->effectEntity = effectEntity;
				this->targetEntity = targetEntity;
			}
		};
	}
}