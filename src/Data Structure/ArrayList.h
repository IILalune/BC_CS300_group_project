
/** @file ArrayList.h */
#ifndef REFERENCE_ARRAYLIST_H_
#define REFERENCE_ARRAYLIST_H_

#include "ListInterface.h"
#include "ArrayList.cpp"

template <class ItemType>
class ArrayList : public ListInterface<ItemType> {
    public:
        ArrayList();
        ~ArrayList() override;
        ArrayList(const ArrayList&) = delete;
        ArrayList& operator=(const ArrayList&) = delete;
        bool isEmpty() const override;
        int getLength() const override;
        bool insert(int position, const ItemType& newEntry) override;
        bool remove(int position) override;
        void clear() final;
        ItemType getEntry(int position) const override;
        bool setEntry(int position, const ItemType& newEntry) override;

    private:
        static const int DEFAULT_CAPACITY = 6;
        ItemType* items;
        int itemCount;
        int maxItems;
    void resize(int newCapacity);
};

#endif
