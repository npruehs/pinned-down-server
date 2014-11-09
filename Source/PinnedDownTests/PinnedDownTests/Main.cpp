#include <memory>
#include <stdio.h>

#include "src/gtest-all.cc"
#include "src/gmock-all.cc"

#include "EventListenerMock.h"

#include "Events\CoveredDistanceChangedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

#include "Systems\DistanceVictorySystem.h"


using namespace PinnedDownGameplay;


TEST(DistanceVictorySystemTest, VictoryWhenFullDistanceCovered)
{
	// Arrange.
	auto game = std::make_shared<Game>();
	game->systemManager->AddSystem(std::make_shared<Systems::DistanceVictorySystem>());

	auto eventListenerMock = std::make_shared<EventListenerMock>(game);
	eventListenerMock->RegisterListener(CoveredDistanceChangedEvent::CoveredDistanceChangedEventType);

	game->systemManager->InitSystems();
	game->Update(1.0f);

	// Act.
	auto coveredDistanceChangedEvent = std::make_shared<CoveredDistanceChangedEvent>(5, 5);
	game->eventManager->QueueEvent(coveredDistanceChangedEvent);

	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Jump);
	game->eventManager->QueueEvent(turnPhaseChangedEvent);

	game->Update(1.0f);

	// Assert.
	EXPECT_EQ(true, eventListenerMock->EventHasOccurred());
}

GTEST_API_ int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
