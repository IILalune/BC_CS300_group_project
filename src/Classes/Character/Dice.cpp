/** @file Dice.cpp */

#include "Dice.h"
#include<random>


Dice::Dice() {
    sixDice = 6;
    threeDice = 3;
}

const int*  Dice::rollDice() {
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());

    std::uniform_int_distribution<int> rollSixSide(1, sixDice);
    std::uniform_int_distribution<int> rollThreeSide(1, threeDice);

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

