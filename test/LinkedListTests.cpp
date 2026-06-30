#include <gtest/gtest.h>
#include "../include/linkedList.h"
#include <string>

// ================================================================
//  User-defined type to verify template works with non-primitive types.
// ================================================================
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
};

// ================================================================
//  SECTION 1: append()
//  Tests: empty list, single, order, large dataset,
//         double, string, struct
// ================================================================

TEST(LL_AppendInt, AppendToEmptyList) {
    LinkedList<int> list;
    list.append(10);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 10);
}

TEST(LL_AppendInt, AppendSingleNegativeValue) {
    LinkedList<int> list;
    list.append(-99);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), -99);
}

TEST(LL_AppendInt, AppendPreservesOrder) {
    LinkedList<int> list;
    list.append(10);
    list.append(20);
    list.append(30);
    EXPECT_EQ(list.get(0), 10);
    EXPECT_EQ(list.get(1), 20);
    EXPECT_EQ(list.get(2), 30);
}

TEST(LL_AppendInt, AppendMultipleElements) {
    LinkedList<int> list;
    for (int i = 0; i < 10; i++) list.append(i);
    EXPECT_EQ(list.getSize(), 10);
    EXPECT_EQ(list.get(9), 9);
}

TEST(LL_AppendInt, AppendLargeDataset) {
    LinkedList<int> list;
    for (int i = 0; i < 1000; i++) list.append(i);
    EXPECT_EQ(list.getSize(), 1000);
    EXPECT_EQ(list.get(0), 0);
    EXPECT_EQ(list.get(999), 999);
    EXPECT_EQ(list.get(500), 500);
}

TEST(LL_AppendDouble, AppendDoubleValues) {
    LinkedList<double> list;
    list.append(1.1);
    list.append(2.2);
    list.append(3.3);
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_DOUBLE_EQ(list.get(0), 1.1);
    EXPECT_DOUBLE_EQ(list.get(2), 3.3);
}

TEST(LL_AppendDouble, AppendNegativeDoubles) {
    LinkedList<double> list;
    list.append(-3.14);
    list.append(0.0);
    EXPECT_DOUBLE_EQ(list.get(0), -3.14);
    EXPECT_DOUBLE_EQ(list.get(1), 0.0);
}

TEST(LL_AppendString, AppendStringValues) {
    LinkedList<std::string> list;
    list.append("hello");
    list.append("world");
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), "hello");
    EXPECT_EQ(list.get(1), "world");
}

TEST(LL_AppendString, AppendEmptyString) {
    LinkedList<std::string> list;
    list.append("");
    list.append("after");
    EXPECT_EQ(list.get(0), "");
    EXPECT_EQ(list.get(1), "after");
}

TEST(LL_AppendString, AppendManyStrings) {
    LinkedList<std::string> list;
    for (int i = 0; i < 10; i++) list.append(std::to_string(i));
    EXPECT_EQ(list.getSize(), 10);
    EXPECT_EQ(list.get(9), "9");
}

TEST(LL_AppendStruct, AppendCustomStruct) {
    LinkedList<Point> list;
    list.append(Point(1, 2));
    list.append(Point(3, 4));
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), (Point{1, 2}));
    EXPECT_EQ(list.get(1), (Point{3, 4}));
}

TEST(LL_AppendStruct, AppendStructPreservesOrder) {
    LinkedList<Point> list;
    for (int i = 0; i < 5; i++) list.append(Point(i, i * 10));
    EXPECT_EQ(list.get(0), (Point{0, 0}));
    EXPECT_EQ(list.get(4), (Point{4, 40}));
}

// ================================================================
//  SECTION 2: get()
//  Tests: valid first/last index, middle, negative, past-end,
//         empty list, const correctness, double, string, struct
// ================================================================

TEST(LL_GetInt, GetValidFirstIndex) {
    LinkedList<int> list;
    list.append(100);
    EXPECT_EQ(list.get(0), 100);
}

TEST(LL_GetInt, GetValidLastIndex) {
    LinkedList<int> list;
    for (int i = 0; i < 5; i++) list.append(i * 10); // 0,10,20,30,40
    EXPECT_EQ(list.get(4), 40);
}

TEST(LL_GetInt, GetValidMiddleIndex) {
    LinkedList<int> list;
    for (int i = 1; i <= 5; i++) list.append(i); // 1,2,3,4,5
    EXPECT_EQ(list.get(2), 3);
}

TEST(LL_GetInt, GetNegativeIndexThrows) {
    LinkedList<int> list;
    list.append(1);
    EXPECT_THROW(list.get(-1), const char*);
}

TEST(LL_GetInt, GetOutOfBoundsThrows) {
    LinkedList<int> list;
    list.append(1);
    EXPECT_THROW(list.get(1), const char*);   // exactly size
    EXPECT_THROW(list.get(999), const char*); // way out
}

TEST(LL_GetInt, GetOnEmptyListThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.get(0), const char*);
}

// get() returns T by value — mutation does NOT affect the list
TEST(LL_GetInt, GetReturnsByValue) {
    LinkedList<int> list;
    list.append(42);
    int val = list.get(0);
    val = 99;
    EXPECT_EQ(list.get(0), 42); // original unchanged
}

// const list — get() must still work
TEST(LL_GetInt, GetOnConstList) {
    LinkedList<int> list;
    list.append(77);
    const LinkedList<int>& cRef = list;
    EXPECT_EQ(cRef.get(0), 77);
}

TEST(LL_GetDouble, GetDoubleValue) {
    LinkedList<double> list;
    list.append(3.14);
    EXPECT_DOUBLE_EQ(list.get(0), 3.14);
}

TEST(LL_GetDouble, GetDoubleOutOfBoundsThrows) {
    LinkedList<double> list;
    list.append(1.0);
    EXPECT_THROW(list.get(5), const char*);
}

TEST(LL_GetString, GetStringValue) {
    LinkedList<std::string> list;
    list.append("test");
    EXPECT_EQ(list.get(0), "test");
}

TEST(LL_GetString, GetStringEmptyListThrows) {
    LinkedList<std::string> list;
    EXPECT_THROW(list.get(0), const char*);
}

TEST(LL_GetStruct, GetStructValue) {
    LinkedList<Point> list;
    list.append(Point(7, 8));
    EXPECT_EQ(list.get(0), (Point{7, 8}));
}

TEST(LL_GetStruct, GetStructOutOfBoundsThrows) {
    LinkedList<Point> list;
    list.append(Point(1, 1));
    EXPECT_THROW(list.get(5), const char*);
}

// ================================================================
//  SECTION 3: insert()
//  Tests: empty list, beginning (head change), middle, end,
//         negative index, past-end, large dataset,
//         double, string, struct
// ================================================================

TEST(LL_InsertInt, InsertIntoEmptyList) {
    LinkedList<int> list;
    list.insert(0, 42);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), 42);
}

TEST(LL_InsertInt, InsertAtBeginning) {
    LinkedList<int> list;
    for (int i = 1; i <= 3; i++) list.append(i); // 1,2,3
    list.insert(0, 99);                           // 99,1,2,3
    EXPECT_EQ(list.getSize(), 4);
    EXPECT_EQ(list.get(0), 99);
    EXPECT_EQ(list.get(1), 1);
    EXPECT_EQ(list.get(3), 3);
}

TEST(LL_InsertInt, InsertAtMiddle) {
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list.append(4);
    list.insert(2, 3); // 1,2,3,4
    EXPECT_EQ(list.getSize(), 4);
    EXPECT_EQ(list.get(2), 3);
    EXPECT_EQ(list.get(3), 4);
}

TEST(LL_InsertInt, InsertAtEnd) {
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list.insert(list.getSize(), 999); // append via insert
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(2), 999);
}

TEST(LL_InsertInt, InsertNegativeIndexThrows) {
    LinkedList<int> list;
    list.append(1);
    EXPECT_THROW(list.insert(-1, 5), const char*);
}

TEST(LL_InsertInt, InsertPastEndThrows) {
    LinkedList<int> list;
    list.append(1);
    EXPECT_THROW(list.insert(list.getSize() + 1, 5), const char*);
}

TEST(LL_InsertInt, InsertIntoEmptyAtIndex1Throws) {
    LinkedList<int> list;
    EXPECT_THROW(list.insert(1, 10), const char*); // size=0, index=1 invalid
}

TEST(LL_InsertInt, InsertLargeDataset) {
    LinkedList<int> list;
    for (int i = 0; i < 100; i++) list.append(i);
    list.insert(50, -1);
    EXPECT_EQ(list.getSize(), 101);
    EXPECT_EQ(list.get(50), -1);
    EXPECT_EQ(list.get(49), 49);
    EXPECT_EQ(list.get(51), 50);
}

TEST(LL_InsertDouble, InsertDoubleAtMiddle) {
    LinkedList<double> list;
    list.append(1.1);
    list.append(3.3);
    list.insert(1, 2.2);
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_DOUBLE_EQ(list.get(1), 2.2);
    EXPECT_DOUBLE_EQ(list.get(2), 3.3);
}

TEST(LL_InsertString, InsertStringAtBeginning) {
    LinkedList<std::string> list;
    list.append("b");
    list.append("c");
    list.insert(0, "a");
    EXPECT_EQ(list.getSize(), 3);
    EXPECT_EQ(list.get(0), "a");
    EXPECT_EQ(list.get(1), "b");
}

TEST(LL_InsertString, InsertStringAtEnd) {
    LinkedList<std::string> list;
    list.append("x");
    list.insert(list.getSize(), "z");
    EXPECT_EQ(list.get(1), "z");
}

TEST(LL_InsertStruct, InsertStructAtBeginning) {
    LinkedList<Point> list;
    list.append(Point(2, 2));
    list.insert(0, Point(1, 1));
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), (Point{1, 1}));
    EXPECT_EQ(list.get(1), (Point{2, 2}));
}

// ================================================================
//  SECTION 4: remove()
//  Tests: only element, beginning (head change), middle, end,
//         remove-until-empty, negative index, past-end,
//         empty list, large dataset, double, string, struct
// ================================================================

TEST(LL_RemoveInt, RemoveOnlyElement) {
    LinkedList<int> list;
    list.append(42);
    list.remove(0);
    EXPECT_EQ(list.getSize(), 0);
}

TEST(LL_RemoveInt, RemoveFromBeginning) {
    LinkedList<int> list;
    for (int i = 1; i <= 5; i++) list.append(i); // 1,2,3,4,5
    list.remove(0);
    EXPECT_EQ(list.getSize(), 4);
    EXPECT_EQ(list.get(0), 2); // new head
}

TEST(LL_RemoveInt, RemoveFromMiddle) {
    LinkedList<int> list;
    for (int i = 1; i <= 5; i++) list.append(i); // 1,2,3,4,5
    list.remove(2);                               // 1,2,4,5
    EXPECT_EQ(list.getSize(), 4);
    EXPECT_EQ(list.get(2), 4);
}

TEST(LL_RemoveInt, RemoveFromEnd) {
    LinkedList<int> list;
    for (int i = 1; i <= 5; i++) list.append(i);
    list.remove(list.getSize() - 1);
    EXPECT_EQ(list.getSize(), 4);
    EXPECT_EQ(list.get(3), 4);
}

TEST(LL_RemoveInt, RemoveUntilEmpty) {
    LinkedList<int> list;
    for (int i = 0; i < 5; i++) list.append(i);
    for (int i = 0; i < 5; i++) {
        list.remove(0); // always remove head
        EXPECT_EQ(list.getSize(), 4 - i);
    }
}

TEST(LL_RemoveInt, RemoveNegativeIndexThrows) {
    LinkedList<int> list;
    list.append(1);
    EXPECT_THROW(list.remove(-1), const char*);
}

TEST(LL_RemoveInt, RemoveOutOfBoundsThrows) {
    LinkedList<int> list;
    list.append(1);
    EXPECT_THROW(list.remove(list.getSize()), const char*); // exactly size
    EXPECT_THROW(list.remove(999), const char*);
}

TEST(LL_RemoveInt, RemoveOnEmptyListThrows) {
    LinkedList<int> list;
    EXPECT_THROW(list.remove(0), const char*);
}

TEST(LL_RemoveInt, RemoveLargeDataset) {
    LinkedList<int> list;
    for (int i = 0; i < 100; i++) list.append(i);
    list.remove(50);
    EXPECT_EQ(list.getSize(), 99);
    EXPECT_EQ(list.get(49), 49);
    EXPECT_EQ(list.get(50), 51);
}

TEST(LL_RemoveDouble, RemoveDoubleFromMiddle) {
    LinkedList<double> list;
    list.append(1.1);
    list.append(2.2);
    list.append(3.3);
    list.remove(1);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_DOUBLE_EQ(list.get(0), 1.1);
    EXPECT_DOUBLE_EQ(list.get(1), 3.3);
}

TEST(LL_RemoveString, RemoveStringFromBeginning) {
    LinkedList<std::string> list;
    list.append("a");
    list.append("b");
    list.remove(0);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), "b");
}

TEST(LL_RemoveString, RemoveStringFromEnd) {
    LinkedList<std::string> list;
    list.append("x");
    list.append("y");
    list.remove(1);
    EXPECT_EQ(list.getSize(), 1);
    EXPECT_EQ(list.get(0), "x");
}

TEST(LL_RemoveStruct, RemoveStructOnlyElement) {
    LinkedList<Point> list;
    list.append(Point(5, 5));
    list.remove(0);
    EXPECT_EQ(list.getSize(), 0);
}

TEST(LL_RemoveStruct, RemoveStructFromMiddle) {
    LinkedList<Point> list;
    list.append(Point(1, 1));
    list.append(Point(2, 2));
    list.append(Point(3, 3));
    list.remove(1);
    EXPECT_EQ(list.getSize(), 2);
    EXPECT_EQ(list.get(0), (Point{1, 1}));
    EXPECT_EQ(list.get(1), (Point{3, 3}));
}

// ================================================================
//  SECTION 5: getSize()
//  Tests: empty, after appends, after removes, after inserts, mixed
// ================================================================

TEST(GetSizeLL, SizeOfFreshList) {
    LinkedList<int> list;
    EXPECT_EQ(list.getSize(), 0);
}

TEST(GetSizeLL, SizeAfterAppends) {
    LinkedList<int> list;
    for (int i = 0; i < 7; i++) list.append(i);
    EXPECT_EQ(list.getSize(), 7);
}

TEST(GetSizeLL, SizeAfterRemove) {
    LinkedList<int> list;
    list.append(1);
    list.append(2);
    list.remove(0);
    EXPECT_EQ(list.getSize(), 1);
}

TEST(GetSizeLL, SizeAfterInsert) {
    LinkedList<int> list;
    list.append(1);
    list.insert(0, 0);
    EXPECT_EQ(list.getSize(), 2);
}

TEST(GetSizeLL, SizeAfterMixedOperations) {
    LinkedList<int> list;
    for (int i = 0; i < 10; i++) list.append(i); // size 10
    list.remove(0);                               // size 9
    list.insert(0, 99);                           // size 10
    list.remove(list.getSize() - 1);              // size 9
    EXPECT_EQ(list.getSize(), 9);
}

// ================================================================
//  SECTION 6: Copy Constructor
//  Tests: deep copy independence, copy empty list,
//         post-copy append does not affect original, string, struct
// ================================================================

TEST(CopyLL_Int, CopyConstructorDeepCopy) {
    LinkedList<int> list1;
    list1.append(1);
    list1.append(2);
    LinkedList<int> list2(list1);
    EXPECT_EQ(list2.getSize(), 2);
    EXPECT_EQ(list2.get(0), 1);
    EXPECT_EQ(list2.get(1), 2);
}

TEST(CopyLL_Int, CopyIsIndependent) {
    LinkedList<int> list1;
    list1.append(10);
    LinkedList<int> list2(list1);
    // Appending to list2 must NOT affect list1
    list2.append(20);
    EXPECT_EQ(list1.getSize(), 1);
    EXPECT_EQ(list2.getSize(), 2);
}

TEST(CopyLL_Int, CopyEmptyList) {
    LinkedList<int> list1;
    LinkedList<int> list2(list1);
    EXPECT_EQ(list2.getSize(), 0);
}

TEST(CopyLL_String, CopyConstructorStrings) {
    LinkedList<std::string> list1;
    list1.append("hello");
    list1.append("world");
    LinkedList<std::string> list2(list1);
    EXPECT_EQ(list2.getSize(), 2);
    EXPECT_EQ(list2.get(0), "hello");
    EXPECT_EQ(list2.get(1), "world");
}

TEST(CopyLL_Struct, CopyConstructorStruct) {
    LinkedList<Point> list1;
    list1.append(Point(3, 7));
    LinkedList<Point> list2(list1);
    EXPECT_EQ(list2.getSize(), 1);
    EXPECT_EQ(list2.get(0), (Point{3, 7}));
}
