#include "Event.h"
#include "ThreatSystem.h"

#include "Events\ThreatChangedEvent.h"

using namespace PinnedDownNet::Events;
using namespace PinnedDownServer::Systems;


ThreatSystem::ThreatSystem()
	: threat(0)
{
}

void ThreatSystem::InitSystem(Game* game)
{
	GameSystem::InitSystem(game);

	this->game->eventManager->AddListener(this, AddThreatAction::AddThreatActionType);

	// Set initial threat value.
	this->SetThreat(0);
}

void ThreatSystem::OnEvent(Event & newEvent)
{
	if (newEvent.GetEventType() == AddThreatAction::AddThreatActionType)
	{
		auto addThreatAction = static_cast<AddThreatAction&>(newEvent);
		this->OnAddThreat(addThreatAction);
	}
}

void ThreatSystem::OnAddThreat(AddThreatAction& addThreatAction)
{
	this->SetThreat(this->threat + addThreatAction.threat);
}

void ThreatSystem::SetThreat(int newThreat)
{
	this->threat = newThreat;

	// Notify listeners.
	auto threatChangedEvent = std::make_shared<ThreatChangedEvent>(this->threat);
	this->game->eventManager->QueueEvent(threatChangedEvent);
}