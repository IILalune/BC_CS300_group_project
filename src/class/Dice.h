/** @file Dice.h */
// Dices class containing one 6 sided dice and 3 sided dice
// Roll the six sided dice for the range of the attack damage
// Roll the second dice, if it match critical damage 2x

#ifndef DICE_H_
#define DICE_H_

#include<vector> 

class Dice {

    // Constructor and Deconstructor
    Dice();
    ~Dice() override;

    Dice(const Dice&) = delete;
    Dice& operator=(const Dice&) = delete;

    std::vector<int> rollDice(); 

    private:
        int sixDice;
        int fourDice;
};

#endif // DICE_H_