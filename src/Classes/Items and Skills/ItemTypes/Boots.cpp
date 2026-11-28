/**
 * 
 */

#include "Classes/Items and Skills/ItemTypes/Boots.h"
#include "Classes/Character/Character.h"

//TODO: finish useItem()

Boots::Boots (int itemId, 
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

    ItemType Boots::getItemType() const {
        return ItemType::Boots;
    }

    bool Boots::isEquipable() const {
        return true;
    }
