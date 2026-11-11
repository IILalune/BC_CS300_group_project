/** @file ArrayList.cpp */

#ifndef REFERENCE_ARRAYLIST_CPP_
#define REFERENCE_ARRAYLIST_CPP_
#include "ArrayList.h"
#include <stdexcept>

template <class ItemType>
ArrayList<ItemType>::ArrayList() {
    this->items = new ItemType[DEFAULT_CAPACITY];
    this->itemCount = 0;
    this->maxItems = DEFAULT_CAPACITY;
}

template <class ItemType>
ArrayList<ItemType>::~ArrayList() {
    delete[] this->items;
}

template <class ItemType>
bool ArrayList<ItemType>::isEmpty() const {
    return this->itemCount == 0;
}

template <class ItemType>
int ArrayList<ItemType>::getLength() const {
    return this->itemCount;
}

template <class ItemType>
bool ArrayList<ItemType>::insert(int position, const ItemType& newEntry) {
    if (position < 1 || position > this->itemCount + 1) {
        return false;
    }
    if (this->itemCount == this->maxItems) {
        this->resize(2 * this->maxItems);
    }
    // Start at the end so unread items are not overwritten.
    for (int index = this->itemCount; index >= position; index--) {
        this->items[index] = this->items[index - 1];
    }
    this->items[position - 1] = newEntry;
    this->itemCount++;
    return true;
}

template <class ItemType>
bool ArrayList<ItemType>::remove(int position) {
    if (position < 1 || position > this->itemCount) {
        return false;
    }
    // Close the gap by shifting later items left.
    for (int index = position; index < this->itemCount; index++) {
        this->items[index - 1] = this->items[index];
    }
    this->itemCount--;
    return true;
}

template <class ItemType>
void ArrayList<ItemType>::clear() {
    this->itemCount = 0;
}

template <class ItemType>
ItemType ArrayList<ItemType>::getEntry(int position) const {
    if (position < 1 || position > this->itemCount) {
        throw std::out_of_range("getEntry(): invalid list position");
    }
    return this->items[position - 1];
}

template <class ItemType>
bool ArrayList<ItemType>::setEntry(int position, const ItemType& newEntry) {
    if (position < 1 || position > this->itemCount) {
        return false;
    }
    this->items[position - 1] = newEntry;
    return true;
}


template <class ItemType>
void ArrayList<ItemType>::resize(int newCapacity) {
    ItemType* newItems = new ItemType[newCapacity];
    try {
        for (int index = 0; index < this->itemCount; index++) {
            newItems[index] = this->items[index];
        }
    } catch (...) {
        delete[] newItems;
        throw;
    }
    delete[] this->items;
    this->items = newItems;
    this->maxItems = newCapacity;
}

#endif
