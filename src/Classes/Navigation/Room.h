#ifndef DRAGON_ROOM_H
#define DRAGON_ROOM_H

#include <iosfwd>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "../../Data Structure/ArrayQueue.h"
#include "../../Data Structure/LinkedList.h"

class Character;
class Monster;
enum class RoomType { Start, Normal, Boss, Treasure };

class Room {
 public:
  Room(int id, RoomType kind, const std::string& text);
  Room(const Room&) = delete;
  Room& operator=(const Room&) = delete;

  int getRoomID() const { return roomID; }
  RoomType getType() const { return type; }
  bool isVisited() const { return visited; }
  bool isCleared() const { return cleared; }
  const std::string& getCurrentRoomInfo() const { return description; }
  const LinkedList<int>& getNeighbors() const { return neighbors; }

  bool isConnectedTo(int targetID) const;
  void printStatus(std::ostream& out) const;
  int enemyCount() const;
  std::shared_ptr<Monster> frontEnemy() const;
  bool removeDefeatedEnemy();

  bool canEnter() const { return true; }  // TODO: Check key and door requirements.
  bool canLeave() const;

 private:
  // DungeonMap manages two-way passages and the player's location. Other
  // modules ask DungeonMap to record a victory or open a chest.
  friend class DungeonMap;
  bool isCombatRoom() const;
  void prepareEncounter(std::mt19937& rng, std::ostream& out);
  bool clearEncounter(std::mt19937& lootRng, std::ostream& out);
  bool openTreasure(std::mt19937& lootRng, std::ostream& out);
  void finishRoom(std::mt19937& lootRng, std::ostream& out);
  void collectLoot(Character& player, std::ostream& out);

  int roomID;
  RoomType type;
  std::string description;
  LinkedList<int> neighbors;  // IDs avoid shared ownership between rooms.
  // Battle reads the front monster each turn and removes it after defeat.
  ArrayQueue<std::shared_ptr<Monster>> enemies;
  std::vector<int> pendingLoot;  // IDs stay here until the appropriate bag accepts them.
  bool encounterPrepared = false;
  bool visited = false;       // True once the player has entered this room.
  bool cleared;               // Enemies defeated or treasure chest opened.
};

#endif
