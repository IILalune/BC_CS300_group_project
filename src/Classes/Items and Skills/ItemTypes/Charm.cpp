/**
 * 
 */

#include "Classes/Items and Skills/ItemTypes/Charm.h"
#include "Classes/Character/Character.h"

//TODO: finish useItem()

Charm::Charm (int itemId, 
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

    ItemType Charm::getItemType() const {
        return ItemType::Charm;
    }

    bool Charm::isEquipable() const {
        return true;
    }
