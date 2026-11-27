/**
 * 
 */

#include "Classes/Items and Skills/ItemId.h"
#include "Classes/Items and Skills/ItemTypes/Weapon.h"
#include "Classes/Items and Skills/ItemTypes/Armor.h"
#include "Classes/Items and Skills/ItemTypes/Consumable.h"

#include <stdexcept>

ItemRegistry::ItemRegistry() {
    registerItems();
}

void ItemRegistry::registerItems() {
    using std::make_unique;
    using namespace ItemId;

    //NOTE IF ADDING NEW ITEM: 
        //YOU MUST ALSO ADD ItemId TO ItemId.h 

  //Template for items, just copy and paste it and fill in for new items:
  //addItem(make_unique<>           (itemId,            "Item Name",        physAttack,     magAttack,      physDef,    magDef,     heal));
  //addItem(make_unique<>           (itemId,            "Item Name",        0,              0,              0,          0,          0));
    addItem(make_unique<Weapon>     (IronSword,         "Iron Sword",       5,              0,              0,          0,          0));
    addItem(make_unique<Armor>      (IronShield,        "Iron Shield",      0,              0,              5,          0,          0));
    addItem(make_unique<Consumable> (HealthPotion,      "Health Potion",    0,              0,              0,          0,          5));

    if (itemCount() != Count) {
        throw std::logic_error("ItemId list has " + std::to_string(Count) 
                                + " items, but " + std::to_string(itemCount())
                                + " were added");
    }
}

void ItemRegistry::addItem(std::unique_ptr<BaseItem> item) {
    //check for list inconsistency
    if (item->getItemId() != itemCount()) {
        throw std::logic_error("Item '" + item->getItemName() + "' has ID" + std::to_string(item->getItemId())
                               + " but was added to position " + std::to_string(itemCount())
                               + ": fix ItemId list order in ItemId.h");
    }

    //check for duplicate name
    if (itemId(item->getItemName()) != -1) {
        throw std::logic_error("duplicate item name '" + item->getItemName() + "'");
    }

    itemDefs_.push_back(std::move(item)); //add item to registry vector
}

int ItemRegistry::itemId(const std::string& name) const {
    for (const auto& item : itemDefs_) {
        if (item->getItemName() == name) {
            return item->getItemId();
        }
    }
    return -1; 
}

bool ItemRegistry::hasItem(const std::string& name) const {
    return itemId(name) != -1; //itemId() returns -1 when no Item has the name
}

const BaseItem& ItemRegistry::itemDef(int id) const {
    if (id < 0 || id >= itemCount()) {
        throw std::out_of_range("no item with ID: " + std::to_string(id));
    }
    return *itemDefs_[id];
}

BaseItem* ItemRegistry::inventoryItem(int id) {
    // The catalog must outlive every bag that holds a pointer to one of its items.
    static ItemRegistry catalog;
    return catalog.itemDefs_.at(id).get();
}

int ItemRegistry::itemCount() const {
    return static_cast<int>(itemDefs_.size()); //.size() returns an unsigned int which cant be negative, casting to int just in case
}
