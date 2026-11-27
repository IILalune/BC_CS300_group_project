/**
 * 
 */

#ifndef ITEM_ID_
#define ITEM_ID_

#include "Classes/Items and Skills/BaseItem.h"

#include <string>
#include <vector>
#include <memory>

namespace ItemId {
    enum : int {
        IronSword,
        IronShield,
        HealthPotion,

        Count //number of items
    };
}

class ItemRegistry {
    public:
        ItemRegistry(); //
        ItemRegistry(const ItemRegistry&) = delete;
        ItemRegistry& operator=(const ItemRegistry&) = delete;

        /**
         * @param name name of item. currently case and space sensitive
         * @return the item id, or -1 if item id does not exist
         */
        int itemId(const std::string& name) const;

        /**
         * @param name name of item. currently case and space sensitive
         * @return true if item with name exists
         */
        bool hasItem(const std::string& name) const;

        /**
         * @param id item id
         * @return read only reference to item object with id
         * @throw std::out_of_range if id is invalid
         */
        const BaseItem& itemDef(int id) const;

        // Room loot uses these borrowed item definitions with Character::addItem().
        static BaseItem* inventoryItem(int id);

        /**
         * @return total item count in registry
         */
        int itemCount() const;

    private:

        //Storage of Item objects in vector
        std::vector<std::unique_ptr<BaseItem>> itemDefs_;

        /**
         * Item list with all values
         */
        void registerItems();

        /**
         * @param item unique ptr for new item
         * @throw std::logic_error if id position is wrong, or if name is already used 
         */
        void addItem(std::unique_ptr<BaseItem> item);

};

#endif
