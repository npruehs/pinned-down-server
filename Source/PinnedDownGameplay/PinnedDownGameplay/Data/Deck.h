#pragma once

#include <list>

#include "CardData.h"

namespace PinnedDownServer
{
	namespace Data
	{
		class Deck
		{
		public:
			Deck();

			void Add(CardData data);
			CardData Draw();

		private:
			std::list<CardData> cards;
		};
	}
}