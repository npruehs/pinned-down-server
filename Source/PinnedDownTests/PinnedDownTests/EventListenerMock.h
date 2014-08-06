#pragma once

#include "Game.h"
#include "IEventListener.h"
#include "Event.h"

using namespace PinnedDownCore;

class EventListenerMock : public IEventListener
{
public:
	EventListenerMock(std::shared_ptr<PinnedDownCore::Game> game)
	{
		this->game = game;
		this->eventOccurred = false;
	}

	void RegisterListener(HashedString const & eventType)
	{
		this->game->eventManager->AddListener(this, eventType);
	}

	bool EventHasOccurred()
	{
		return this->eventOccurred;
	}

private:
	std::shared_ptr<PinnedDownCore::Game> game;
	bool eventOccurred;

	void OnEvent(Event & event)
	{
		this->eventOccurred = true;
	}
};