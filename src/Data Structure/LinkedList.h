/** @file LinkedList.h */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <memory>
#include <stdexcept>
#include <utility>

#include "ListInterface.h"

/**
 * @brief A linked implementation of the course List ADT.
 *
 * Positions begin at 1, the same as ArrayList.
 */
template <class ItemType>
class LinkedList : public ListInterface<ItemType> {
    private:
        struct Node {
            explicit Node(const ItemType& value) : item(value) {
            }

            ItemType item;
            std::unique_ptr<Node> next;
        };

        std::unique_ptr<Node> head;
        int itemCount;

        Node* nodeAt(int position) const {
            Node* current = head.get();

            for (int index = 1; index < position; index++) {
                current = current->next.get();
            }

            return current;
        }

    public:
        LinkedList() : itemCount(0) {
        }

        ~LinkedList() override = default;
        LinkedList(const LinkedList&) = delete;
        LinkedList& operator=(const LinkedList&) = delete;

        bool isEmpty() const override {
            return itemCount == 0;
        }

        int getLength() const override {
            return itemCount;
        }

        bool insert(int position, const ItemType& newEntry) override {
            if (position < 1 || position > itemCount + 1) {
                return false;
            }

            std::unique_ptr<Node> newNode = std::make_unique<Node>(newEntry);

            if (position == 1) {
                newNode->next = std::move(head);
                head = std::move(newNode);
            } else {
                Node* previous = nodeAt(position - 1);
                newNode->next = std::move(previous->next);
                previous->next = std::move(newNode);
            }

            itemCount++;
            return true;
        }

        bool remove(int position) override {
            if (position < 1 || position > itemCount) {
                return false;
            }

            std::unique_ptr<Node> removedNode;

            if (position == 1) {
                removedNode = std::move(head);
                head = std::move(removedNode->next);
            } else {
                Node* previous = nodeAt(position - 1);
                removedNode = std::move(previous->next);
                previous->next = std::move(removedNode->next);
            }

            itemCount--;
            return true;
        }

        void clear() override {
            head.reset();
            itemCount = 0;
        }

        ItemType getEntry(int position) const override {
            if (position < 1 || position > itemCount) {
                throw std::out_of_range("getEntry(): invalid list position");
            }

            return nodeAt(position)->item;
        }

        bool setEntry(int position, const ItemType& newEntry) override {
            if (position < 1 || position > itemCount) {
                return false;
            }

            nodeAt(position)->item = newEntry;
            return true;
        }
};

#endif // LINKED_LIST_H
