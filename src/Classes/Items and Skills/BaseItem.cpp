/**
 * 
 */

#include "Classes/Items and Skills/BaseItem.h"

BaseItem::BaseItem(int itemId, 
                   const std::string& name,
                   int physicalAttack,
                   int magicAttack,
                   int physicalDefense,
                   int magicDefense,
                   int heal)
                :  itemId(itemId), 
                   name(name),
                   physicalAttack(physicalAttack),
                   magicAttack(magicAttack),
                   physicalDefense(physicalDefense),
                   magicDefense(magicDefense),
                   heal(heal) {}


    //getters
    
    int BaseItem::getItemId() const {
        return itemId;
    }

    const std::string& BaseItem::getItemName() const {
        return name;
    }

    int BaseItem::getPhysicalAttack() const {
        return physicalAttack;
    }

    int BaseItem::getMagicAttack() const {
        return magicAttack;
    }

    int BaseItem::getPhysicalDefense() const {
        return physicalDefense;
    }

    int BaseItem::getMagicDefense() const {
        return magicDefense;
    }

    int BaseItem::getHeal() const {
        return heal;
    }


    //methods

    bool BaseItem::isEquipable() const {
        return false;
    }

    bool BaseItem::isUsable() const {
        return false;
    }

    std::string BaseItem::useItem(Character& user) const {
        return "can't use " + name;
    }

    std::string BaseItem::itemDescription() const {
        return name + itemDetails() + ".";
    }

    std::string BaseItem::itemDetails() const {
        std::string details;

        if (physicalAttack != 0) {
            details += ". physical attack is " + std::to_string(physicalAttack);
        }
        if (magicAttack != 0) {
            details += ". magic attack is " + std::to_string(magicAttack);
        }
        if (physicalDefense != 0) {
            details += ". physical defense is " + std::to_string(physicalDefense);
        }
        if (magicDefense != 0) {
            details += ". magic defense is " + std::to_string(magicDefense);
        }
        if (heal != 0) {
            details += ". heal is " + std::to_string(heal);
        }

        return details;
    }
