#include "Deck.h"

using namespace PinnedDownServer::Data;


Deck::Deck()
{
}

void Deck::Add(CardData card)
{
	this->cards.push_back(card);
}

CardData Deck::Draw()
{
	auto card = this->cards.front();
	this->cards.pop_front();
	return card;
}

bool Deck::IsEmpty()
{
	return this->cards.empty();
}