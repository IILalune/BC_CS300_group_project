/** @file ArrayQueue.cpp */

#ifndef ARRAY_QUEUE_CPP_
#define ARRAY_QUEUE_CPP_

#include "ArrayQueue.h"
#include <stdexcept>

template <class ItemType>
ArrayQueue<ItemType>::ArrayQueue() {
    items = new ItemType[DEFAULT_CAPACITY];
    frontIndex = 0;
    itemCount = 0;
    maxItems = DEFAULT_CAPACITY;
}

template <class ItemType>
ArrayQueue<ItemType>::~ArrayQueue() {
    delete[] items;
}

template <class ItemType>
int ArrayQueue<ItemType>::getCurrentSize() const {
    return itemCount;
}

template <class ItemType>
bool ArrayQueue<ItemType>::isEmpty() const {
    return itemCount == 0;
}

template <class ItemType>
ItemType ArrayQueue<ItemType>::peek() const {
    if (isEmpty()) {
        throw std::out_of_range("ArrayQueue::peek(): the queue is empty");
    }

    return items[frontIndex];
}

template <class ItemType>
void ArrayQueue<ItemType>::enqueue(const ItemType& newItem) {
    if (itemCount == maxItems) {
        resize(2 * maxItems);
    }

    int backIndex = (frontIndex + itemCount) % maxItems;
    items[backIndex] = newItem;
    itemCount++;
}

template <class ItemType>
void ArrayQueue<ItemType>::dequeue() {
    if (isEmpty()) {
        throw std::out_of_range("ArrayQueue::dequeue(): the queue is empty");
    }

    frontIndex = (frontIndex + 1) % maxItems;
    itemCount--;
}

template <class ItemType>
void ArrayQueue<ItemType>::clear() {
    frontIndex = 0;
    itemCount = 0;
}

template <class ItemType>
void ArrayQueue<ItemType>::resize(int newCapacity) {
    ItemType* newItems = new ItemType[newCapacity];

    for (int index = 0; index < itemCount; index++) {
        newItems[index] = items[(frontIndex + index) % maxItems];
    }

    delete[] items;
    items = newItems;
    frontIndex = 0;
    maxItems = newCapacity;
}

#endif  // ARRAY_QUEUE_CPP_
