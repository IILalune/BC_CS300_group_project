#include "DungeonMap.h"

#include <queue>
#include <stdexcept>

DungeonMap::DungeonMap(std::ostream& report) : out(report) {}

std::shared_ptr<Room> DungeonMap::findRoom(int id) const {
  // Rooms are only appended, so each ID is its one-based list position.
  if (id < 1 || id > rooms.getLength()) return nullptr;
  return rooms.getEntry(id);
}

const Room* DungeonMap::GetRoom(int id) const {
  return findRoom(id).get();  // The list owns the room. Callers may inspect it.
}

int DungeonMap::GenerateNewRoom(RoomType type, const std::string& description) {
  const int id = rooms.getLength() + 1;
  auto room = std::make_shared<Room>(id, type, description);
  if (!rooms.insert(id, room))
    throw std::runtime_error("Room insertion failed");
  if (currentRoomID == -1) {
    currentRoomID = id;
    room->visited = true;
  }
  return id;
}

bool DungeonMap::ConnectBothRoom(int a, int b) {
  auto first = findRoom(a);
  auto second = findRoom(b);
  if (!first || !second || a == b || first->isConnectedTo(b)) return false;
  const int pos = first->neighbors.getLength() + 1;
  if (!first->neighbors.insert(pos, b)) return false;
  // If the reverse link fails, undo the first link to keep travel two-way.
  try {
    if (second->neighbors.insert(second->neighbors.getLength() + 1, a))
      return true;
  } catch (...) {
    first->neighbors.remove(pos);
    throw;
  }
  first->neighbors.remove(pos);
  return false;
}

void DungeonMap::PrintCurrentRoom() const {
  const Room* room = CurrentRoom();
  if (room)
    room->printStatus(out);
  else
    out << "No current room.\n";
}

std::vector<int> DungeonMap::AvailableRoom() const {
  std::vector<int> options;
  const Room* current = CurrentRoom();
  const bool canLeave = current && current->canLeave();
  if (canLeave) {
    const auto& exits = current->getNeighbors();
    for (int pos = 1; pos <= exits.getLength(); ++pos) {
      const int id = exits.getEntry(pos);
      const Room* target = GetRoom(id);
      if (target && target->canEnter()) options.push_back(id);
    }
  }
  // Insertion sort shifts larger IDs right to make room for the next ID.
  for (std::size_t i = 1; i < options.size(); ++i) {
    const int next = options[i];
    std::size_t pos = i;
    while (pos > 0 && options[pos - 1] > next) {
      options[pos] = options[pos - 1];
      --pos;
    }
    options[pos] = next;
  }
  out << "Routes";
  if (canLeave) out << " from #" << currentRoomID;
  for (int id : options)
    out << "\n  #" << id << " - " << GetRoom(id)->getCurrentRoomInfo();
  if (options.empty()) out << ": no accessible exits.";
  out << '\n';
  return options;
}

std::shared_ptr<Room> DungeonMap::travelTarget(int id) const {
  // Moving and backtracking share this check. Rejection preserves both location and history.
  auto target = findRoom(id);
  const Room* current = CurrentRoom();
  if (!current || !target || !current->canLeave() ||
      !current->isConnectedTo(id) || !target->canEnter()) {
    out << "Cannot travel to #" << id << " from here.\n";
    return nullptr;
  }
  return target;
}

void DungeonMap::enterRoom(const std::shared_ptr<Room>& target) {
  const bool returning = target->visited;
  currentRoomID = target->getRoomID();
  target->visited = true;
  out << (returning ? "Re-entered " : "Entered ") << "room #" << currentRoomID
      << ".\n";
  target->printStatus(out);
}

bool DungeonMap::MoveToRoom(int id) {
  auto target = travelTarget(id);
  if (!target) return false;
  history.push(currentRoomID);  // Record the starting room after validating the move.
  enterRoom(target);
  return true;
}

bool DungeonMap::GoBack() {
  if (history.empty()) {
    out << "No previous room to return to.\n";
    return false;
  }
  auto target = travelTarget(history.top());
  if (!target) return false;
  enterRoom(target);
  history.pop();  // Calling MoveToRoom here would add a new history entry.
  return true;
}

bool DungeonMap::ClearCurrentRoom() {
  auto room = findRoom(currentRoomID);
  if (room) return room->clearEncounter(out);
  out << "No current room.\n";
  return false;
}

bool DungeonMap::OpenCurrentTreasure() {
  auto room = findRoom(currentRoomID);
  if (room) return room->openTreasure(out);
  out << "No current room.\n";
  return false;
}

bool DungeonMap::AllRoomsConnected() const {
  const int count = GetRoomCount();
  if (count == 0) return false;
  std::queue<int> pending;
  std::vector<bool> seen(count + 1, false);
  seen[1] = true;
  pending.push(1);  // Run BFS from the entrance regardless of player position.
  int reached = 0;
  while (!pending.empty()) {
    const int id = pending.front();
    pending.pop();
    ++reached;
    const auto& exits = GetRoom(id)->getNeighbors();
    for (int pos = 1; pos <= exits.getLength(); ++pos) {
      const int next = exits.getEntry(pos);
      if (next < 1 || next > count) return false;
      if (!seen[next]) {
        seen[next] = true;  // Mark on enqueue so cycles cannot queue the same room twice.
        pending.push(next);
      }
    }
  }
  return reached == count;
}
