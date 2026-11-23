#ifndef DUNGEON_MAP_H
#define DUNGEON_MAP_H

#include <iostream>
#include <memory>
#include <stack>
#include <vector>

#include "Room.h"

class DungeonMap {
 public:
  // DungeonMap stores a reference, so the report stream must outlive it.
  explicit DungeonMap(std::ostream& report = std::cout);
  DungeonMap(const DungeonMap&) = delete;
  DungeonMap& operator=(const DungeonMap&) = delete;

  int GenerateNewRoom(RoomType type, const std::string& description);
  bool ConnectBothRoom(int a, int b);
  const Room* GetRoom(int id) const;
  const Room* CurrentRoom() const { return GetRoom(currentRoomID); }
  int GetRoomCount() const { return rooms.getLength(); }
  void PrintCurrentRoom() const;
  std::vector<int> AvailableRoom() const;
  bool MoveToRoom(int id);
  bool GoBack();
  bool ClearCurrentRoom();  // Call after the combat system confirms victory.
  bool OpenCurrentTreasure();
  bool AllRoomsConnected() const;

 private:
  std::shared_ptr<Room> findRoom(int id) const;
  std::shared_ptr<Room> travelTarget(int id) const;
  void enterRoom(const std::shared_ptr<Room>& target);

  LinkedList<std::shared_ptr<Room>> rooms;
  std::stack<int> history;  // Tracks the route. Room state stays in rooms.
  int currentRoomID = -1;
  std::ostream& out;
};
#endif
