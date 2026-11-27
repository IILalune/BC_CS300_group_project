/**
 * 
 */

#include "Classes/Items and Skills/ItemTypes/Weapon.h"
#include "Classes/Character/Character.h"

//TODO: finish useItem()

Weapon::Weapon (int itemId, 
                        const std::string& name,
                        int physicalAttack,
                        int magicAttack,
                        int physicalDefense,
                        int magicDefense,
                        int heal)
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

    bool Weapon::isUsable() const {
        return true;
    }

