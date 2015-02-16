#include <memory>

#include "src/gtest-all.cc"
#include "src/gmock-all.cc"

#include "PinnedDownTests.h"

#include "Actions\AssignCardAction.h"

#include "Components\CardComponent.h"
#include "Components\FlagshipComponent.h"
#include "Components\OwnerComponent.h"

#include "Events\CardAssignedEvent.h"
#include "Events\CardUnassignedEvent.h"
#include "Events\CoveredDistanceChangedEvent.h"
#include "Events\EnemyCardPlayedEvent.h"
#include "Events\DefeatEvent.h"
#include "Events\TurnPhaseChangedEvent.h"
#include "Events\VictoryEvent.h"

#include "Systems\AssignmentSystem.h"
#include "Systems\DistanceVictorySystem.h"
#include "Systems\FlagshipDefeatSystem.h"


using namespace PinnedDownGameplay::Systems;
using namespace PinnedDownNet::Components;


TEST(AssignmentTests, AssignPlayerShipToEnemyShip)
{
	// ARRAGE.
	// Create test game.
	auto testData = CreateTestGame<AssignmentSystem>(CardAssignedEvent::CardAssignedEventType);

	// Create source card for player.
	auto source = testData->game->entityManager->CreateEntity();
	auto sourceOwnerComponent = std::make_shared<OwnerComponent>();
	sourceOwnerComponent->owner = 1;
	testData->game->entityManager->AddComponent(source, sourceOwnerComponent);

	// Create target card for enemy.
	auto target = testData->game->entityManager->CreateEntity();

	auto targetOwnerComponent = std::make_shared<OwnerComponent>();
	targetOwnerComponent->owner = INVALID_ENTITY_ID;
	testData->game->entityManager->AddComponent(target, targetOwnerComponent);

	auto targetCardComponent = std::make_shared<CardComponent>();
	targetCardComponent->cardType = CardType::Starship;
	testData->game->entityManager->AddComponent(target, targetCardComponent);

	auto enemyCardPlayedEvent = std::make_shared<EnemyCardPlayedEvent>(target);
	testData->game->eventManager->QueueEvent(enemyCardPlayedEvent);

	// Skip to assignment phase.
	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Assignment);
	testData->game->eventManager->QueueEvent(turnPhaseChangedEvent);

	testData->game->Update(1.0f);

	EXPECT_EQ(false, testData->mock->EventHasOccurred());

	// ACT.
	// Assign card.
	auto assignCardAction = std::make_shared<AssignCardAction>(source, target);
	testData->game->eventManager->QueueEvent(assignCardAction);

	testData->game->Update(1.0f);

	// ASSERT.
	// Check assigned card.
	EXPECT_EQ(true, testData->mock->EventHasOccurred());
}

TEST(AssignmentTests, UnassignPlayerShipWhenAssignedToOtherTarget)
{
	// ARRAGE.
	// Create test game.
	auto testData = CreateTestGame<AssignmentSystem>(CardUnassignedEvent::CardUnassignedEventType);

	// Create source card for player.
	auto source = testData->game->entityManager->CreateEntity();
	auto sourceOwnerComponent = std::make_shared<OwnerComponent>();
	sourceOwnerComponent->owner = 1;
	testData->game->entityManager->AddComponent(source, sourceOwnerComponent);

	// Create first target card for enemy.
	auto firstTarget = testData->game->entityManager->CreateEntity();

	auto firstTargetOwnerComponent = std::make_shared<OwnerComponent>();
	firstTargetOwnerComponent->owner = INVALID_ENTITY_ID;
	testData->game->entityManager->AddComponent(firstTarget, firstTargetOwnerComponent);

	auto firstTargetCardComponent = std::make_shared<CardComponent>();
	firstTargetCardComponent->cardType = CardType::Starship;
	testData->game->entityManager->AddComponent(firstTarget, firstTargetCardComponent);

	auto enemyCardPlayedEvent = std::make_shared<EnemyCardPlayedEvent>(firstTarget);
	testData->game->eventManager->QueueEvent(enemyCardPlayedEvent);

	// Create second target card for enemy.
	auto secondTarget = testData->game->entityManager->CreateEntity();

	auto secondTargetOwnerComponent = std::make_shared<OwnerComponent>();
	secondTargetOwnerComponent->owner = INVALID_ENTITY_ID;
	testData->game->entityManager->AddComponent(secondTarget, secondTargetOwnerComponent);

	auto secondTargetCardComponent = std::make_shared<CardComponent>();
	secondTargetCardComponent->cardType = CardType::Starship;
	testData->game->entityManager->AddComponent(secondTarget, secondTargetCardComponent);

	enemyCardPlayedEvent = std::make_shared<EnemyCardPlayedEvent>(secondTarget);
	testData->game->eventManager->QueueEvent(enemyCardPlayedEvent);

	// Skip to assignment phase.
	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Assignment);
	testData->game->eventManager->QueueEvent(turnPhaseChangedEvent);

	// Assign to first target.
	auto assignCardAction = std::make_shared<AssignCardAction>(source, firstTarget);
	testData->game->eventManager->QueueEvent(assignCardAction);

	testData->game->Update(1.0f);

	EXPECT_EQ(false, testData->mock->EventHasOccurred());

	// ACT.
	// Assign to second target.
	assignCardAction = std::make_shared<AssignCardAction>(source, secondTarget);
	testData->game->eventManager->QueueEvent(assignCardAction);

	testData->game->Update(1.0f);

	// ASSERT.
	// Check assigned card.
	EXPECT_EQ(true, testData->mock->EventHasOccurred());
}

TEST(AssignmentTests, UnassignPlayerShipWhenOtherAssignedToSameTarget)
{
	// ARRAGE.
	// Create test game.
	auto testData = CreateTestGame<AssignmentSystem>(CardUnassignedEvent::CardUnassignedEventType);

	// Create first source card for player.
	auto firstSource = testData->game->entityManager->CreateEntity();
	auto firstSourceOwnerComponent = std::make_shared<OwnerComponent>();
	firstSourceOwnerComponent->owner = 1;
	testData->game->entityManager->AddComponent(firstSource, firstSourceOwnerComponent);

	// Create second source card for player.
	auto secondSource = testData->game->entityManager->CreateEntity();
	auto secondSourceOwnerComponent = std::make_shared<OwnerComponent>();
	secondSourceOwnerComponent->owner = 1;
	testData->game->entityManager->AddComponent(secondSource, secondSourceOwnerComponent);

	// Create target card for enemy.
	auto target = testData->game->entityManager->CreateEntity();

	auto targetOwnerComponent = std::make_shared<OwnerComponent>();
	targetOwnerComponent->owner = INVALID_ENTITY_ID;
	testData->game->entityManager->AddComponent(target, targetOwnerComponent);

	auto targetCardComponent = std::make_shared<CardComponent>();
	targetCardComponent->cardType = CardType::Starship;
	testData->game->entityManager->AddComponent(target, targetCardComponent);

	auto enemyCardPlayedEvent = std::make_shared<EnemyCardPlayedEvent>(target);
	testData->game->eventManager->QueueEvent(enemyCardPlayedEvent);

	// Skip to assignment phase.
	auto turnPhaseChangedEvent = std::make_shared<TurnPhaseChangedEvent>(TurnPhase::Assignment);
	testData->game->eventManager->QueueEvent(turnPhaseChangedEvent);

	// Assign first to target.
	auto assignCardAction = std::make_shared<AssignCardAction>(firstSource, target);
	testData->game->eventManager->QueueEvent(assignCardAction);

	testData->game->Update(1.0f);

	EXPECT_EQ(false, testData->mock->EventHasOccurred());

	// ACT.
	// Assign secondto  target.
	assignCardAction = std::make_shared<AssignCardAction>(secondSource, target);
	testData->game->eventManager->QueueEvent(assignCardAction);

	testData->game->Update(1.0f);

	// ASSERT.
	// Check assigned card.
	EXPECT_EQ(true, testData->mock->EventHasOccurred());
}

TEST(VictoryDefeatTests, DefeatWhenFlagshipDestroyed)
{
	// ARRANGE.
	// Create test game.
	auto testData = CreateTestGame<FlagshipDefeatSystem>(DefeatEvent::DefeatEventType);

	// Create flagship.
	auto entity = testData->game->entityManager->CreateEntity();
	auto flagshipComponent = std::make_shared<FlagshipComponent>();
	testData->game->entityManager->AddComponent(entity, flagshipComponent);
	testData->game->Update(1.0f);

	EXPECT_EQ(false, testData->mock->EventHasOccurred());

	// ACT.
	// Remove flagship.
	testData->game->entityManager->RemoveEntity(entity);
	testData->game->Update(1.0f);

	// ASSERT.
	// Check for defeat.
	EXPECT_EQ(true, testData->mock->EventHasOccurred());
}

TEST(VictoryDefeatTests, VictoryWhenFullDistanceCovered)
{
	// ARRANGE.
	// Create test game.
	auto testData = CreateTestGame<DistanceVictorySystem>(VictoryEvent::VictoryEventType);

	EXPECT_EQ(false, testData->mock->EventHasOccurred());

	// ACT.
	// Change covered distance to maximum.
	auto coveredDistanceChangedEvent = std::make_shared<CoveredDistanceChangedEvent>(5, 5);
	testData->game->eventManager->QueueEvent(coveredDistanceChangedEvent);

	testData->game->Update(1.0f);

	// ASSERT.
	// Check for victory.
	EXPECT_EQ(true, testData->mock->EventHasOccurred());
}


GTEST_API_ int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
