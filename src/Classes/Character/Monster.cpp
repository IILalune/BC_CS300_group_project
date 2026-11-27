#include "Monster.h"

#include <stdexcept>

Monster::Monster(MonsterKind kind)
    : Character(startingStats(kind)), kind(kind) {}

Character::StartingStats Monster::startingStats(MonsterKind kind) {
    // Each row gives the name, HP, mana, physical damage range, and magic damage.
    switch (kind) {
        case MonsterKind::Goblin: return {"Goblin", 36, 0, 5, 9, 0};
        case MonsterKind::Wisp:   return {"Wisp", 30, 0, 0, 0, 6};
        case MonsterKind::Dragon: return {"Dragon", 100, 0, 0, 0, 12};
    }
    throw std::invalid_argument("Unknown monster kind");
}

AttackStyle Monster::attackStyle() const {
    return kind == MonsterKind::Goblin ? AttackStyle::Physical : AttackStyle::Magic;
}
