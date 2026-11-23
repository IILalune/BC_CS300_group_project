/**
 * 
 */

#include "Classes/Items and Skills/ItemTypes/Armor.h"
#include "Classes/Character/Character.h"

//TODO: finish useItem()

Armor::Armor (int itemId, 
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

    ItemType Armor::getItemType() const {
        return ItemType::Armor;
    }

    bool Armor::isUsable() const {
        return true;
    }