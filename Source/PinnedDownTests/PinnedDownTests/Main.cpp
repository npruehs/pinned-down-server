#include <memory>
#include <stdio.h>

#include "gtest/gtest.h"

#include "src/gtest.cc"
#include "src/gtest-death-test.cc"
#include "src/gtest-filepath.cc"
#include "src/gtest-port.cc"
#include "src/gtest-printers.cc"
#include "src/gtest-test-part.cc"
#include "src/gtest-typed-test.cc"

#include "EventListenerMock.h"

#include "Events\CoveredDistanceChangedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"

#include "Systems\DistanceVictorySystem.h"


using namespace PinnedDownServer;


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

	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::WrapUp);
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
