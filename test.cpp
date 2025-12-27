#include "pch.h"
#include "Array.h"
#include <string>

TEST(ArrayTest, DefaultConstructor) {
    Array<int> a;
    EXPECT_EQ(a.size(), 0);
}

TEST(ArrayTest, CapacityConstructor) {
    Array<int> a(16);
    EXPECT_EQ(a.size(), 0);
}

TEST(ArrayTest, InsertAtEnd) {
    Array<int> a;
    for (int i = 0; i < 10; ++i) {
        int idx = a.insert(i + 1);
        EXPECT_EQ(idx, i);
    }
    EXPECT_EQ(a.size(), 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(a[i], i + 1);
    }
}

TEST(ArrayTest, InsertAtIndex) {
    Array<int> a;
    a.insert(1);
    a.insert(3);
    a.insert(1, 2);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 3);
}

TEST(ArrayTest, InsertAtBeginning) {
    Array<int> a;
    a.insert(3);
    a.insert(0, 2);
    a.insert(0, 1);
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
    EXPECT_EQ(a[2], 3);
}

TEST(ArrayTest, Remove) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i);
    }
    a.remove(2);
    EXPECT_EQ(a.size(), 4);
    EXPECT_EQ(a[0], 0);
    EXPECT_EQ(a[1], 1);
    EXPECT_EQ(a[2], 3);
    EXPECT_EQ(a[3], 4);
}

TEST(ArrayTest, RemoveFirst) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    a.remove(0);
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(a[1], 3);
}

TEST(ArrayTest, RemoveLast) {
    Array<int> a;
    a.insert(1);
    a.insert(2);
    a.insert(3);
    a.remove(2);
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], 1);
    EXPECT_EQ(a[1], 2);
}

TEST(ArrayTest, IndexOperator) {
    Array<int> a;
    a.insert(10);
    a.insert(20);
    EXPECT_EQ(a[0], 10);
    EXPECT_EQ(a[1], 20);
    a[0] = 100;
    a[1] = 200;
    EXPECT_EQ(a[0], 100);
    EXPECT_EQ(a[1], 200);
}

TEST(ArrayTest, ConstIndexOperator) {
    Array<int> a;
    a.insert(10);
    a.insert(20);
    const Array<int>& ca = a;
    EXPECT_EQ(ca[0], 10);
    EXPECT_EQ(ca[1], 20);
}

TEST(ArrayTest, Iterator) {
    Array<int> a;
    for (int i = 1; i <= 5; ++i) {
        a.insert(i);
    }
    int expected = 1;
    for (auto it = a.iterator(); ; it.next()) {
        EXPECT_EQ(it.get(), expected);
        ++expected;
        if (!it.hasNext()) break;
    }
    EXPECT_EQ(expected, 6);
}

TEST(ArrayTest, IteratorSet) {
    Array<int> a;
    for (int i = 1; i <= 3; ++i) {
        a.insert(i);
    }
    for (auto it = a.iterator(); ; it.next()) {
        it.set(it.get() * 2);
        if (!it.hasNext()) break;
    }
    EXPECT_EQ(a[0], 2);
    EXPECT_EQ(a[1], 4);
    EXPECT_EQ(a[2], 6);
}

TEST(ArrayTest, ConstIterator) {
    Array<int> a;
    for (int i = 1; i <= 5; ++i) {
        a.insert(i);
    }
    const Array<int>& ca = a;
    int expected = 1;
    for (auto it = ca.iterator(); ; it.next()) {
        EXPECT_EQ(it.get(), expected);
        ++expected;
        if (!it.hasNext()) break;
    }
    EXPECT_EQ(expected, 6);
}

TEST(ArrayTest, ReverseIterator) {
    Array<int> a;
    for (int i = 1; i <= 5; ++i) {
        a.insert(i);
    }
    int expected = 5;
    for (auto it = a.reverseIterator(); ; it.next()) {
        EXPECT_EQ(it.get(), expected);
        --expected;
        if (!it.hasNext()) break;
    }
    EXPECT_EQ(expected, 0);
}

TEST(ArrayTest, ConstReverseIterator) {
    Array<int> a;
    for (int i = 1; i <= 5; ++i) {
        a.insert(i);
    }
    const Array<int>& ca = a;
    int expected = 5;
    for (auto it = ca.reverseIterator(); ; it.next()) {
        EXPECT_EQ(it.get(), expected);
        --expected;
        if (!it.hasNext()) break;
    }
    EXPECT_EQ(expected, 0);
}

TEST(ArrayTest, CopyConstructor) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i);
    }
    Array<int> b(a);
    EXPECT_EQ(b.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(b[i], i);
    }
    a[0] = 100;
    EXPECT_EQ(b[0], 0);
}

TEST(ArrayTest, CopyAssignment) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i);
    }
    Array<int> b;
    b.insert(999);
    b = a;
    EXPECT_EQ(b.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(b[i], i);
    }
    a[0] = 100;
    EXPECT_EQ(b[0], 0);
}

TEST(ArrayTest, MoveConstructor) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i);
    }
    Array<int> b(std::move(a));
    EXPECT_EQ(b.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(b[i], i);
    }
    EXPECT_EQ(a.size(), 0);
}

TEST(ArrayTest, MoveAssignment) {
    Array<int> a;
    for (int i = 0; i < 5; ++i) {
        a.insert(i);
    }
    Array<int> b;
    b.insert(999);
    b = std::move(a);
    EXPECT_EQ(b.size(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(b[i], i);
    }
    EXPECT_EQ(a.size(), 0);
}

TEST(ArrayTest, CapacityGrowth) {
    Array<int> a(2);
    for (int i = 0; i < 100; ++i) {
        a.insert(i);
    }
    EXPECT_EQ(a.size(), 100);
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(a[i], i);
    }
}

TEST(ArrayTest, StringType) {
    Array<std::string> a;
    a.insert("hello");
    a.insert("world");
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], "hello");
    EXPECT_EQ(a[1], "world");
}

TEST(ArrayTest, StringInsertAtIndex) {
    Array<std::string> a;
    a.insert("first");
    a.insert("third");
    a.insert(1, "second");
    EXPECT_EQ(a.size(), 3);
    EXPECT_EQ(a[0], "first");
    EXPECT_EQ(a[1], "second");
    EXPECT_EQ(a[2], "third");
}

TEST(ArrayTest, StringRemove) {
    Array<std::string> a;
    a.insert("one");
    a.insert("two");
    a.insert("three");
    a.remove(1);
    EXPECT_EQ(a.size(), 2);
    EXPECT_EQ(a[0], "one");
    EXPECT_EQ(a[1], "three");
}

TEST(ArrayTest, StringCopy) {
    Array<std::string> a;
    a.insert("hello");
    a.insert("world");
    Array<std::string> b(a);
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], "hello");
    EXPECT_EQ(b[1], "world");
    a[0] = "changed";
    EXPECT_EQ(b[0], "hello");
}

TEST(ArrayTest, StringCopyAssignment) {
    Array<std::string> a;
    a.insert("hello");
    a.insert("world");
    Array<std::string> b;
    b.insert("temp");
    b = a;
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], "hello");
    EXPECT_EQ(b[1], "world");
    a[0] = "changed";
    EXPECT_EQ(b[0], "hello");
}

TEST(ArrayTest, StringMoveConstructor) {
    Array<std::string> a;
    a.insert("hello");
    a.insert("world");
    Array<std::string> b(std::move(a));
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], "hello");
    EXPECT_EQ(b[1], "world");
    EXPECT_EQ(a.size(), 0);
}

TEST(ArrayTest, StringMoveAssignment) {
    Array<std::string> a;
    a.insert("hello");
    a.insert("world");
    Array<std::string> b;
    b.insert("temp");
    b = std::move(a);
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b[0], "hello");
    EXPECT_EQ(b[1], "world");
    EXPECT_EQ(a.size(), 0);
}

TEST(ArrayTest, StringCapacityGrowth) {
    Array<std::string> a(2);
    for (int i = 0; i < 50; ++i) {
        a.insert("string" + std::to_string(i));
    }
    EXPECT_EQ(a.size(), 50);
    for (int i = 0; i < 50; ++i) {
        EXPECT_EQ(a[i], "string" + std::to_string(i));
    }
}

TEST(ArrayTest, StringIterator) {
    Array<std::string> a;
    a.insert("one");
    a.insert("two");
    a.insert("three");
    int count = 0;
    for (auto it = a.iterator(); ; it.next()) {
        ++count;
        if (!it.hasNext()) break;
    }
    EXPECT_EQ(count, 3);
}

TEST(ArrayTest, StringReverseIterator) {
    Array<std::string> a;
    a.insert("one");
    a.insert("two");
    a.insert("three");
    auto it = a.reverseIterator();
    EXPECT_EQ(it.get(), "three");
    it.next();
    EXPECT_EQ(it.get(), "two");
    it.next();
    EXPECT_EQ(it.get(), "one");
    EXPECT_FALSE(it.hasNext());
}

TEST(ArrayTest, EmptyIterator) {
    Array<int> a;
    auto it = a.iterator();
    EXPECT_FALSE(it.hasNext());
}

TEST(ArrayTest, SingleElementIterator) {
    Array<int> a;
    a.insert(42);
    auto it = a.iterator();
    EXPECT_EQ(it.get(), 42);
    EXPECT_FALSE(it.hasNext());
}

TEST(ArrayTest, ExampleFromTask) {
    Array<int> a;
    for (int i = 0; i < 10; ++i) {
        a.insert(i + 1);
    }
    for (int i = 0; i < a.size(); ++i) {
        a[i] *= 2;
    }
    int expected = 2;
    for (auto it = a.iterator(); ; it.next()) {
        EXPECT_EQ(it.get(), expected);
        expected += 2;
        if (!it.hasNext()) break;
    }
    EXPECT_EQ(expected, 22);
}
