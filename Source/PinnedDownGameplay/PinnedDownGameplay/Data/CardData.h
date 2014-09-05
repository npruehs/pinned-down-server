#pragma once

namespace PinnedDownServer
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