# Date: June 29
Duration: 60 minutes

Goal: 
Implement the `RedisLite` class as a wrapper around our custom `HashMap`, providing standard key-value database methods (`set`, `get`, `del`, `exist`, `clear`, `size`, `empty`). Modularize the code by separating declarations and implementations into `.h` and `.cpp` files, and ensure the project correctly compiles using the CMake build system.

Problem Encountered: 
1. The base custom `HashMap` was missing essential query and management methods like `exist`, `clear`, and `empty`.
2. The initial implementation of the `clear` method in `HashMap` caused performance issues by shrinking the capacity back to 4 and unnecessarily allocating/de-allocating new bucket arrays, defeating the purpose of pre-allocated buckets.

## What I Tried: 
1. Added missing utility methods (`exist`, `clear`, `empty`) to the `HashMap` implementation.
2. Optimized the `HashMap::clear()` method to keep the current capacity and iterate over existing buckets, assigning an empty `DoublyLinkedList` to each. This brilliantly leverages the existing `DoublyLinkedList::operator=` to cleanly free underlying nodes without memory leaks or reallocation overhead.
3. Designed the `RedisLite` class. First we defined all methods inside `redisLite.h`, then successfully separated the logic by putting the implementations into `src/customLibrery/redisLite.cpp`.
4. Handled naming conventions by using `del` instead of `delete` to avoid C++ keyword collisions.
5. Updated `CMakeLists.txt` to include `src/customLibrery/redisLite.cpp` into the `CustomLibrary` build target.

## Outcome: 
Successfully implemented the `RedisLite` database wrapper that runs flawlessly over our custom `HashMap` collection. The underlying memory management inside `HashMap::clear` is highly optimized. The code structure is now highly modular with separated headers and source files, and correctly builds and links using CMake. All tests pass successfully in `main.cpp`.
