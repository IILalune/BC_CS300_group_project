/** @file Character */

#include "Character.h"

//Contructor  (Testing values, to be balance later)
Character::Character(std::string name) {
    Name = name; 
    Level = 1;
    levelPoint = 0;
    this->minPhysicalAttack = 40;
    this->maxPhysicalAttack = 80;    
    this->criticalScalePoint = 2;
    magicAttack = 25;
    maxHealth = 100;
    Health = maxHealth;
    maxMana = 50;
    Mana = maxMana;
    Defense = 0;
}


// Behavior
bool Character::move(DungeonMap& dungeon, int roomId) {
    return dungeon.MoveToRoom(roomId);
}

bool Character::physicalAttack(Character& target) {
    if (target.getHealth() == 0) { return false;}

    const int* result = dice.rollDice();
    int damageDice = result[0];
    int attackRange = (maxPhysicalAttack - minPhysicalAttack) * (damageDice - 1) / 5;  
    int attackOutput =  minPhysicalAttack + attackRange;
    
    if (result[2] == 1) { attackOutput *= criticalScalePoint;}
    
    target.takeDamage(attackOutput + equipment.getPhysicalAttackBonus());
    return true;
}

bool Character::magicalAttack(Character& target) {
    if (target.Health == 0) { return false;}
    
    target.takeDamage(magicAttack * equipment.getMagicDefenseBonusMultiplier());
    return true;
}

void Character::takeDamage(int damageTaken) {
    Health = Health - damageTaken; 
}


bool Character::isAlive () {
    if (Health == 0) {return false;}
    return true;  
}

// Inventory

bool Character::addItem(BaseItem* item) {
    // Check if item legit
    if (item == nullptr) {return false;}
    // Check item type if consumable to route to consumable tab
    if (item -> getItemType() == ItemType:: Consumable) {
        return consumableInventory.addItem(item);
    }
    return gearInventory.addItem(item);
}

bool Character::removeGearItem(int inventoryPosition) {
    return gearInventory.removeItem(inventoryPosition);
}
bool Character::removeConsumableItem(int inventoryPosition) {
    return consumableInventory.removeItem(inventoryPosition);
}

BaseItem* Character::getGearItem(int inventoryPosition) const {
    return gearInventory.getItem(inventoryPosition);
}
BaseItem* Character::getConsumableItem(int inventoryPosition) const {
    return consumableInventory.getItem(inventoryPosition);
}

int Character::getGearItemCount() const {
    return gearInventory.getItemCount();
}
int Character::getConsumableItemCount() const {
    return consumableInventory.getItemCount();
}

void Character::restoreHealth(int restoreValue) {Health += restoreValue;}
void Character::restoreMana(int restoreValue) {Mana += restoreValue;}


const std::string& Character::getName() const {
    return Name;
}

int Character::getLevel() const {
    return Level;
}

int Character::getLevelPoint() const {
    return levelPoint;
}

int Character::getHealth() const {
    return Health;
}

int Character::getMaxHealth() const {
    return maxHealth;
}

int Character::getMinPhysicalAttack() const {
    return minPhysicalAttack + equipment.getPhysicalAttackBonus();
}

int Character::getMaxPhysicalAttack() const {
    return maxPhysicalAttack + equipment.getPhysicalAttackBonus();
}

int Character::getMana() const {
    return Mana;
}

int Character::getMaxMana() const {
    return maxMana;
}

int Character::getDefense() const {
    return Defense + equipment.getPhysicalDefenseBonus();
}


bool Character::equipItem(int inventoryPosition) {
    if (inventoryPosition < 1 || inventoryPosition > gearInventory.getCapacity()) {
        return false;
    }

    BaseItem* newItem = gearInventory.getItem(inventoryPosition);
    if (!newItem -> isEquipable()) {
        return false;
    }

    // Find appropriate equipment Slot for Item
    EquipmentSlot targetSlot;
    switch (newItem -> getItemType()) {
        case ItemType::Weapon: targetSlot = Weapon_Slot; break;
        case ItemType::Armor: targetSlot = Armor_Slot; break; 
        // TODO, ask Jason to implement 
        // case ItemType:: Boots: targetSlot = Boot_Slot; break;
        // chase ItemType:: Charm: targetSlot = Charm_Slot; break;
        case ItemType::Consumable: return false;
        default: return false;
    }

    // Slot is empty
    if (equipment.isSlotEmpty(targetSlot)) {
        if (!equipment.equipItem(newItem, targetSlot)) {
            return false;
        }
        return gearInventory.removeItem(inventoryPosition);
    }

    // Slot already equiped with an item
    BaseItem* equipedItem = equipment.unequipItem(targetSlot);
    if(!equipment.equipItem(newItem, targetSlot)) {
        equipment.equipItem(equipedItem, targetSlot);
        return false;
    }
    return gearInventory.replaceItem(inventoryPosition, equipedItem);
}

bool Character::unequipItem(EquipmentSlot targetSlot) {
    // inventory full
    if (gearInventory.isFull()) {return false;}
        
    BaseItem* equipedItem = equipment.unequipItem(targetSlot);
    if (equipItem == nullptr) {return false;}
    return gearInventory.addItem(equipedItem);

}

