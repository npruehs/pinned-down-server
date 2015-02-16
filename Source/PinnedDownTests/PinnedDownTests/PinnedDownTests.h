#pragma once

#include "EventListenerMock.h"

class PinnedDownTestData
{
public:
	std::shared_ptr<Game> game;
	std::shared_ptr<EventListenerMock> mock;
};

template<class PinnedDownSystem> inline std::shared_ptr<PinnedDownTestData> CreateTestGame(HashedString const & eventType)
{	
	auto game = std::make_shared<Game>();
	game->systemManager->AddSystem(std::make_shared<PinnedDownSystem>());

	auto mock = std::make_shared<EventListenerMock>(game);
	mock->RegisterListener(eventType);

	game->systemManager->InitSystems();
	game->Update(1.0f);

	auto testData = std::make_shared<PinnedDownTestData>();
	testData->game = game;
	testData->mock = mock;

	return testData;
}