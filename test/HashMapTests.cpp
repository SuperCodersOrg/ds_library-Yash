#include <gtest/gtest.h>
#include "../include/hashMap.h"
#include <string>

// ================================================================
//  Custom struct used as a VALUE type to verify non-primitive support.
//  Must support operator== for EXPECT_EQ comparisons.
// ================================================================
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const { return x == other.x && y == other.y; }
    bool operator!=(const Point& other) const { return !(*this == other); }
};

// ================================================================
//  Custom struct used as a KEY type.
//  HashMap uses hashFunction.generate(key) — works on primitive-sized
//  types. We use int keys for struct-value tests to stay safe.
// ================================================================


// ================================================================
//  SECTION 1: set() and get()
//  Tests: basic set/get, int keys, string keys, double keys,
//         struct values, negative keys
// ================================================================

TEST(SetGet, IntKeyIntValue) {
    HashMap<int, int> map;
    map.set(1, 100);
    EXPECT_EQ(map.get(1), 100);
}

TEST(SetGet, NegativeIntKey) {
    HashMap<int, int> map;
    map.set(-5, 42);
    EXPECT_EQ(map.get(-5), 42);
}

TEST(SetGet, ZeroKey) {
    HashMap<int, int> map;
    map.set(0, 999);
    EXPECT_EQ(map.get(0), 999);
}

TEST(SetGet, StringKeyStringValue) {
    HashMap<std::string, std::string> map;
    map.set("name", "yash");
    EXPECT_EQ(map.get("name"), "yash");
}

TEST(SetGet, StringKeyIntValue) {
    HashMap<std::string, int> map;
    map.set("age", 21);
    EXPECT_EQ(map.get("age"), 21);
}

TEST(SetGet, IntKeyDoubleValue) {
    HashMap<int, double> map;
    map.set(7, 3.14);
    EXPECT_DOUBLE_EQ(map.get(7), 3.14);
}

TEST(SetGet, IntKeyStructValue) {
    HashMap<int, Point> map;
    map.set(1, Point(10, 20));
    EXPECT_EQ(map.get(1), Point(10, 20));
}

TEST(SetGet, StringKeyStructValue) {
    HashMap<std::string, Point> map;
    map.set("origin", Point(0, 0));
    map.set("dest",   Point(5, 7));
    EXPECT_EQ(map.get("origin"), Point(0, 0));
    EXPECT_EQ(map.get("dest"),   Point(5, 7));
}

TEST(SetGet, MultipleKeys) {
    HashMap<int, int> map;
    for (int i = 0; i < 10; i++) map.set(i, i * i);
    for (int i = 0; i < 10; i++) EXPECT_EQ(map.get(i), i * i);
}

TEST(SetGet, EmptyStringKey) {
    HashMap<std::string, int> map;
    map.set("", 0);
    EXPECT_EQ(map.get(""), 0);
}

// ================================================================
//  SECTION 2: Overwrite (set() on existing key)
//  Tests: value must be updated, size must NOT increase
// ================================================================

TEST(Overwrite, IntKeyOverwriteValue) {
    HashMap<int, int> map;
    map.set(1, 10);
    map.set(1, 99);
    EXPECT_EQ(map.get(1), 99);
    EXPECT_EQ(map.getSize(), 1);  // no duplicate key
}

TEST(Overwrite, StringKeyOverwriteValue) {
    HashMap<std::string, std::string> map;
    map.set("city", "delhi");
    map.set("city", "mumbai");
    EXPECT_EQ(map.get("city"), "mumbai");
    EXPECT_EQ(map.getSize(), 1);
}

TEST(Overwrite, StructValueOverwrite) {
    HashMap<int, Point> map;
    map.set(1, Point(0, 0));
    map.set(1, Point(9, 9));
    EXPECT_EQ(map.get(1), Point(9, 9));
    EXPECT_EQ(map.getSize(), 1);
}

TEST(Overwrite, OverwriteDoesNotGrowSize) {
    HashMap<int, int> map;
    map.set(5, 1);
    map.set(5, 2);
    map.set(5, 3);
    EXPECT_EQ(map.getSize(), 1);
}

// ================================================================
//  SECTION 3: exist()
//  Tests: key present, key absent, after remove, after overwrite
// ================================================================

TEST(Exist, KeyPresent) {
    HashMap<int, int> map;
    map.set(42, 1);
    EXPECT_TRUE(map.exist(42));
}

TEST(Exist, KeyAbsent) {
    HashMap<int, int> map;
    EXPECT_FALSE(map.exist(1));
}

TEST(Exist, StringKeyPresent) {
    HashMap<std::string, int> map;
    map.set("hello", 1);
    EXPECT_TRUE(map.exist("hello"));
}

TEST(Exist, StringKeyAbsent) {
    HashMap<std::string, int> map;
    map.set("hello", 1);
    EXPECT_FALSE(map.exist("world"));
}

TEST(Exist, AfterRemove) {
    HashMap<int, int> map;
    map.set(7, 7);
    map.remove(7);
    EXPECT_FALSE(map.exist(7));
}

TEST(Exist, StillExistsAfterOverwrite) {
    HashMap<std::string, std::string> map;
    map.set("key", "v1");
    map.set("key", "v2");
    EXPECT_TRUE(map.exist("key"));
}

// ================================================================
//  SECTION 4: remove()
//  Tests: remove existing, size decrements, throws on missing key
// ================================================================

TEST(Remove, RemoveExistingIntKey) {
    HashMap<int, int> map;
    map.set(1, 10);
    map.remove(1);
    EXPECT_FALSE(map.exist(1));
    EXPECT_EQ(map.getSize(), 0);
}

TEST(Remove, RemoveExistingStringKey) {
    HashMap<std::string, std::string> map;
    map.set("x", "y");
    map.remove("x");
    EXPECT_FALSE(map.exist("x"));
}

TEST(Remove, RemoveDecreasesSize) {
    HashMap<int, int> map;
    map.set(1, 1);
    map.set(2, 2);
    map.set(3, 3);
    map.remove(2);
    EXPECT_EQ(map.getSize(), 2);
    EXPECT_TRUE(map.exist(1));
    EXPECT_TRUE(map.exist(3));
    EXPECT_FALSE(map.exist(2));
}

TEST(Remove, RemoveNonExistentKeyThrows) {
    HashMap<int, int> map;
    EXPECT_THROW(map.remove(99), const char*);
}

TEST(Remove, RemoveOneOfManyKeysInSameBucket) {
    // Keys 0 and 4 likely hash to same bucket with capacity=4
    HashMap<int, int> map;
    map.set(0, 100);
    map.set(4, 400);
    map.remove(0);
    EXPECT_FALSE(map.exist(0));
    EXPECT_TRUE(map.exist(4));
    EXPECT_EQ(map.get(4), 400);
}

// ================================================================
//  SECTION 5: getSize() and empty()
// ================================================================

TEST(SizeAndEmpty, StartsEmpty) {
    HashMap<int, int> map;
    EXPECT_EQ(map.getSize(), 0);
    EXPECT_TRUE(map.empty());
}

TEST(SizeAndEmpty, NotEmptyAfterSet) {
    HashMap<int, int> map;
    map.set(1, 1);
    EXPECT_FALSE(map.empty());
    EXPECT_EQ(map.getSize(), 1);
}

TEST(SizeAndEmpty, SizeAfterMultipleSets) {
    HashMap<std::string, int> map;
    map.set("a", 1);
    map.set("b", 2);
    map.set("c", 3);
    EXPECT_EQ(map.getSize(), 3);
}

TEST(SizeAndEmpty, EmptyAfterClear) {
    HashMap<int, int> map;
    map.set(1, 1);
    map.set(2, 2);
    map.clear();
    EXPECT_TRUE(map.empty());
    EXPECT_EQ(map.getSize(), 0);
}

// ================================================================
//  SECTION 6: clear()
//  Tests: all keys gone, size zero, can re-use after clear
// ================================================================

TEST(Clear, AllKeysGoneAfterClear) {
    HashMap<int, int> map;
    map.set(1, 1);
    map.set(2, 2);
    map.set(3, 3);
    map.clear();
    EXPECT_FALSE(map.exist(1));
    EXPECT_FALSE(map.exist(2));
    EXPECT_FALSE(map.exist(3));
}

TEST(Clear, CanSetAfterClear) {
    HashMap<std::string, std::string> map;
    map.set("k", "v");
    map.clear();
    map.set("k", "new");
    EXPECT_EQ(map.get("k"), "new");
    EXPECT_EQ(map.getSize(), 1);
}

TEST(Clear, ClearOnEmptyMap) {
    HashMap<int, int> map;
    EXPECT_NO_THROW(map.clear());
    EXPECT_EQ(map.getSize(), 0);
}

// ================================================================
//  SECTION 7: Rehashing (load factor > 0.75)
//  Insert enough keys to force a rehash and verify all data intact
// ================================================================

TEST(Rehash, DataIntactAfterRehash) {
    HashMap<int, int> map;
    // Initial capacity = 4, rehash at size > 3
    for (int i = 0; i < 20; i++) map.set(i, i * 10);
    for (int i = 0; i < 20; i++) {
        EXPECT_TRUE(map.exist(i));
        EXPECT_EQ(map.get(i), i * 10);
    }
    EXPECT_EQ(map.getSize(), 20);
}

TEST(Rehash, StringKeysAfterRehash) {
    HashMap<std::string, int> map;
    map.set("alpha",   1);
    map.set("beta",    2);
    map.set("gamma",   3);
    map.set("delta",   4);  // triggers rehash
    map.set("epsilon", 5);
    EXPECT_EQ(map.get("alpha"),   1);
    EXPECT_EQ(map.get("beta"),    2);
    EXPECT_EQ(map.get("gamma"),   3);
    EXPECT_EQ(map.get("delta"),   4);
    EXPECT_EQ(map.get("epsilon"), 5);
}

TEST(Rehash, StructValuesAfterRehash) {
    HashMap<int, Point> map;
    for (int i = 0; i < 15; i++) map.set(i, Point(i, i * 2));
    for (int i = 0; i < 15; i++) EXPECT_EQ(map.get(i), Point(i, i * 2));
}

// ================================================================
//  SECTION 8: get() throws on missing key
// ================================================================

TEST(GetThrows, GetMissingIntKey) {
    HashMap<int, int> map;
    EXPECT_THROW(map.get(42), const char*);
}

TEST(GetThrows, GetMissingStringKey) {
    HashMap<std::string, std::string> map;
    EXPECT_THROW(map.get("missing"), const char*);
}

TEST(GetThrows, GetAfterRemoveThrows) {
    HashMap<int, int> map;
    map.set(1, 1);
    map.remove(1);
    EXPECT_THROW(map.get(1), const char*);
}

// ================================================================
//  SECTION 9: get() returns a reference (mutation via get)
// ================================================================

TEST(GetRef, MutateValueViaGetRef) {
    HashMap<std::string, int> map;
    map.set("score", 0);
    map.get("score") = 100;
    EXPECT_EQ(map.get("score"), 100);
}

TEST(GetRef, MutateStructFieldViaGetRef) {
    HashMap<int, Point> map;
    map.set(1, Point(0, 0));
    map.get(1).x = 99;
    EXPECT_EQ(map.get(1).x, 99);
}

// ================================================================
//  SECTION 10: Copy constructor and assignment operator
// ================================================================

TEST(CopySemantics, CopyConstructorCopiesAllKeys) {
    HashMap<int, int> original;
    original.set(1, 10);
    original.set(2, 20);

    HashMap<int, int> copy(original);
    EXPECT_EQ(copy.get(1), 10);
    EXPECT_EQ(copy.get(2), 20);
    EXPECT_EQ(copy.getSize(), 2);
}

TEST(CopySemantics, CopyIsIndependent) {
    HashMap<int, int> original;
    original.set(1, 10);

    HashMap<int, int> copy(original);
    copy.set(1, 999);  // modify copy

    EXPECT_EQ(original.get(1), 10);  // original must be untouched
    EXPECT_EQ(copy.get(1), 999);
}

TEST(CopySemantics, AssignmentOperator) {
    HashMap<std::string, std::string> a;
    a.set("k", "v");

    HashMap<std::string, std::string> b;
    b = a;
    EXPECT_EQ(b.get("k"), "v");
}

TEST(CopySemantics, SelfAssignment) {
    HashMap<int, int> map;
    map.set(1, 42);
    map = map;  // must not crash or corrupt
    EXPECT_EQ(map.get(1), 42);
}

// ================================================================
//  SECTION 11: char key and bool key
//  (double keys NOT supported — hashFunction has no double overload)
// ================================================================

TEST(CharKey, CharKeyIntValue) {
    HashMap<char, int> map;
    map.set('a', 1);
    map.set('z', 26);
    EXPECT_EQ(map.get('a'), 1);
    EXPECT_EQ(map.get('z'), 26);
}

TEST(CharKey, CharKeyStringValue) {
    HashMap<char, std::string> map;
    map.set('A', "alpha");
    map.set('B', "beta");
    EXPECT_EQ(map.get('A'), "alpha");
    EXPECT_EQ(map.get('B'), "beta");
}

TEST(BoolKey, BoolKeyIntValue) {
    HashMap<bool, int> map;
    map.set(true,  1);
    map.set(false, 0);
    EXPECT_EQ(map.get(true),  1);
    EXPECT_EQ(map.get(false), 0);
}

// ================================================================
//  SECTION 12: Collision handling (same bucket, different keys)
// ================================================================

TEST(Collision, MultipleKeysInSameBucket) {
    HashMap<int, std::string> map;
    // Keys 0, 4, 8, 12 all map to bucket 0 with initial capacity 4
    map.set(0,  "zero");
    map.set(4,  "four");
    map.set(8,  "eight");
    // Note: inserting 4 elements triggers rehash, so set last after
    EXPECT_EQ(map.get(0), "zero");
    EXPECT_EQ(map.get(4), "four");
    EXPECT_EQ(map.get(8), "eight");
}

TEST(Collision, RemoveFromCollidingBucket) {
    HashMap<int, int> map;
    map.set(0, 1);
    map.set(4, 2);
    map.remove(0);
    EXPECT_FALSE(map.exist(0));
    EXPECT_TRUE(map.exist(4));
    EXPECT_EQ(map.get(4), 2);
}

// ================================================================
//  SECTION 13: const HashMap access
//  Probes: does const V& get(K) const dispatch correctly?
//  A real bug here would be a compile error or wrong value returned.
// ================================================================

TEST(ConstAccess, ConstRefCallsConstGet) {
    HashMap<int, int> map;
    map.set(1, 100);
    const HashMap<int, int>& constMap = map;
    EXPECT_EQ(constMap.get(1), 100);
}

TEST(ConstAccess, ConstRefExistWorks) {
    HashMap<std::string, std::string> map;
    map.set("hello", "world");
    const HashMap<std::string, std::string>& constMap = map;
    EXPECT_TRUE(constMap.exist("hello"));
    EXPECT_FALSE(constMap.exist("missing"));
}

TEST(ConstAccess, ConstRefGetMissingThrows) {
    HashMap<int, int> map;
    map.set(1, 1);
    const HashMap<int, int>& constMap = map;
    EXPECT_THROW(constMap.get(999), const char*);
}

TEST(ConstAccess, ConstRefGetSizeAndEmpty) {
    HashMap<int, int> map;
    map.set(1, 1);
    map.set(2, 2);
    const HashMap<int, int>& constMap = map;
    EXPECT_EQ(constMap.getSize(), 2);
    EXPECT_FALSE(constMap.empty());
}

// ================================================================
//  SECTION 14: get() reference validity (no-rehash case)
//  After getting a V& reference, a set() that does NOT trigger
//  rehash must NOT invalidate the reference.
//  (capacity=4, inserting key 2 at size=1 stays at load=0.5 < 0.75)
// ================================================================

TEST(GetRef, RefRemainsValidAfterNonRehashSet) {
    HashMap<int, int> map;
    map.set(1, 10);
    int& ref = map.get(1);
    map.set(2, 20);  // load = 2/4 = 0.5 → no rehash
    // ref must still point to key=1's value
    EXPECT_EQ(ref, 10);
}

TEST(GetRef, RefReflectsMutation) {
    // Mutating through ref should be visible via get()
    HashMap<std::string, int> map;
    map.set("counter", 0);
    int& ref = map.get("counter");
    ref++;
    ref++;
    EXPECT_EQ(map.get("counter"), 2);
}

// ================================================================
//  SECTION 15: Size drift under mixed operations
//  A common bug: size counter gets out of sync when mixing
//  set/overwrite/remove/clear operations.
// ================================================================

TEST(SizeDrift, SizeAfterMixedOps) {
    HashMap<int, int> map;
    // insert 5 unique keys
    for (int i = 0; i < 5; i++) map.set(i, i);
    EXPECT_EQ(map.getSize(), 5);

    // overwrite 3 of them → size must stay 5
    map.set(0, 99);
    map.set(2, 99);
    map.set(4, 99);
    EXPECT_EQ(map.getSize(), 5);

    // remove 2 → size must be 3
    map.remove(1);
    map.remove(3);
    EXPECT_EQ(map.getSize(), 3);

    // insert 2 new → size must be 5
    map.set(10, 10);
    map.set(11, 11);
    EXPECT_EQ(map.getSize(), 5);
}

TEST(SizeDrift, SizeAfterRemoveAndReinsertSameKey) {
    HashMap<std::string, int> map;
    map.set("x", 1);
    EXPECT_EQ(map.getSize(), 1);

    map.remove("x");
    EXPECT_EQ(map.getSize(), 0);

    map.set("x", 2);       // re-insert same key
    EXPECT_EQ(map.getSize(), 1);  // must be 1, not 2
    EXPECT_EQ(map.get("x"), 2);
}

TEST(SizeDrift, SizeNeverGoesNegative) {
    HashMap<int, int> map;
    map.set(1, 1);
    map.remove(1);
    EXPECT_GE(map.getSize(), 0);   // size must be >= 0
    EXPECT_EQ(map.getSize(), 0);
}

// ================================================================
//  SECTION 16: Multiple clear() cycles
//  clear() calls ~DoublyLinkedList() explicitly then assigns — risky.
//  This tests that the internal state is fully reset each time.
// ================================================================

TEST(MultipleClear, TwoClearCycles) {
    HashMap<int, int> map;
    // First fill and clear
    map.set(1, 1); map.set(2, 2); map.set(3, 3);
    map.clear();
    EXPECT_EQ(map.getSize(), 0);
    EXPECT_TRUE(map.empty());

    // Second fill and clear
    map.set(10, 10); map.set(20, 20);
    map.clear();
    EXPECT_EQ(map.getSize(), 0);
    EXPECT_TRUE(map.empty());
}

TEST(MultipleClear, DataAfterThreeCycles) {
    HashMap<std::string, int> map;
    for (int cycle = 0; cycle < 3; cycle++) {
        map.set("a", cycle * 10);
        map.set("b", cycle * 20);
        EXPECT_EQ(map.get("a"), cycle * 10);
        EXPECT_EQ(map.get("b"), cycle * 20);
        EXPECT_EQ(map.getSize(), 2);
        map.clear();
    }
    EXPECT_EQ(map.getSize(), 0);
}

// ================================================================
//  SECTION 17: Remove all entries one by one, then refill
//  Tests that the map is fully functional after being emptied.
// ================================================================

TEST(RemoveAllRefill, RemoveAllThenRefill) {
    HashMap<int, int> map;
    for (int i = 0; i < 8; i++) map.set(i, i * 5);

    // Remove all one by one
    for (int i = 0; i < 8; i++) {
        EXPECT_TRUE(map.exist(i));
        map.remove(i);
        EXPECT_FALSE(map.exist(i));
    }
    EXPECT_EQ(map.getSize(), 0);
    EXPECT_TRUE(map.empty());

    // Refill and verify
    for (int i = 0; i < 8; i++) map.set(i, i * 100);
    for (int i = 0; i < 8; i++) EXPECT_EQ(map.get(i), i * 100);
    EXPECT_EQ(map.getSize(), 8);
}

// ================================================================
//  SECTION 18: Overwrite does not leave ghost entry
//  Bug: if set() removes and re-appends but doesn't decrement
//  size, getSize() inflates. Or if a second copy of the key
//  lingers in the chain, get/exist could return a stale value.
// ================================================================

TEST(GhostEntry, OverwriteThenRemoveLeavesSizeZero) {
    HashMap<int, int> map;
    map.set(1, 10);
    map.set(1, 99);  // overwrite
    map.remove(1);
    EXPECT_EQ(map.getSize(), 0);
    EXPECT_FALSE(map.exist(1));
    EXPECT_THROW(map.get(1), const char*);
}

TEST(GhostEntry, OverwriteNoDuplicateInChain) {
    // After overwriting, only ONE entry should exist for the key.
    // If there's a ghost, remove() would leave a duplicate behind
    // and exist/get would still find it.
    HashMap<int, int> map;
    map.set(42, 1);
    map.set(42, 2);  // overwrite
    map.set(42, 3);  // overwrite again
    map.remove(42);

    EXPECT_FALSE(map.exist(42));          // must be gone
    EXPECT_THROW(map.get(42), const char*); // must throw
    EXPECT_EQ(map.getSize(), 0);           // must be 0, not -1 or 1
}
