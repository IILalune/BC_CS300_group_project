/** @file Character */

#include "Character.h"

//Contructor
Character::Character(std::string name) {
    // TODO..
    
    // Character Stats
    characterName = name; 
    this->minPhysicalAttack = 40;
    this->maxPhysicalAttack = 80;    
    this->criticalScalePoint = 2;
    // Character default associated object
}

int Character::physicalAttack() {
    const int* result = dice.rollDice();
    int damageDice = result[0];
    int attackRange = (maxPhysicalAttack - minPhysicalAttack) * (damageDice - 1) / 5;  
    int attackOutput =  minPhysicalAttack + attackRange;
    if (result[2] == 1) {
        attackOutput *= criticalScalePoint;
    }
    return attackOutput;
}






