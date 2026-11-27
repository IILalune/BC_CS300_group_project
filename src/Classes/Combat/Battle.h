#ifndef DUNGEON_BATTLE_H
#define DUNGEON_BATTLE_H

#include <iosfwd>

class Character;
class Room;

// InputClosed preserves the current monster's HP and the order of the room's queue.
enum class BattleResult { NoEncounter, Victory, Defeat, InputClosed };

// DungeonMap handles room clearing and loot after RunBattle returns Victory.
BattleResult RunBattle(Character& player, Room& room, std::istream& input,
                       std::ostream& out);

#endif
