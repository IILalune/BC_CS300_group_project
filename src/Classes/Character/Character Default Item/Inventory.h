/** @file Inventory.h */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "ArrayList.h"
#include "ItemType.h"
#include "BaseItem.h"

/**
 * @brief Stores a chosen number of item references for one character.
 * Inventory does not create or delete items. It only remembers where an item is.
 */
class Inventory {
    public:

        Inventory(int inventoryCapacity);
        bool addItem(BaseItem* item);
        bool removeItem(int position);
        bool replaceItem(int position, BaseItem* item);
        BaseItem* getItem(int position) const;

        int getItemCount() const;
        int getCapacity() const;
        bool isEmpty() const;
        bool isFull() const;

    private:
        int maxItems;
        ArrayList<BaseItem*> inventory;
};



#endif // INVENTORY_H
