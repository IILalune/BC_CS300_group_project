#include "Battle.h"

#include "../Character/Monster.h"
#include "../Navigation/Room.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace {
// Read the entire line so an extra word cannot become a second action.
bool readChoice(std::istream& input, std::string& choice) {
  std::string line;
  if (!std::getline(input, line)) return false;
  std::istringstream words(line);
  std::string extra;
  if (!(words >> choice) || words >> extra) choice.clear();
  return true;
}

void strike(Character& attacker, Character& target, AttackStyle style,
            std::ostream& out) {
  const int before = target.getHealth();
  const bool physical = style == AttackStyle::Physical;
  if (physical)
    attacker.physicalAttack(target);
  else
    attacker.magicalAttack(target);
  const int after = target.getHealth();
  out << attacker.getName() << " uses a " << (physical ? "physical" : "magic")
      << " attack on " << target.getName() << " for " << before - after
      << " damage (" << target.getName() << " HP " << before << " -> "
      << after << ").\n";
}

bool drinkPotion(Character& player, std::istream& input, std::ostream& out) {
  const int count = player.getConsumableItemCount();
  if (count == 0) {
    out << "No consumables in your bag.\n";
    return false;
  }

  out << "Consumables:\n";
  for (int slot = 1; slot <= count; ++slot) {
    const BaseItem* item = player.getConsumableItem(slot);
    out << "  " << slot << ". " << item->getItemName()
        << " (heals " << item->getHeal() << " HP)\n";
  }
  out << "Drink which potion? Enter a slot, or b to go back: " << std::flush;
  std::string choice;
  if (!readChoice(input, choice) || choice == "b" || choice == "B" || choice == "0")
    return false;

  std::istringstream number(choice);
  int slot;
  char extra;
  if (!(number >> slot) || number >> extra || slot < 1 || slot > count) {
    out << "Invalid potion slot. Back to battle.\n";
    return false;
  }
  const int before = player.getHealth();
  if (before >= player.getMaxHealth()) {
    out << "HP is full. Potion kept.\n";
    return false;
  }
  const std::string name = player.getConsumableItem(slot)->getItemName();
  if (!player.useConsumableItem(slot)) {
    out << "Cannot use " << name << ".\n";
    return false;
  }
  out << player.getName() << " used " << name << " (HP " << before << " -> "
      << player.getHealth() << ").\n";
  return true;
}

// Attacking or drinking a potion spends the turn. Browsing the bag is free.
bool takePlayerTurn(Character& player, Monster& enemy, std::istream& input,
                    std::ostream& out) {
  std::string choice;
  while (true) {
    out << "Choose [p]hysical, [m]agic, or [i]nventory: " << std::flush;
    if (!readChoice(input, choice)) return false;
    if (choice == "p" || choice == "P" || choice == "1") {
      strike(player, enemy, AttackStyle::Physical, out);
      return true;
    }
    if (choice == "m" || choice == "M" || choice == "2") {
      strike(player, enemy, AttackStyle::Magic, out);
      return true;
    }
    if (choice == "i" || choice == "I") {
      if (drinkPotion(player, input, out)) return true;
      if (!input) return false;
    } else {
      out << "Invalid action. Enter p, m, or i.\n";
    }
  }
}
}  // namespace

BattleResult RunBattle(Character& player, Room& room, std::istream& input,
                       std::ostream& out) {
  const int roomID = room.getRoomID();
  const int total = room.enemyCount();
  if (total == 0) {
    out << "Room #" << roomID << " has no enemies to fight.\n";
    return BattleResult::NoEncounter;
  }

  int round = 1;
  // The room keeps the front monster in its queue until the player defeats it.
  while (room.enemyCount() > 0) {
    auto enemy = room.frontEnemy();
    const int opponent = total - room.enemyCount() + 1;

    while (player.getHealth() > 0 && enemy->getHealth() > 0) {
      out << "Room #" << roomID << ", round " << round << ", opponent "
          << opponent << "/" << total << ": " << enemy->getName() << " ("
          << enemy->getHealth() << " HP). " << player.getName() << " has "
          << player.getHealth() << " HP.\n";
      if (!takePlayerTurn(player, *enemy, input, out)) {
        out << "\nBattle paused in room #" << roomID << ".\n";
        return BattleResult::InputClosed;
      }
      // A surviving monster counters. The next monster waits for a new turn.
      if (enemy->getHealth() > 0) {
        strike(*enemy, player, enemy->attackStyle(), out);
      }
      ++round;
    }

    if (player.getHealth() <= 0) {
      out << "Defeat in room #" << roomID << ". " << player.getName()
          << " has " << player.getHealth() << " HP.\n";
      return BattleResult::Defeat;
    }

    room.removeDefeatedEnemy();
    out << enemy->getName() << " defeated. Enemies remaining: "
        << room.enemyCount() << ".\n";
  }
  return BattleResult::Victory;
}
