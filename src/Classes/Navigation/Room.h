#ifndef DRAGON_ROOM_H
#define DRAGON_ROOM_H

#include <iosfwd>
#include <string>

#include "../../Data Structure/LinkedList.h"

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

  bool canEnter() const { return true; }  // TODO: Check key and door requirements.
  bool canLeave() const { return true; }  // TODO: Apply the combat retreat rules.

 private:
  // DungeonMap manages two-way passages and the player's location. Other
  // modules ask DungeonMap to record a victory or open a chest.
  friend class DungeonMap;
  bool clearEncounter(std::ostream& out);
  bool openTreasure(std::ostream& out);

  int roomID;
  RoomType type;
  std::string description;
  LinkedList<int> neighbors;  // IDs avoid shared ownership between rooms.
  bool visited = false;       // True once the player has entered this room.
  bool cleared;               // Enemies defeated or treasure chest opened.
};

#endif
