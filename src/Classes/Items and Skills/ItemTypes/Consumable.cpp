/**
 * 
 */

#include "Classes/Items and Skills/ItemTypes/Consumable.h"
#include "Classes/Character/Character.h"

#include <algorithm>

Consumable::Consumable (int itemId, 
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

    ItemType Consumable::getItemType() const {
        return ItemType::Consumable;
    }

    bool Consumable::isUsable() const {
        return true;
    }

std::string Consumable::useItem(Character& character) const {
    const std::string message = character.getName() + " used " + getItemName();
    const int health = character.getHealth();
    const int maxHealth = character.getMaxHealth();
    // restoreHealth adds directly, so the potion enforces the HP cap.
    if (health > 0 && health < maxHealth && getHeal() > 0) {
        character.restoreHealth(std::min(getHeal(), maxHealth - health));
    }
    return message;
}
