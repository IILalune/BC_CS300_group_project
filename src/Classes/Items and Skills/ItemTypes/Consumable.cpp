/**
 * 
 */

#include "Classes/Items and Skills/ItemTypes/Consumable.h"
#include "Classes/Character/Character.h"

//TODO: finish useItem()

Consumable::Consumable (int itemId, 
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

    ItemType Consumable::getItemType() const {
        return ItemType::Consumable;
    }

    bool Consumable::isUsable() const {
        return true;
    }

    /**
     * IMPLEMENT WHEN Character.h and cpp ARE FINISHED
     * currently only heal will be implemented, add more if needed
     */
    std::string Consumable::useItem(Character& character) const {
        

        return character.getName() + " used " + getItemName();
    }
