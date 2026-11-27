#include "Room.h"

#include <ostream>

#include "../Character/Monster.h"
#include "../Items and Skills/ItemId.h"

Room::Room(int id, RoomType kind, const std::string& text)
    : roomID(id),
      type(kind),
      description(text),
      cleared(kind == RoomType::Start) {}

bool Room::isCombatRoom() const {
  return type == RoomType::Normal || type == RoomType::Boss;
}

bool Room::canLeave() const {
  return !isCombatRoom() || cleared;
}

int Room::enemyCount() const {
  return enemies.getCurrentSize();
}

std::shared_ptr<Monster> Room::frontEnemy() const {
  if (enemies.isEmpty()) return nullptr;
  return enemies.peek();
}

bool Room::removeDefeatedEnemy() {
  auto front = frontEnemy();
  if (!front || front->getHealth() > 0) return false;
  enemies.dequeue();
  return true;
}

void Room::prepareEncounter(std::mt19937& rng, std::ostream& out) {
  if (!isCombatRoom() || encounterPrepared) return;

  // Create the whole group before adding anyone to the room.
  const int maxEnemies = 3;
  std::shared_ptr<Monster> group[maxEnemies];
  std::string order;
  int count = 1;
  if (type == RoomType::Normal) {
    std::uniform_int_distribution<int> rollCount(1, maxEnemies);
    count = rollCount(rng);
  }
  std::bernoulli_distribution pickGoblin(0.5);
  for (int i = 0; i < count; ++i) {
    MonsterKind kind = MonsterKind::Dragon;
    if (type == RoomType::Normal) {
      kind = pickGoblin(rng) ? MonsterKind::Goblin : MonsterKind::Wisp;
    }
    group[i] = std::make_shared<Monster>(kind);
    if (i != 0) order += " -> ";
    order += group[i]->getName();
  }
  for (int i = 0; i < count; ++i) {
    enemies.enqueue(group[i]);
  }
  encounterPrepared = true;
  out << "Encounter in room #" << roomID << " (in order): " << order << '\n';
}

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

bool Room::clearEncounter(std::mt19937& lootRng, std::ostream& out) {
  if (!isCombatRoom()) {
    out << "There is no combat encounter here.\n";
    return false;
  }
  if (cleared) {
    out << "Room already cleared.\n";
    return false;
  }

  if (!encounterPrepared || !enemies.isEmpty()) {
    out << "Room #" << roomID << " still has enemies.\n";
    return false;
  }

  finishRoom(lootRng, out);
  return true;
}

bool Room::openTreasure(std::mt19937& lootRng, std::ostream& out) {
  if (type != RoomType::Treasure) {
    out << "There is no treasure chest here.\n";
    return false;
  }
  if (cleared) {
    out << "Treasure already opened.\n";
    return false;
  }

  finishRoom(lootRng, out);
  return true;
}

void Room::finishRoom(std::mt19937& lootRng, std::ostream& out) {
  // Both callers check cleared first, so returning to a room keeps the original drops.
  int potionChance = 20;
  int gearChance = 5;
  if (type == RoomType::Boss) {
    potionChance = 100;
    gearChance = 20;
  } else if (type == RoomType::Treasure) {
    potionChance = 100;
    gearChance = 100;
  }

  // One independent roll per category. Reserve before adding either drop.
  pendingLoot.reserve(2);
  std::uniform_int_distribution<int> percent(1, 100);
  if (potionChance == 100 || percent(lootRng) <= potionChance) {
    pendingLoot.push_back(ItemId::HealthPotion);
  }
  if (gearChance == 100 || percent(lootRng) <= gearChance) {
    std::uniform_int_distribution<int> pickGear(0, 1);
    pendingLoot.push_back(pickGear(lootRng) == 0 ? ItemId::IronSword : ItemId::IronShield);
  }
  cleared = true;
  out << "Room #" << roomID << " cleared. ";
  if (type == RoomType::Treasure) out << "Treasure chest opened.\n";
  else out << "All enemies defeated.\n";
  if (pendingLoot.empty()) out << "Room #" << roomID << " dropped no loot.\n";
  for (int id : pendingLoot) {
    out << "Room #" << roomID << " dropped: " << ItemRegistry::inventoryItem(id)->getItemName() << ".\n";
  }
}

void Room::collectLoot(Character& player, std::ostream& out) {
  // The registry owns each definition. Character::addItem stores its pointer in the matching bag.
  for (auto item = pendingLoot.begin(); item != pendingLoot.end(); ) {
    BaseItem* definition = ItemRegistry::inventoryItem(*item);
    const auto& name = definition->getItemName();
    const char* bag = definition->getItemType() == ItemType::Consumable ? "consumable" : "gear";
    if (player.addItem(definition)) {
      item = pendingLoot.erase(item);
      out << "Room #" << roomID << ": " << name << " added to " << bag << " bag.\n";
    } else {
      ++item;
      out << "Room #" << roomID << ": " << bag << " bag full. " << name << " left in room.\n";
    }
  }
}
