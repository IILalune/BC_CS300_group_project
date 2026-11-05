/** @file ArrayQueueTest.cpp */

#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

#include "ArrayQueue.h"

TEST(ArrayQueueTest, NewQueueIsEmpty) {
    ArrayQueue<std::string> queue;

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.getCurrentSize(), 0);
}

TEST(ArrayQueueTest, EnqueueAndDequeueUseFirstInFirstOutOrder) {
    ArrayQueue<int> queue;

    for (int number = 1; number <= 3; number++) {
        queue.enqueue(number);
    }

    EXPECT_EQ(queue.getCurrentSize(), 3);

    for (int expected = 1; expected <= 3; expected++) {
        EXPECT_EQ(queue.peek(), expected);
        queue.dequeue();
    }

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.getCurrentSize(), 0);
}

TEST(ArrayQueueTest, PeekDoesNotRemoveTheFrontItem) {
    ArrayQueue<std::string> queue;
    queue.enqueue("AA10");
    queue.enqueue("DL22");

    EXPECT_EQ(queue.peek(), "AA10");
    EXPECT_EQ(queue.peek(), "AA10");
    EXPECT_EQ(queue.getCurrentSize(), 2);
}

TEST(ArrayQueueTest, PeekOnEmptyQueueThrows) {
    ArrayQueue<int> queue;

    EXPECT_THROW(queue.peek(), std::out_of_range);
}

TEST(ArrayQueueTest, DequeueOnEmptyQueueThrows) {
    ArrayQueue<int> queue;

    EXPECT_THROW(queue.dequeue(), std::out_of_range);
}

TEST(ArrayQueueTest, ClearRemovesAllItems) {
    ArrayQueue<int> queue;
    queue.enqueue(10);
    queue.enqueue(20);

    queue.clear();

    EXPECT_TRUE(queue.isEmpty());
    EXPECT_EQ(queue.getCurrentSize(), 0);

    queue.enqueue(30);
    EXPECT_EQ(queue.peek(), 30);
}

TEST(ArrayQueueTest, KeepsOrderWhenItWrapsAround) {
    ArrayQueue<int> queue;

    for (int number = 0; number < 5; number++) {
        queue.enqueue(number);
    }

    for (int count = 0; count < 4; count++) {
        queue.dequeue();
    }

    for (int number = 5; number < 9; number++) {
        queue.enqueue(number);
    }

    for (int expected = 4; expected < 9; expected++) {
        EXPECT_EQ(queue.peek(), expected);
        queue.dequeue();
    }

    EXPECT_TRUE(queue.isEmpty());
}

TEST(ArrayQueueTest, GrowsWhileWrapped) {
    ArrayQueue<int> queue;

    for (int number = 0; number < 6; number++) {
        queue.enqueue(number);
    }

    queue.dequeue();
    queue.dequeue();
    queue.enqueue(6);
    queue.enqueue(7);
    queue.enqueue(8);

    for (int expected = 2; expected <= 8; expected++) {
        EXPECT_EQ(queue.peek(), expected);
        queue.dequeue();
    }

    EXPECT_TRUE(queue.isEmpty());
}
