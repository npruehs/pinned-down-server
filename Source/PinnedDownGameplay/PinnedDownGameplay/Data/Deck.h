#pragma once

#include <memory>
#include <vector>

#include "CardData.h"

#include "..\Util\Random.h"

using namespace PinnedDownGameplay::Util;

namespace PinnedDownGameplay
{
	namespace Data
	{
		class Deck
		{
		public:
			Deck();

			void Add(CardData data);
			CardData Draw();
			bool IsEmpty();
			void Shuffle(std::shared_ptr<Random> random);

		private:
			int current;
			std::vector<CardData> cards;
		};
	}
}