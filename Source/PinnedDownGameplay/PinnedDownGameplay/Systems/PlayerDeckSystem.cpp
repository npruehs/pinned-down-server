#include "Event.h"
#include "PlayerDeckSystem.h"

#include "Components\CardStateComponent.h"

#include "Events\CardStateChangedEvent.h"

using namespace PinnedDownGameplay::Components;
using namespace PinnedDownGameplay::Systems;
using namespace PinnedDownNet::Components;
using namespace PinnedDownNet::Events;


PlayerDeckSystem::PlayerDeckSystem()
{
}

void PlayerDeckSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	// Setup card factory.
	this->cardFactory = std::make_shared<CardFactory>(this->game);

	// Register listeners.
	this->game->eventManager->AddListener(this, PlayerAddedEvent::PlayerAddedEventType);
	this->game->eventManager->AddListener(this, TurnPhaseChangedEvent::TurnPhaseChangedEventType);
}

void PlayerDeckSystem::OnEvent(Event & newEvent)
{
	CALL_EVENT_HANDLER(PlayerAddedEvent);
	CALL_EVENT_HANDLER(TurnPhaseChangedEvent);
}

EVENT_HANDLER_DEFINITION(PlayerDeckSystem, PlayerAddedEvent)
{
	// Setup player deck.
	auto playerEntity = data.serverEntity;
	this->players.push_back(playerEntity);

	auto playerDeckComponent = std::make_shared<PlayerDeckComponent>();
	playerDeckComponent->drawDeck = std::make_shared<Deck>();
	this->game->entityManager->AddComponent(playerEntity, playerDeckComponent);
	
	// Add cards.
	playerDeckComponent->drawDeck->Add(CardData(0, 45));
	playerDeckComponent->drawDeck->Add(CardData(0, 45));
	playerDeckComponent->drawDeck->Add(CardData(0, 45));
	playerDeckComponent->drawDeck->Add(CardData(0, 45));
	playerDeckComponent->drawDeck->Add(CardData(0, 48));
	playerDeckComponent->drawDeck->Add(CardData(0, 48));
	playerDeckComponent->drawDeck->Add(CardData(0, 48));
	playerDeckComponent->drawDeck->Add(CardData(0, 48));
	playerDeckComponent->drawDeck->Add(CardData(0, 54));
	playerDeckComponent->drawDeck->Add(CardData(0, 55));
	playerDeckComponent->drawDeck->Add(CardData(0, 57));
	playerDeckComponent->drawDeck->Add(CardData(0, 58));

	// Shuffle deck.
	auto random = std::make_shared<Random>();
	playerDeckComponent->drawDeck->Shuffle(random);

	// Draw initial cards.
	this->DrawToHandLimit(playerEntity);
}

EVENT_HANDLER_DEFINITION(PlayerDeckSystem, TurnPhaseChangedEvent)
{
	if (data.newTurnPhase != TurnPhase::Jump)
	{
		return;
	}

	// Draw cards.
	for (auto it = this->players.begin(); it != this->players.end(); ++it)
	{
		auto playerEntity = *it;
		this->DrawToHandLimit(playerEntity);
	}
}

void PlayerDeckSystem::DrawToHandLimit(Entity playerEntity)
{
	auto playerDeckComponent = this->game->entityManager->GetComponent<PlayerDeckComponent>(playerEntity, PlayerDeckComponent::PlayerDeckComponentType);

	while (!playerDeckComponent->drawDeck->IsEmpty() && playerDeckComponent->hand.size() < this->handCardLimit)
	{
		auto topCard = playerDeckComponent->drawDeck->Draw();
		auto handCard = this->cardFactory->CreateCard(playerEntity, topCard.setIndex, topCard.cardIndex, CardState::Hand);
		playerDeckComponent->hand.push_back(handCard);
	}
}