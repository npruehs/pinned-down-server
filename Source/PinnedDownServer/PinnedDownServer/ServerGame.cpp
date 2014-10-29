

#include "MasterServer.h"
#include "ServerGame.h"

#include "Events\ClientConnectedEvent.h"

#include "Systems\AssignmentSystem.h"
#include "Systems\CardPlayingSystem.h"
#include "Systems\CardStateSystem.h"
#include "Systems\DamageSystem.h"
#include "Systems\DistanceSystem.h"
#include "Systems\DistanceVictorySystem.h"
#include "Systems\EnemyAttackSystem.h"
#include "Systems\FightSystem.h"
#include "Systems\FlagshipSystem.h"
#include "Systems\FlagshipDefeatSystem.h"
#include "Systems\JumpEnemyRemovalSystem.h"
#include "Systems\JumpThreatSystem.h"
#include "Systems\PlayerSystem.h"
#include "Systems\PlayerDeckSystem.h"
#include "Systems\ThreatSystem.h"
#include "Systems\TurnPhaseSystem.h"


using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;
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
	this->game->systemManager->AddSystem(std::make_shared<Systems::DamageSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::DistanceSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::DistanceVictorySystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::EnemyAttackSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::FlagshipSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::FlagshipDefeatSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::TurnPhaseSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::JumpEnemyRemovalSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::JumpThreatSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::PlayerSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::PlayerDeckSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::ThreatSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::AssignmentSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::FightSystem>());
	this->game->systemManager->AddSystem(std::make_shared<Systems::CardPlayingSystem>());
	
	this->game->systemManager->InitSystems();
	this->Update();

	// Add first player.
	auto clientConnectedEvent = std::make_shared<ClientConnectedEvent>(this->clientId);
	this->game->eventManager->QueueEvent(clientConnectedEvent);
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
