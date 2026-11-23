/** @file Equipment.h */

#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "../../Items and Skills/ItemType.h"
#include "../../Items and Skills/BaseItem.h"


enum EquipmentSlot { 
    Weapon_Slot = 0,
    Armor_Slot = 1,
    Boots_Slot = 2,
    Charm_Slot = 3,
};

/**
 * @brief Equipmenet class that have 4 slots that store 4 different items for  Character
 */
class Equipment {
    public:
        Equipment();

        bool isSlotEmpty(EquipmentSlot targetSlot) const;
        bool equipItem(BaseItem* item, EquipmentSlot targetSlot);
        BaseItem* unequipItem(EquipmentSlot targetSlot);
        BaseItem* getItem(EquipmentSlot targetSlot) const;
        
        int getPhysicalAttackBonus() const;
        int getMagicAttackBonus() const;
        int getPhysicalDefenseBonus() const;
        int getMagicDefenseBonus() const;
    private:
        BaseItem* equipmentSlots[4];
        bool isValidSlot(EquipmentSlot targetSlot) const;

};

#endif
