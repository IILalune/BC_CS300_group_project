/** @file Inventory.h */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "../../../Data Structure/ArrayList.h"
#include "../../Items and Skills/ItemType.h"
#include "../../Items and Skills/BaseItem.h"

/**
 * @brief Stores up to ten item references for one character.
 *
 * Inventory does not create or delete items. It only remembers where an item is.
 */
class Inventory {
    public:
        static const int MAX_ITEMS = 10;

        Inventory();

        bool addItem(BaseItem* item);
        bool removeItem(int position);
        BaseItem* getItem(int position) const;

        int getItemCount() const;
        bool isEmpty() const;
        bool isFull() const;

    private:
        ArrayList<BaseItem*> inventory;
};



#endif // INVENTORY_H
