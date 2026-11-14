/**
 * 
 */

#ifndef CONSUMABLE_
#define CONSUMABLE_

#include "Classes/Items and Skills/BaseItem.h"

/**
 * Could manipulate the extra values to provide buffs to the character using them.
 */
class Consumable : public BaseItem {
    public:
        Consumable (int itemId, 
                    const std::string& name,
                    int physicalAttack = 0,
                    int magicAttack = 0,
                    int physicalDefense = 0,
                    int magicDefense = 0,
                    int heal = 0);

        //More on these in BaseItem.h
        ItemType getItemType() const override;
        bool isUsable() const override;

        //will heal the character using it
        std::string useItem(Character& character) const override;
};

#endif