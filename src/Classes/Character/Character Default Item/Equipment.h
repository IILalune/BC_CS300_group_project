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
    Equipment_Slot_Count = 4
};

/**
 * @brief Equipmenet class that have 4 slots that store 4 different items for  Character
 */
class Equipment {
    public:
        Equipment();
        
        /** 
         * @brief Check if the equipment slot is empty
         * @param targetSlot The equipment slot that need to be checked
         * @return <True> if the Equipment Slot is empty
         */
        bool isSlotEmpty(EquipmentSlot targetSlot) const;

        /**
         * @brief Equip and Item to the corsponding  equipment slot
         * @param item The Item object to be insert into the equipment slot
         * @param targetSlot The equipment slot to change the Item
         * @return <True> if equiped the Item suceeded.
         */
        bool equipItem(BaseItem* item, EquipmentSlot targetSlot);

        /** 
         * @brief Unequip Item from the equipment slot.
         * @param targetSlot The slot that containing the Item
         * @return <True> if remove the item sucessfully   
         * The removed item will be assigned to an first empty spot found in inventory
         */
        BaseItem* unequipItem(EquipmentSlot targetSlot);
        BaseItem* getItem(EquipmentSlot targetSlot) const;
        
        int getPhysicalAttackBonus() const;
        int getMagicAttackBonus() const;
        int getPhysicalDefenseBonus() const;
        int getMagicDefenseBonus() const;
    private:
        BaseItem* equipmentSlots[Equipment_Slot_Count];
        bool isValidSlot(EquipmentSlot targetSlot) const;

};

#endif
