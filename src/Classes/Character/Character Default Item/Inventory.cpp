/** @file Inventory.cpp */

#include "Inventory.h"

Inventory::Inventory() : inventory(MAX_ITEMS) {
}

bool Inventory::addItem(BaseItem* item) {
    if (item == nullptr || isFull()) {
        return false;
    }

    return inventory.insert(inventory.getLength() + 1, item);
}

bool Inventory::removeItem(int position) {
    return inventory.remove(position);
}

BaseItem* Inventory::getItem(int position) const {
    return inventory.getEntry(position);
}

int Inventory::getItemCount() const {
    return inventory.getLength();
}

bool Inventory::isEmpty() const {
    return inventory.isEmpty();
}

bool Inventory::isFull() const {
    return inventory.getLength() >= MAX_ITEMS;
}
