/**
 * 
 */

#include "Classes/Items and Skills/ItemTypes/Weapon.h"
#include "Classes/Character/Character.h"

//TODO: finish useItem()

Weapon::Weapon (int itemId, 
                        const std::string& name,
                        int physicalAttack = 0,
                        int magicAttack = 0,
                        int physicalDefense = 0,
                        int magicDefense = 0,
                        int heal = 0) 
            :BaseItem  (itemId,
                        name,
                        physicalAttack,
                        magicAttack,
                        physicalDefense,
                        magicDefense,
                        heal) {}

    ItemType Weapon::getItemType() const {
        return ItemType::Weapon;
    }

    bool Weapon::isEquipable() const {
        return true;
    }

