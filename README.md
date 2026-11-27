# BC_CS300_group_project

This branch adds dungeon navigation and turn-based combat to the group project. It also includes the changes to shared character, queue and item code needed to run encounters, award loot and drink potions. The change list below uses `Main` at `fb37e16` as its starting point.

## Using the modules from the group game

The group game supplies its own entry point. Compile the `.cpp` files under `src/Classes/` and use the four include directories shown in the test command below. The list and queue headers include their own template `.cpp` files.

1. Create the player and call `GenerateFloor(rng, report)`. Supply an output stream such as `std::cout` or a log file. The map writes room events, combat actions and pickup results to that stream.
2. Use `AvailableRoom()` to list exits and `player.move(*floor, roomID)` to enter a room. After travel, call `floor->FightCurrentRoom(player, input)`. It handles the enemy queue, the first victory and the drops. A safe or cleared room returns `NoEncounter`. On `Defeat`, end the run. On `InputClosed`, stop reading commands.
3. During a fight the player can enter `p` for physical attack, `m` for magic or `i` to see the potion bag. Choose a numbered potion to drink it. Use `b` or `0` in the bag to return to the fight.
4. The group's exploration menu can call `GoBack()` to retrace a step, `OpenCurrentTreasure(player)` to open a chest and `CollectCurrentLoot(player)` to pick up items still in the room.

## Navigation and combat changes

| File | What to review |
|---|---|
| [MapGenerator.cpp](src/Classes/Navigation/MapGenerator.cpp) and [header](src/Classes/Navigation/MapGenerator.h) | `GenerateFloor` builds a main path from the entrance to the dragon and adds branches ending in chests. It uses the map's existing breadth-first search to check that every room is reachable. |
| [DungeonMap.cpp](src/Classes/Navigation/DungeonMap.cpp) and [header](src/Classes/Navigation/DungeonMap.h) | `enterRoom` prepares the encounter and updates the backtracking stack. `FightCurrentRoom` calls the battle loop and records the first victory. `OpenCurrentTreasure` and `CollectCurrentLoot` use the current room's stored rewards. The existing linked lists store rooms and exits, and insertion sort orders the available exit IDs. |
| [Room.cpp](src/Classes/Navigation/Room.cpp) and [header](src/Classes/Navigation/Room.h) | `prepareEncounter` queues 1–3 random goblins or wisps on first entry to a normal room, or one dragon in a boss room. `finishRoom` generates loot once. `collectLoot` keeps any items that the player's bag could not accept. |
| [Battle.cpp](src/Classes/Combat/Battle.cpp) and [header](src/Classes/Combat/Battle.h) | `RunBattle` fights only the front enemy in the course's FIFO queue. After a successful action, that enemy counters if it survived. A defeated enemy leaves the queue and the player goes first against the next one. Defeat ends the run. |
| [Monster.cpp](src/Classes/Character/Monster.cpp) and [header](src/Classes/Character/Monster.h) | Defines the three encounter presets. Goblins use physical attacks. Wisps and the dragon use magic. |

## Changes to shared group code

These edits are included so teammates can review the integration with their modules separately.

| Shared files | What changed and why |
|---|---|
| [Character.cpp](src/Classes/Character/Character.cpp) and [header](src/Classes/Character/Character.h) | A protected `StartingStats` constructor lets each enemy supply its starting values. The public player constructor keeps its original values. `useConsumableItem` checks the selected bag slot and HP, calls the item's effect and removes one potion after use. The header forward-declares `DungeonMap` to avoid a circular include. |
| [ItemId.cpp](src/Classes/Items%20and%20Skills/ItemId.cpp) and [header](src/Classes/Items%20and%20Skills/ItemId.h) | Added `ItemRegistry::inventoryItem(id)`. Room drops pass this pointer to the existing `Character::addItem` interface. A shared registry keeps the definitions alive while bag slots reference them. The existing item IDs, values and `itemDef` lookup keep their original meanings. |
| [Consumable.cpp](src/Classes/Items%20and%20Skills/ItemTypes/Consumable.cpp) | The original `useItem` produced a message. It now applies the item's heal value, capped at the character's missing HP. The existing health potion restores up to 5 HP. The battle loop spends one turn only when drinking succeeds. |
| [Armor.cpp](src/Classes/Items%20and%20Skills/ItemTypes/Armor.cpp), [Weapon.cpp](src/Classes/Items%20and%20Skills/ItemTypes/Weapon.cpp) and the `Consumable.cpp` constructor | Removed repeated default arguments from the constructor definitions so the source compiles. Callers still get the defaults from the headers. |
| [ArrayQueue.cpp](src/Data%20Structure/ArrayQueue.cpp) and [header](src/Data%20Structure/ArrayQueue.h) | The template definitions are included after the class declaration. `dequeue` and `clear` release stored references so the room queue gives up ownership of defeated enemies. |

The integration uses the existing `Inventory`, `Equipment` and `BaseItem` interfaces. Physical damage, magic damage and the underlying HP subtraction follow the existing character code. The battle loop treats HP at or below zero as defeat.

## First-clear rewards

| First event | Health potion | Equipment |
|---|---:|---:|
| Normal room cleared | 20% | 5% |
| Boss defeated | 100% | 20% |
| Treasure chest opened | 100% | 100% |

Potion and equipment rolls are independent. Each room drops at most one of each. Equipment is an equal choice between an iron sword and an iron shield. The first clear or chest opening tries to put each item in its own bag. When a bag is full, the item stays in that room. After returning with space, call `CollectCurrentLoot(player)` to collect that item. Each room rolls its reward only once.

## Tests

[Queue tests](test/ArrayQueueTest.cpp) cover FIFO order, wraparound, growth and reference release. [Battle tests](test/BattleTest.cpp) cover room entry, opponent order, boss victory, defeat and interrupted input. [Loot tests](test/LootTest.cpp) cover first rewards, repeat visits, full bags, potion healing and turn use.

Google Test supplies the test program's `main()` through `gtest_main`. To run all three suites from the repository root in Bash with a C++20 compiler and Google Test installed:

```bash
mkdir -p build
mapfile -d '' sources < <(find src/Classes -name '*.cpp' -print0)
g++ -std=c++20 -Isrc -I'src/Data Structure' -I'src/Classes/Navigation' \
    -I'src/Classes/Items and Skills' "${sources[@]}" test/*Test.cpp \
    -lgtest_main -lgtest -pthread -o build/dungeon-tests
./build/dungeon-tests
```
