/** @file Dice.cpp */

#include "Dice.h"
#include<random>


Dice::Dice() {
    sixDice = 0;
    threeDice = 0;
}


bool Dice::isCritical() {
    if (result[2] == 0) {
        return false;
    } else if (result[2] == 1) {
        return true;
    }
}

const int*  Dice::rollDice() {
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());

    std::uniform_int_distribution<int> rollSixSide(1, 6);
    std::uniform_int_distribution<int> rollThreeSide(1, 3);

    int damageDice = rollSixSide(generator);
    int criticalDice = rollThreeSide(generator);

    int isCritical = 0;
    if (damageDice == criticalDice) {
        isCritical = 1;
    }

    result[0] = damageDice;
    result[1] = criticalDice;
    result[2] = isCritical;

    return result;
}

