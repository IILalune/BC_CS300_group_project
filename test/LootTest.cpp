#include <gtest/gtest.h>

#include <random>
#include <sstream>
#include <string>

#include "Classes/Character/Monster.h"
#include "Classes/Items and Skills/ItemId.h"
#include "Classes/Navigation/DungeonMap.h"

namespace {
BattleResult fight(DungeonMap& map, Character& player, const char* commands) {
    std::istringstream input(commands);
    return map.FightCurrentRoom(player, input);
}
}  // namespace

TEST(LootTest, NormalVictoryCanDropNeitherItemEitherItemOrBothExactlyOnce) {
    bool seen[2][2] = {};
    int differentOutcomes = 0;
    for (unsigned seed = 0; seed < 2048 && differentOutcomes < 4; ++seed) {
        SCOPED_TRACE(seed);
        std::ostringstream report;
        DungeonMap map(report, std::mt19937(seed));
        Character player("Hero");
        map.GenerateNewRoom(RoomType::Normal, "Guard post");
        ASSERT_EQ(fight(map, player, "p\np\np\n"), BattleResult::Victory);

        const int potions = player.getConsumableItemCount();
        const int gear = player.getGearItemCount();
        ASSERT_GE(potions, 0);
        ASSERT_LE(potions, 1);
        ASSERT_GE(gear, 0);
        ASSERT_LE(gear, 1);
        if (potions) {
            EXPECT_EQ(player.getConsumableItem(1)->getItemId(), ItemId::HealthPotion);
        }
        if (gear) {
            const int id = player.getGearItem(1)->getItemId();
            EXPECT_TRUE(id == ItemId::IronSword || id == ItemId::IronShield);
        }
        if (!seen[potions][gear]) {
            seen[potions][gear] = true;
            ++differentOutcomes;
        }
        EXPECT_EQ(fight(map, player, ""), BattleResult::NoEncounter);
        map.CollectCurrentLoot(player);
        EXPECT_EQ(player.getConsumableItemCount(), potions);
        EXPECT_EQ(player.getGearItemCount(), gear);
    }
    EXPECT_EQ(differentOutcomes, 4);
}

TEST(LootTest, OpeningAChestAwardsOnceEvenAfterLeavingAndReturning) {
    std::ostringstream report;
    DungeonMap map(report, std::mt19937(42));
    Character player("Hero");
    const int start = map.GenerateNewRoom(RoomType::Start, "Entrance");
    const int chest = map.GenerateNewRoom(RoomType::Treasure, "Vault");
    ASSERT_TRUE(map.ConnectBothRoom(start, chest));
    EXPECT_FALSE(map.OpenCurrentTreasure(player));
    ASSERT_TRUE(map.MoveToRoom(chest));
    map.CollectCurrentLoot(player);
    EXPECT_EQ(player.getConsumableItemCount(), 0);
    EXPECT_EQ(player.getGearItemCount(), 0);
    ASSERT_TRUE(map.OpenCurrentTreasure(player));
    ASSERT_EQ(player.getConsumableItemCount(), 1);
    EXPECT_EQ(player.getConsumableItem(1)->getItemId(), ItemId::HealthPotion);
    ASSERT_EQ(player.getGearItemCount(), 1);
    const int gearId = player.getGearItem(1)->getItemId();
    EXPECT_TRUE(gearId == ItemId::IronSword || gearId == ItemId::IronShield);
    EXPECT_NE(report.str().find("dropped: Health Potion"), std::string::npos);
    EXPECT_NE(report.str().find("Health Potion added to consumable bag"), std::string::npos);
    const int potions = player.getConsumableItemCount();
    const int gear = player.getGearItemCount();
    EXPECT_FALSE(map.OpenCurrentTreasure(player));
    ASSERT_TRUE(map.GoBack());
    ASSERT_TRUE(map.MoveToRoom(chest));
    EXPECT_FALSE(map.OpenCurrentTreasure(player));
    map.CollectCurrentLoot(player);
    EXPECT_EQ(player.getConsumableItemCount(), potions);
    EXPECT_EQ(player.getGearItemCount(), gear);
}

TEST(LootTest, EarlyClearAndDefeatDoNotAwardLoot) {
    std::ostringstream report;
    DungeonMap map(report, std::mt19937(4));
    Character player("Hero");
    map.GenerateNewRoom(RoomType::Boss, "Dragon den");
    EXPECT_FALSE(map.ClearCurrentRoom());
    EXPECT_EQ(fight(map, player, ""), BattleResult::InputClosed);
    map.CollectCurrentLoot(player);
    EXPECT_EQ(player.getConsumableItemCount(), 0);
    EXPECT_EQ(player.getGearItemCount(), 0);
    player.takeDamage(95);
    ASSERT_EQ(fight(map, player, "m\n"), BattleResult::Defeat);
    EXPECT_LT(player.getHealth(), 0);
    EXPECT_FALSE(map.CurrentRoom()->isCleared());
    map.CollectCurrentLoot(player);
    EXPECT_EQ(player.getConsumableItemCount(), 0);
    EXPECT_EQ(player.getGearItemCount(), 0);
}

TEST(LootTest, FullBagsLeaveBothBossDropsForSeparateRetriesInThatRoom) {
    // Find a boss with both drops without relying on a library-specific sequence.
    int gearSeed = -1;
    int gearId = -1;
    bool sawPotionOnly = false;
    for (int seed = 0; seed < 256 && (gearSeed < 0 || !sawPotionOnly); ++seed) {
        std::ostringstream report;
        DungeonMap map(report, std::mt19937(seed));
        Character player("Hero");
        map.GenerateNewRoom(RoomType::Boss, "Dragon den");
        ASSERT_EQ(fight(map, player, "m\nm\nm\nm\n"), BattleResult::Victory);
        ASSERT_EQ(player.getConsumableItemCount(), 1);
        EXPECT_EQ(player.getConsumableItem(1)->getItemId(), ItemId::HealthPotion);
        if (player.getGearItemCount() == 0) {
            sawPotionOnly = true;
        } else {
            ASSERT_EQ(player.getGearItemCount(), 1);
            gearSeed = seed;
            gearId = player.getGearItem(1)->getItemId();
        }
    }
    ASSERT_GE(gearSeed, 0);
    ASSERT_TRUE(sawPotionOnly);
    ASSERT_TRUE(gearId == ItemId::IronSword || gearId == ItemId::IronShield);

    std::ostringstream report;
    DungeonMap map(report, std::mt19937(gearSeed));
    Character player("Hero");
    const int boss = map.GenerateNewRoom(RoomType::Boss, "Dragon den");
    const int safe = map.GenerateNewRoom(RoomType::Start, "Camp");
    ASSERT_TRUE(map.ConnectBothRoom(boss, safe));
    for (int i = 0; i < 5; ++i) {
        ASSERT_TRUE(player.addItem(ItemRegistry::inventoryItem(ItemId::HealthPotion)));
    }
    for (int i = 0; i < 10; ++i) {
        ASSERT_TRUE(player.addItem(ItemRegistry::inventoryItem(ItemId::IronSword)));
    }
    ASSERT_EQ(fight(map, player, "m\nm\nm\nm\n"), BattleResult::Victory);
    EXPECT_EQ(player.getConsumableItemCount(), 5);
    EXPECT_EQ(player.getGearItemCount(), 10);
    EXPECT_NE(report.str().find("consumable bag full"), std::string::npos);
    EXPECT_NE(report.str().find("gear bag full"), std::string::npos);

    ASSERT_TRUE(player.removeConsumableItem(1));
    map.CollectCurrentLoot(player);
    ASSERT_EQ(player.getConsumableItemCount(), 5);
    EXPECT_EQ(player.getGearItemCount(), 10);
    ASSERT_TRUE(player.removeConsumableItem(1));
    ASSERT_TRUE(player.removeGearItem(1));
    ASSERT_TRUE(map.MoveToRoom(safe));
    map.CollectCurrentLoot(player);
    EXPECT_EQ(player.getGearItemCount(), 9);
    ASSERT_TRUE(map.GoBack());
    map.CollectCurrentLoot(player);
    EXPECT_EQ(player.getConsumableItemCount(), 4);
    ASSERT_EQ(player.getGearItemCount(), 10);
    EXPECT_EQ(player.getGearItem(10)->getItemId(), gearId);
    ASSERT_TRUE(player.removeGearItem(1));
    map.CollectCurrentLoot(player);
    EXPECT_EQ(player.getGearItemCount(), 9);
    EXPECT_EQ(player.getConsumableItemCount(), 4);
}

TEST(LootTest, UsePotionCapsHealthRemovesOneSlotAndLeavesGearAlone) {
    Character player("Hero");
    auto potion = ItemRegistry::inventoryItem(ItemId::HealthPotion);
    auto sword = ItemRegistry::inventoryItem(ItemId::IronSword);
    ASSERT_TRUE(player.addItem(sword));
    ASSERT_TRUE(player.addItem(potion));
    ASSERT_TRUE(player.addItem(potion));
    EXPECT_FALSE(player.useConsumableItem(1));
    player.takeDamage(2);
    EXPECT_FALSE(player.useConsumableItem(0));
    EXPECT_FALSE(player.useConsumableItem(3));
    EXPECT_EQ(player.getConsumableItemCount(), 2);
    ASSERT_TRUE(player.useConsumableItem(2));
    EXPECT_EQ(player.getHealth(), player.getMaxHealth());
    EXPECT_EQ(player.getConsumableItemCount(), 1);
    EXPECT_EQ(player.getConsumableItem(1), potion);
    EXPECT_EQ(player.getGearItemCount(), 1);
    EXPECT_EQ(player.getGearItem(1), sword);
}

TEST(LootTest, DrinkingIsOneTurnAndOnlyTheLivingOpponentAttacks) {
    std::ostringstream report;
    DungeonMap map(report, std::mt19937(6));
    Character player("Hero");
    map.GenerateNewRoom(RoomType::Boss, "Dragon den");
    player.takeDamage(20);
    ASSERT_TRUE(player.addItem(ItemRegistry::inventoryItem(ItemId::HealthPotion)));
    ASSERT_EQ(fight(map, player, "i\n1\n"), BattleResult::InputClosed);
    EXPECT_EQ(player.getConsumableItemCount(), 0);
    EXPECT_EQ(player.getHealth(), 80 + 5 - 12);
    EXPECT_EQ(map.CurrentRoom()->frontEnemy()->getHealth(), 100);
    EXPECT_NE(report.str().find("Health Potion"), std::string::npos);
}

TEST(LootTest, FailedInventoryChoicesAndEofAreFreeAndCancelReturnsToCombat) {
    std::ostringstream report;
    DungeonMap map(report, std::mt19937(7));
    Character player("Hero");
    map.GenerateNewRoom(RoomType::Boss, "Dragon den");
    EXPECT_EQ(fight(map, player, "i\n"), BattleResult::InputClosed);
    EXPECT_EQ(player.getHealth(), 100);
    ASSERT_TRUE(player.addItem(ItemRegistry::inventoryItem(ItemId::HealthPotion)));
    EXPECT_EQ(fight(map, player, "i\n1\n"), BattleResult::InputClosed);
    EXPECT_EQ(player.getHealth(), 100);
    EXPECT_EQ(player.getConsumableItemCount(), 1);
    player.takeDamage(10);
    for (const char* commands : {"i\n", "i\n99\n", "i\nbad\n", "i\n0\n", "i\nb\n"}) {
        SCOPED_TRACE(commands);
        EXPECT_EQ(fight(map, player, commands), BattleResult::InputClosed);
        EXPECT_EQ(player.getConsumableItemCount(), 1);
        EXPECT_EQ(player.getHealth(), 90);
        EXPECT_EQ(map.CurrentRoom()->frontEnemy()->getHealth(), 100);
    }
    EXPECT_EQ(fight(map, player, "i\nb\nm\n"), BattleResult::InputClosed);
    EXPECT_EQ(map.CurrentRoom()->frontEnemy()->getHealth(), 75);
    EXPECT_EQ(player.getHealth(), 90 - 12);
}
