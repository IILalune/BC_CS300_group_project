/** @file Dice.h */

#ifndef DICE_H_
#define DICE_H_

#include<vector> 

class Dice {
    public:
        // Constructor and Deconstructor
        Dice();

        /**
        * @brief Roll the dice
        * @return A vector that containing the result of rolling the dice, critical dice,
        *                   and 1 or 0 to indicate if critical strike is rolled. 
        */
        const int* rollDice(); 

        /**
         * @brief  Check if the roll is critical
         * pre : the Dice should be rolled once, else critical is false by default
         * @return <True> if the dice roll is a critical hit
         */
        bool isCritical();
        
    private:
        int sixDice;
        int threeDice;
        int result[3] = {0, 0, 0};
    };

#endif // DICE_H_