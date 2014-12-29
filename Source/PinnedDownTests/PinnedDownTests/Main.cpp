#include <memory>

#include "src/gtest-all.cc"
#include "src/gmock-all.cc"

#include "PinnedDownTests.h"

#include "Components\FlagshipComponent.h"

#include "Events\CoveredDistanceChangedEvent.h"
#include "Events\TurnPhaseChangedEvent.h"
#include "Events\DefeatEvent.h"
#include "Events\VictoryEvent.h"

#include "Systems\DistanceVictorySystem.h"
#include "Systems\FlagshipDefeatSystem.h"


using namespace PinnedDownGameplay::Systems;
using namespace PinnedDownNet::Components;


TEST(VictoryDefeatTests, DefeatWhenFlagshipDestroyed)
{
	// Arrange.
	auto testData = CreateTestGame<FlagshipDefeatSystem>(DefeatEvent::DefeatEventType);

	auto entity = testData->game->entityManager->CreateEntity();
	auto flagshipComponent = std::make_shared<FlagshipComponent>();
	testData->game->entityManager->AddComponent(entity, flagshipComponent);
	testData->game->Update(1.0f);

	// Act.
	testData->game->entityManager->RemoveEntity(entity);
	testData->game->Update(1.0f);

	// Assert.
	EXPECT_EQ(true, testData->mock->EventHasOccurred());
}

TEST(VictoryDefeatTests, VictoryWhenFullDistanceCovered)
{
	// Arrange.
	auto testData = CreateTestGame<DistanceVictorySystem>(VictoryEvent::VictoryEventType);

	// Act.
	auto coveredDistanceChangedEvent = std::make_shared<CoveredDistanceChangedEvent>(5, 5);
	testData->game->eventManager->QueueEvent(coveredDistanceChangedEvent);

	testData->game->Update(1.0f);

	// Assert.
	EXPECT_EQ(true, testData->mock->EventHasOccurred());
}


GTEST_API_ int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
