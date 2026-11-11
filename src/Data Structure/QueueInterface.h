/** @file QueueInterface.h */

#ifndef QUEUE_INTERFACE_H_
#define QUEUE_INTERFACE_H_

/**
 * @brief Defines the operations available in a first-in, first-out queue.
 * @tparam ItemType The type of items stored in the queue.
 */
template <class ItemType>
class QueueInterface {
public:
    /** @brief Get the number of items currently in the queue. */
    virtual int getCurrentSize() const = 0;

    /** @brief Check whether the queue has no items. */
    virtual bool isEmpty() const = 0;

    /**
     * @brief Get a copy of the item at the front of the queue.
     * @return The item that has been in the queue the longest.
     * @throw std::out_of_range If the queue is empty.
     */
    virtual ItemType peek() const = 0;

    /**
     * @brief Add an item to the back of the queue.
     * @param newItem The item to add.
     */
    virtual void enqueue(const ItemType& newItem) = 0;

    /**
     * @brief Remove the item at the front of the queue.
     * @throw std::out_of_range If the queue is empty.
     */
    virtual void dequeue() = 0;

    /** @brief Remove every item from the queue. */
    virtual void clear() = 0;

    virtual ~QueueInterface() {}
};

#endif  // QUEUE_INTERFACE_H_
