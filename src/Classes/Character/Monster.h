#ifndef MONSTER_H_
#define MONSTER_H_

#include "Character.h"

enum class MonsterKind { Goblin, Wisp, Dragon };
enum class AttackStyle { Physical, Magic };

// Each kind uses Character's attacks with its own starting stats and counterattack style.
class Monster : public Character {
public:
    explicit Monster(MonsterKind kind);
    AttackStyle attackStyle() const;

private:
    static StartingStats startingStats(MonsterKind kind);
    MonsterKind kind;
};

#endif
