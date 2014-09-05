#include "Deck.h"

using namespace PinnedDownServer::Data;
using namespace PinnedDownServer::Util;

Deck::Deck() :
	current(0)
{
}

void Deck::Add(CardData card)
{
	this->cards.push_back(card);
}

CardData Deck::Draw()
{
	auto card = this->cards[this->current++];
	return card;
}

bool Deck::IsEmpty()
{
	return this->current >= this->cards.size();
}

void Deck::Shuffle(std::shared_ptr<Random> random)
{
	auto maxExclusive = this->cards.size();

	while (maxExclusive > 1)
	{
		auto randomIndex = random->NextUnsignedInt(maxExclusive);

		auto temp = this->cards[maxExclusive - 1];
		this->cards[maxExclusive - 1] = this->cards[randomIndex];
		this->cards[randomIndex] = temp;

		maxExclusive--;
	}
}