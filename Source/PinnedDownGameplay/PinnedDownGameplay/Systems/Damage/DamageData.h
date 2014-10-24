#pragma once

#include "EntityManager.h"

using namespace PinnedDownCore;

namespace PinnedDownServer
{
	namespace Systems
	{
		namespace Damage
		{
			struct DamageData
			{
				Entity damagedShip;
				Entity damage;

				DamageData(Entity damagedShip, Entity damage)
				{
					this->damagedShip = damagedShip;
					this->damage = damage;
				}
			};
		}
	}
}