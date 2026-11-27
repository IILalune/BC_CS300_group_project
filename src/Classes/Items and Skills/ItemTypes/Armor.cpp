/**
 * 
 */

#include "Classes/Items and Skills/ItemTypes/Armor.h"
#include "Classes/Character/Character.h"

//TODO: finish useItem()

Armor::Armor (int itemId, 
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

    ItemType Armor::getItemType() const {
        return ItemType::Armor;
    }

    bool Armor::isUsable() const {
        return true;
    }
