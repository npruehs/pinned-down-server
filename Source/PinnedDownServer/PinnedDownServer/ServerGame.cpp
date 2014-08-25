

#include "MasterServer.h"
#include "ServerGame.h"

#include "Systems\CardStateSystem.h"
#include "Systems\DistanceSystem.h"
#include "Systems\DistanceVictorySystem.h"
#include "Systems\FlagshipSystem.h"
#include "Systems\FlagshipDefeatSystem.h"
#include "Systems\JumpThreatSystem.h"
#include "Systems\TurnPhaseSystem.h"


using namespace PinnedDownCore;
using namespace PinnedDownServer;


ServerGame::ServerGame(MasterServer* masterServer, int clientId)
	: game(std::make_shared<Game>())
{
	this->masterServer = masterServer;
	this->clientId = clientId;

	// Setup game infrastructure.
	this->serverEventDispatcher = std::make_shared<ServerEventDispatcher>(this, this->game);

	// Init systems.
	this->game->systemManager->AddSystem(std::make_shared<Systems::CardStateSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::DistanceSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::DistanceVictorySystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::FlagshipSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::FlagshipDefeatSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::TurnPhaseSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::JumpThreatSystem>());

	this->game->systemManager->InitSystems();
	this->Update();
}

void ServerGame::OnClientAction(std::shared_ptr<Event> clientAction)
{
	this->game->eventManager->QueueEvent(clientAction);
	this->Update();
}

void ServerGame::OnServerEvent(Event& serverEvent)
{
	this->masterServer->OnServerEvent(this->clientId, serverEvent);
}

void ServerGame::Update()
{
	this->game->Update(1.0f);
}
