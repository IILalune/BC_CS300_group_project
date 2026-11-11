#include <iostream>
#include "Classes/Character/Character.h"

int main() {
    Character mainCharacter("Paul");

    std::cout << "Main character created.\n";

    int damage = mainCharacter.physicalAttack();
    std::cout << "Physical damage: " << damage << '\n';

    return 0;
}