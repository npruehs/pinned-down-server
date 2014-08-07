#include "Event.h"
#include "FlagshipSystem.h"

using namespace PinnedDownServer::Systems;


FlagshipSystem::FlagshipSystem()
{
}

void FlagshipSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	// Setup card factory.
	this->cardFactory = std::make_shared<CardFactory>(this->game);

	// Play flagships.
	this->PlayFlagships();
}

void FlagshipSystem::OnEvent(Event & newEvent)
{
}

void FlagshipSystem::PlayFlagships()
{
	this->cardFactory->CreateCard(0, 56);
}