/** @file Inventory.cpp */

#include "Inventory.h"

Inventory::Inventory(int inventoryCapacity) : inventory(inventoryCapacity) {
    maxItems = inventoryCapacity;
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

bool Inventory::replaceItem(int position, BaseItem* item) {
    return inventory.setEntry(position,item);
}

int Inventory::getItemCount() const {
    return inventory.getLength();
}

int Inventory::getCapacity() const {
    return maxItems;
}

bool Inventory::isEmpty() const {
    return inventory.isEmpty();
}

bool Inventory::isFull() const {
    return inventory.getLength() >= maxItems;
}
