

#include "MasterServer.h"
#include "ServerGame.h"

#include "Events\ClientConnectedEvent.h"

#include "Systems\AssignmentSystem.h"
#include "Systems\CardPlayingSystem.h"
#include "Systems\CardStateSystem.h"
#include "Systems\DamageSystem.h"
#include "Systems\DistanceSystem.h"
#include "Systems\DistanceVictorySystem.h"
#include "Systems\EffectPlayingSystem.h"
#include "Systems\EnemyAttackSystem.h"
#include "Systems\FightSystem.h"
#include "Systems\FlagshipSystem.h"
#include "Systems\FlagshipDefeatSystem.h"
#include "Systems\JumpEnemyRemovalSystem.h"
#include "Systems\PlayerSystem.h"
#include "Systems\PlayerDeckSystem.h"
#include "Systems\StarshipPlayingSystem.h"
#include "Systems\TemporaryEffectSystem.h"
#include "Systems\ThreatSystem.h"
#include "Systems\TurnPhaseSystem.h"


using namespace PinnedDownCore;
using namespace PinnedDownGameplay::Events;
using namespace PinnedDownGameplay::Systems;
using namespace PinnedDownServer;


ServerGame::ServerGame(MasterServer* masterServer, std::shared_ptr<HTTPClient> httpClient, std::shared_ptr<ServerLogger> logger, PinnedDownClientData* pinnedDownClient)
	: masterServer(masterServer),
	httpClient(httpClient),
	logger(logger),
	pinnedDownClient(pinnedDownClient),
	game(std::make_shared<Game>())
{
	// Setup game infrastructure.
	this->serverEventDispatcher = std::make_shared<ServerEventDispatcher>(this, this->game);

	// Setup analytics.
	this->analytics = std::make_shared<ServerAnalytics>(this->game, this->httpClient, this->logger, this->pinnedDownClient->clientGUID);

	// Init systems.
	this->game->systemManager->AddSystem(std::make_shared<CardStateSystem>());
	this->game->systemManager->AddSystem(std::make_shared<DamageSystem>());
	this->game->systemManager->AddSystem(std::make_shared<DistanceSystem>());
	this->game->systemManager->AddSystem(std::make_shared<DistanceVictorySystem>());
	this->game->systemManager->AddSystem(std::make_shared<EnemyAttackSystem>());
	this->game->systemManager->AddSystem(std::make_shared<FlagshipSystem>());
	this->game->systemManager->AddSystem(std::make_shared<FlagshipDefeatSystem>());
	this->game->systemManager->AddSystem(std::make_shared<TurnPhaseSystem>());
	this->game->systemManager->AddSystem(std::make_shared<JumpEnemyRemovalSystem>());
	this->game->systemManager->AddSystem(std::make_shared<PlayerSystem>());
	this->game->systemManager->AddSystem(std::make_shared<PlayerDeckSystem>());
	this->game->systemManager->AddSystem(std::make_shared<ThreatSystem>());
	this->game->systemManager->AddSystem(std::make_shared<AssignmentSystem>());
	this->game->systemManager->AddSystem(std::make_shared<FightSystem>());
	this->game->systemManager->AddSystem(std::make_shared<CardPlayingSystem>());
	this->game->systemManager->AddSystem(std::make_shared<StarshipPlayingSystem>());
	this->game->systemManager->AddSystem(std::make_shared<EffectPlayingSystem>());
	this->game->systemManager->AddSystem(std::make_shared<TemporaryEffectSystem>());
	
	this->game->systemManager->InitSystems();
	this->Update();

	// Add first player.
	auto clientConnectedEvent = std::make_shared<ClientConnectedEvent>(this->pinnedDownClient->clientId);
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
	this->masterServer->OnServerEvent(this->pinnedDownClient->clientId, serverEvent);
}

void ServerGame::Update()
{
	this->game->Update(1.0f);
}
