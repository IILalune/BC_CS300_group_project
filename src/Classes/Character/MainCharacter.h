
#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include<string>

#include "Character Default Item/Dice.h"
#include "Character Default Item/Equipment.h"
#include "Character Default Item/Inventory.h"

class Character {
    public:
        // Constructor
        Character(std::string name);


        // Character Actions
        int physicalAttack();
        int magicalAttack();
        bool addItem(BaseItem* item);
        bool equipItem(int inventoryPosition, EquipmentSlot targetSlot);
        bool unequipItem(EquipmentSlot targetSlot);
        
        
        // Accessors
        std::string getName(); 
        int getMinPhysicalAttack();
        int getMaxPhysicalAttack();
        int getMana();
        int getMaxMana();
        int getDefense();
    
    private:
    /** Field*/
        std::string characterName;
        int Level;
        int levelPoint;

        // Character Stats 
        int minPhysicalAttack;
        int maxPhysicalAttack;
        int criticalScalePoint;
        int magicAttack; // flat damage, damage multipler depended on weapon boost?
        int Health;       
        int maxHealth;
        int Mana;
        int maxMana;
        int Defense;    
        
       
        
        // Character default objects:
        Dice dice;
        Inventory inventory;
        Equipment equipment;
       
        

};

#endif
