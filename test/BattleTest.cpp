#include <gtest/gtest.h>

#include <random>
#include <sstream>

#include "Classes/Character/Monster.h"
#include "Classes/Navigation/DungeonMap.h"
#include "Classes/Navigation/MapGenerator.h"

TEST(BattleTest, SafeRoomsHaveNoEnemiesAndDoNotConsumeAnAttack) {
    std::ostringstream report;
    DungeonMap map(report, std::mt19937(1));
    Character player("Hero");
    std::istringstream input("m\n");
    EXPECT_EQ(map.FightCurrentRoom(player, input), BattleResult::NoEncounter);

    const int start = map.GenerateNewRoom(RoomType::Start, "Entrance");
    const int treasure = map.GenerateNewRoom(RoomType::Treasure, "Vault");
    ASSERT_TRUE(map.ConnectBothRoom(start, treasure));
    EXPECT_EQ(map.CurrentRoom()->enemyCount(), 0);
    EXPECT_EQ(map.FightCurrentRoom(player, input), BattleResult::NoEncounter);
    ASSERT_TRUE(map.MoveToRoom(treasure));
    EXPECT_TRUE(map.CurrentRoom()->canLeave());
    EXPECT_EQ(map.CurrentRoom()->enemyCount(), 0);
    EXPECT_EQ(map.FightCurrentRoom(player, input), BattleResult::NoEncounter);
    EXPECT_FALSE(map.CurrentRoom()->isCleared());
    EXPECT_TRUE(map.OpenCurrentTreasure());
    EXPECT_EQ(input.peek(), 'm');
    EXPECT_EQ(player.getHealth(), player.getMaxHealth());
}

TEST(BattleTest, BossLocksExitsUntilVictoryAndDoesNotRespawn) {
    std::ostringstream report;
    DungeonMap map(report, std::mt19937(2));
    Character player("Hero");
    const int start = map.GenerateNewRoom(RoomType::Start, "Entrance");
    const int boss = map.GenerateNewRoom(RoomType::Boss, "Throne room");
    ASSERT_TRUE(map.ConnectBothRoom(start, boss));
    const Room* room = map.GetRoom(boss);
    ASSERT_NE(room, nullptr);
    EXPECT_EQ(room->enemyCount(), 0);
    ASSERT_TRUE(map.MoveToRoom(boss));
    ASSERT_EQ(room->enemyCount(), 1);
    auto dragon = room->frontEnemy();
    ASSERT_NE(dragon, nullptr);
    EXPECT_EQ(dragon->getName(), "Dragon");
    EXPECT_EQ(dragon->attackStyle(), AttackStyle::Magic);
    EXPECT_EQ(dragon->getHealth(), 100);
    EXPECT_FALSE(room->canLeave());
    EXPECT_FALSE(map.ClearCurrentRoom());
    EXPECT_TRUE(map.AvailableRoom().empty());
    EXPECT_FALSE(map.MoveToRoom(start));
    EXPECT_FALSE(map.GoBack());
    EXPECT_EQ(map.CurrentRoom(), room);

    std::istringstream attacks("m\nm\nm\nm\n");
    ASSERT_EQ(map.FightCurrentRoom(player, attacks), BattleResult::Victory);
    EXPECT_EQ(player.getHealth(), 100 - 3 * 12);
    EXPECT_EQ(room->enemyCount(), 0);
    EXPECT_TRUE(room->isCleared());
    EXPECT_TRUE(room->canLeave());
    ASSERT_TRUE(map.GoBack());
    EXPECT_EQ(map.CurrentRoom()->getRoomID(), start);
    ASSERT_TRUE(map.MoveToRoom(boss));
    EXPECT_EQ(room->enemyCount(), 0);
    EXPECT_EQ(room->frontEnemy(), nullptr);
    EXPECT_EQ(map.FightCurrentRoom(player, attacks), BattleResult::NoEncounter);
}

TEST(BattleTest, InvalidInputAndEndOfStreamPreserveTheEncounter) {
    std::ostringstream report;
    DungeonMap map(report, std::mt19937(3));
    Character player("Hero");
    map.GenerateNewRoom(RoomType::Boss, "Arena");
    const Room* room = map.CurrentRoom();
    auto dragon = room->frontEnemy();
    ASSERT_NE(dragon, nullptr);
    std::istringstream invalid("\nx\nm extra\n");
    ASSERT_EQ(map.FightCurrentRoom(player, invalid), BattleResult::InputClosed);
    EXPECT_EQ(player.getHealth(), 100);
    EXPECT_EQ(dragon->getHealth(), 100);

    std::istringstream oneTurn("bad\nm\n");
    ASSERT_EQ(map.FightCurrentRoom(player, oneTurn), BattleResult::InputClosed);
    EXPECT_EQ(room->frontEnemy(), dragon);
    EXPECT_EQ(room->enemyCount(), 1);
    EXPECT_EQ(dragon->getHealth(), 75);
    EXPECT_EQ(player.getHealth(), 88);
    EXPECT_FALSE(room->isCleared());
    EXPECT_FALSE(room->canLeave());
}

TEST(BattleTest, EnemyOverkillEndsTheFightWhileTheOpponentRemains) {
    std::ostringstream report;
    DungeonMap map(report, std::mt19937(4));
    Character player("Hero");
    player.takeDamage(95);
    map.GenerateNewRoom(RoomType::Boss, "Arena");
    std::istringstream attacks("m\nm\n");
    ASSERT_EQ(map.FightCurrentRoom(player, attacks), BattleResult::Defeat);
    EXPECT_LE(player.getHealth(), 0);
    const Room* room = map.CurrentRoom();
    EXPECT_FALSE(room->isCleared());
    EXPECT_FALSE(room->canLeave());
    ASSERT_EQ(room->enemyCount(), 1);
    ASSERT_NE(room->frontEnemy(), nullptr);
    EXPECT_EQ(room->frontEnemy()->getHealth(), 75);
    EXPECT_EQ(attacks.peek(), 'm');
}

TEST(BattleTest, NormalRoomFightsOneQueuedEnemyAtATime) {
    bool sawMultipleEnemies = false;
    // Check the rolled groups without depending on any one library's seed-to-roll mapping.
    for (unsigned seed = 0; seed < 32; ++seed) {
        SCOPED_TRACE(seed);
        std::ostringstream report;
        DungeonMap map(report, std::mt19937(seed));
        Character player("Hero");
        map.GenerateNewRoom(RoomType::Normal, "Guard post");
        const Room* room = map.CurrentRoom();
        const int count = room->enemyCount();
        ASSERT_GE(count, 1);
        ASSERT_LE(count, 3);
        if (count > 1) sawMultipleEnemies = true;
        for (int remaining = count; remaining > 0; --remaining) {
            auto front = room->frontEnemy();
            ASSERT_NE(front, nullptr);
            if (front->getName() == "Goblin") {
                EXPECT_EQ(front->attackStyle(), AttackStyle::Physical);
            } else {
                EXPECT_EQ(front->getName(), "Wisp");
                EXPECT_EQ(front->attackStyle(), AttackStyle::Magic);
            }
            ASSERT_EQ(front->getHealth(), front->getMaxHealth());
            const int healthBeforeRound = player.getHealth();
            std::istringstream first("m\n");
            ASSERT_EQ(map.FightCurrentRoom(player, first), BattleResult::InputClosed);
            EXPECT_EQ(room->frontEnemy(), front);
            EXPECT_EQ(room->enemyCount(), remaining);
            EXPECT_EQ(front->getHealth(), front->getMaxHealth() - 25);
            const int counterDamage = healthBeforeRound - player.getHealth();
            if (front->attackStyle() == AttackStyle::Magic) {
                EXPECT_EQ(counterDamage, 6);
            } else {
                EXPECT_GE(counterDamage, 5);
                EXPECT_LE(counterDamage, 18);
            }

            const int healthBeforeKill = player.getHealth();
            std::istringstream second("m\n");
            const auto expected = remaining == 1 ? BattleResult::Victory : BattleResult::InputClosed;
            ASSERT_EQ(map.FightCurrentRoom(player, second), expected);
            EXPECT_LE(front->getHealth(), 0);
            EXPECT_EQ(room->enemyCount(), remaining - 1);
            EXPECT_NE(room->frontEnemy(), front);
            EXPECT_EQ(player.getHealth(), healthBeforeKill);
        }
        EXPECT_TRUE(room->isCleared());
        EXPECT_TRUE(room->canLeave());
    }
    EXPECT_TRUE(sawMultipleEnemies);
}

TEST(BattleTest, PhysicalAttackKillsEachNormalOpponentBeforeItCanCounter) {
    std::ostringstream report;
    DungeonMap map(report, std::mt19937(6));
    Character player("Hero");
    map.GenerateNewRoom(RoomType::Normal, "Hall");
    // Every physical roll does at least 40 damage. Both normal monsters start below that.
    std::istringstream attacks("p\np\np\n");
    ASSERT_EQ(map.FightCurrentRoom(player, attacks), BattleResult::Victory);
    EXPECT_EQ(player.getHealth(), player.getMaxHealth());
    EXPECT_TRUE(map.CurrentRoom()->isCleared());
}

TEST(BattleTest, GeneratedFloorStartsSafeAndSpawnsOnTravelToACombatRoom) {
    std::mt19937 rng(5);
    std::ostringstream report;
    auto map = GenerateFloor(rng, report);
    ASSERT_NE(map, nullptr);
    ASSERT_NE(map->CurrentRoom(), nullptr);
    EXPECT_EQ(map->CurrentRoom()->getType(), RoomType::Start);
    EXPECT_EQ(map->CurrentRoom()->enemyCount(), 0);
    EXPECT_TRUE(map->AllRoomsConnected());
    const auto exits = map->AvailableRoom();
    ASSERT_FALSE(exits.empty());
    const int firstRoom = exits.front();
    ASSERT_EQ(map->GetRoom(firstRoom)->getType(), RoomType::Normal);
    EXPECT_EQ(map->GetRoom(firstRoom)->enemyCount(), 0);
    ASSERT_TRUE(map->MoveToRoom(firstRoom));
    EXPECT_GE(map->CurrentRoom()->enemyCount(), 1);
    EXPECT_LE(map->CurrentRoom()->enemyCount(), 3);
    EXPECT_FALSE(map->GoBack());
}
