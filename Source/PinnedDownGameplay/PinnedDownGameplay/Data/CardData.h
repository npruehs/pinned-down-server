#pragma once

namespace PinnedDownGameplay
{
	namespace Data
	{
		struct CardData
		{
			int setIndex;
			int cardIndex;

			CardData() { }

			CardData(int setIndex, int cardIndex)
			{
				this->setIndex = setIndex;
				this->cardIndex = cardIndex;
			}
		};
	}
}