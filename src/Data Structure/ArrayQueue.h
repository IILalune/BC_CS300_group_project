/** @file ArrayQueue.h */

#ifndef ARRAY_QUEUE_H_
#define ARRAY_QUEUE_H_

#include "QueueInterface.h"
#include "ArrayQueue.cpp"

/**
 * @brief A first-in, first-out queue stored in a resizable circular array.
 * @tparam ItemType The type of items stored in the queue.
 */
template <class ItemType>
class ArrayQueue : public QueueInterface<ItemType> {
public:
    /** @brief Create an empty queue with a starting capacity of six. */
    ArrayQueue();

    /** @brief Free the dynamic array used by the queue. */
    ~ArrayQueue() override;

    ArrayQueue(const ArrayQueue&) = delete;
    ArrayQueue& operator=(const ArrayQueue&) = delete;

    int getCurrentSize() const override;
    bool isEmpty() const override;
    ItemType peek() const override;
    void enqueue(const ItemType& newItem) override;
    void dequeue() override;
    void clear() override;

private:
    static const int DEFAULT_CAPACITY = 6;

    ItemType* items;
    int frontIndex;
    int itemCount;
    int maxItems;

    /**
     * @brief Make a larger array and copy queue items in first-in order.
     * @param newCapacity The capacity of the new array.
     */
    void resize(int newCapacity);
};

#endif  // ARRAY_QUEUE_H_
