/** @file Character */

#include "MainCharacter.h"

//Contructor
Character::Character(std::string name) {
    characterName = name; 
    Level = 1;
    levelPoint = 0;
    this->minPhysicalAttack = 40;
    this->maxPhysicalAttack = 80;    
    this->criticalScalePoint = 2;
    magicAttack = 20;
    maxHealth = 100;
    Health = maxHealth;
    maxMana = 50;
    Mana = maxMana;
    Defense = 0;
}

int Character::physicalAttack() {
    const int* result = dice.rollDice();
    int damageDice = result[0];
    int attackRange = (maxPhysicalAttack - minPhysicalAttack) * (damageDice - 1) / 5;  
    int attackOutput =  minPhysicalAttack + attackRange;
    if (result[2] == 1) {
        attackOutput *= criticalScalePoint;
    }
    return attackOutput + equipment.getPhysicalAttackBonus();
}

int Character::magicalAttack() {
    return magicAttack + equipment.getMagicAttackBonus();
}

bool Character::addItem(BaseItem* item) {
    return inventory.addItem(item);
}

bool Character::equipItem(int inventoryPosition, EquipmentSlot targetSlot) {
    if (inventoryPosition < 1 || inventoryPosition > inventory.getItemCount()) {
        return false;
    }

    BaseItem* item = inventory.getItem(inventoryPosition);
    if (!item->isEquipable() || !equipment.equipItem(item, targetSlot)) {
        return false;
    }

    return inventory.removeItem(inventoryPosition);
}

bool Character::unequipItem(EquipmentSlot targetSlot) {
    if (inventory.isFull()) {
        return false;
    }

    BaseItem* item = equipment.unequipItem(targetSlot);
    if (item == nullptr) {
        return false;
    }

    return inventory.addItem(item);
}

std::string Character::getName() {
    return characterName;
}

int Character::getMinPhysicalAttack() {
    return minPhysicalAttack;
}

int Character::getMaxPhysicalAttack() {
    return maxPhysicalAttack;
}

int Character::getMana() {
    return Mana;
}

int Character::getMaxMana() {
    return maxMana;
}

int Character::getDefense() {
    return Defense + equipment.getPhysicalDefenseBonus();
}





