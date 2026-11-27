
#ifndef MAIN_CHARACTER_H
#define MAIN_CHARACTER_H

#include <string>
#include <vector>

#include "Character Default Item/Dice.h"
#include "Character Default Item/Equipment.h"
#include "Character Default Item/Inventory.h"
#include "BaseItem.h"

class DungeonMap;

class Character {
    public:
        // Constructor
        Character(std::string name);


        // Character Actions and status in Room
        bool move(DungeonMap& dungeon, int RoomId);
        bool physicalAttack(Character& target);
        bool magicalAttack(Character& target);
        void takeDamage(int damageTaken);
        void defend(); 
        bool isAlive();
        // std::vector<int> showStats(); TODO after
        // int castSkill();  TODO after other tests
        
        // Inventory actions and affects:
        bool addItem(BaseItem* item);
        bool equipItem(int inventoryPosition);
        bool unequipItem(EquipmentSlot targetSlot);
        bool removeGearItem(int inventoryPosition);
        bool removeConsumableItem(int inventoryPosition);
        BaseItem* getGearItem(int inventoryPosition) const;
        BaseItem* getConsumableItem(int inventoryPosition) const;
        int getGearItemCount() const;
        int getConsumableItemCount() const;
        // Battle spends the player's turn only when a potion is consumed.
        bool useConsumableItem(int inventoryPosition);
        void restoreHealth(int restoreValue); 
        void restoreMana(int restoreValue); 

        
        // Accessors, value returned with bonus added
        const std::string& getName() const;
        int getLevel() const;
        int getLevelPoint() const;
        int getHealth() const; 
        int getMaxHealth() const;
        int getMinPhysicalAttack() const ;
        int getMaxPhysicalAttack() const;
        int getMana() const;
        int getMaxMana() const;
        int getDefense() const;

    protected:
        struct StartingStats {
            std::string name;
            int health;
            int mana;
            int minPhysicalAttack;
            int maxPhysicalAttack;
            int magicAttack;
        };
        explicit Character(StartingStats stats);
    
    private:
    /** Field*/
        std::string Name;
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
        Inventory gearInventory{10};
        Inventory consumableInventory{5};
        Equipment equipment;
};

#endif
