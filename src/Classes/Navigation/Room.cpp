#include "Room.h"

#include <ostream>

Room::Room(int id, RoomType kind, const std::string& text)
    : roomID(id),
      type(kind),
      description(text),
      cleared(kind == RoomType::Start) {}

bool Room::isConnectedTo(int targetID) const {
  for (int pos = 1; pos <= neighbors.getLength(); ++pos) {
    if (neighbors.getEntry(pos) == targetID) return true;
  }
  return false;
}

void Room::printStatus(std::ostream& out) const {
  out << "Room #" << roomID << " - " << description << ". ";
  switch (type) {
    case RoomType::Start:
      out << "Safe entrance.";
      break;
    case RoomType::Normal:
      out << (cleared ? "Room cleared." : "Enemies remain.");
      break;
    case RoomType::Boss:
      out << (cleared ? "Boss defeated." : "Boss encounter awaits.");
      break;
    case RoomType::Treasure:
      out << (cleared ? "Treasure chest opened." : "Treasure chest sealed.");
      break;
  }
  out << '\n';
}

bool Room::clearEncounter(std::ostream& out) {
  if (type != RoomType::Normal && type != RoomType::Boss) {
    out << "There is no combat encounter here.\n";
    return false;
  }
  if (cleared) {
    out << "Room already cleared.\n";
    return false;
  }

  // The combat system should report victory after the last enemy falls.
  cleared = true;
  out << "Room #" << roomID << " cleared. All enemies defeated.\n";

  // TODO: On the first clear, award a variable number of health or mana potions.
  // Equipment may also drop on rare occasions. The item system will manage the
  // items, their drop rates, and whether the player has collected them.
  return true;
}

bool Room::openTreasure(std::ostream& out) {
  if (type != RoomType::Treasure) {
    out << "There is no treasure chest here.\n";
    return false;
  }
  if (cleared) {
    out << "Treasure already opened.\n";
    return false;
  }

  cleared = true;
  out << "Room #" << roomID << " cleared. Treasure chest opened.\n";

  // TODO: On the first opening, use the same drop rules as a cleared encounter.
  // On later visits, read the chest state so its rewards are rolled only once.
  return true;
}
