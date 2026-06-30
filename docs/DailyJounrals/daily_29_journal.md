# Daily Design Journal · daily_29_journal.md

## Section 1 — Specific Bug
Encountered a performance design flaw in the `HashMap::clear()` method. The first attempt re-allocated a brand-new bucket array of size 4 and freed the old one, causing unnecessary heap operations:
```text
// BAD: Shrinks capacity and triggers heap allocations on every clear
void HashMap::clear() {
    delete[] buckets;
    capacity = 4;
    buckets = new DoublyLinkedList<Node<K,V>>[capacity];
    size = 0;
}
```
**Fix:** Rewrote `clear()` to preserve the existing `capacity` and iterate over the already-allocated buckets array, assigning an empty `DoublyLinkedList` to each slot via the overloaded `operator=`. This leverages the existing destructor logic inside `DoublyLinkedList` to cleanly free all nodes, avoiding unnecessary reallocation overhead:
```cpp
// GOOD: Reuses existing bucket array, O(n) node cleanup with zero heap realloc
void HashMap<K, V>::clear() {
    for (int i = 0; i < capacity; i++) {
        buckets.get(i) = DoublyLinkedList<Node<K, V>>();
    }
    size = 0;
}
```

## Section 2 — Failed Attempt
When implementing the `CommandParser`, my first instinct was to use `std::stringstream` to split the raw input string into tokens. This would have been a one-liner. However, this directly violates the project's core "build-from-scratch" philosophy of the `SuperCodersCollectionsLibrary` — relying on STL parsing utilities contradicts the entire point of the exercise.

My second thought was to use `std::transform` for uppercase conversion. Again, that's just a wrapper around a loop we can write ourselves. I scrapped both approaches and instead built manual character-by-character loop logic: index tracking to find whitespace boundaries for tokenization, and ASCII arithmetic (`ch - 32`) to convert lowercase command characters to uppercase. The result is a fully self-contained parser with zero STL parsing dependencies.

## Section 3 — Code Reference
- **Filenames & Lines:**
  - `src/customLibrery/hashMap.cpp` — `exist()`, `clear()`, `empty()` utility methods added to complete the `HashMap` API.
  - `include/redisLite.h` — `RedisLite` class declared with full public interface: `set`, `get`, `del`, `exist`, `clear`, `size`, `empty`.
  - `src/customLibrery/redisLite.cpp` — `RedisLite` method implementations delegating to the internal `HashMap` member.
  - `include/commandParser.h` — `CommandParser` class declared with `startREPL()` and `parseAndExecute()`.
  - `src/customLibrery/commandParser.cpp` — Manual tokenizer and ASCII-based uppercase converter powering the REPL loop.
  - `CMakeLists.txt` — Updated to include `redisLite.cpp` and `commandParser.cpp` in the `CustomLibrary` build target.

#### Implemented Functions — Session 1 (RedisLite Wrapper)
- **`set(K key, V value)`**: Delegates directly to `HashMap::set`. Stores or updates a key-value pair.
- **`get(K key)`**: Delegates to `HashMap::get`. Retrieves the value for a given key.
- **`del(K key)`**: Uses `del` instead of `delete` to avoid collision with the C++ reserved keyword. Removes a key-value pair.
- **`exist(K key)`**: Returns a boolean indicating whether a key is present in the underlying map.
- **`clear()`**: Wipes all stored pairs using the optimized `HashMap::clear()` without reallocation.
- **`size()`**: Returns the current count of active key-value pairs.
- **`empty()`**: Returns `true` if the database contains zero entries.

#### Implemented Functions — Session 2 (Command Parser / REPL)
- **`startREPL()`**: Enters an infinite read-eval-print loop, reading raw terminal input and dispatching to `parseAndExecute()` until the user types `EXIT`.
- **`parseAndExecute()`**: Manually tokenizes the input string using index-based whitespace detection, converts the command token to uppercase via ASCII subtraction, then dispatches to the correct `RedisLite` method.

## Section 4 — Learning Reflection
Today had two distinct arcs that reinforced one another perfectly.

**Session 1** taught me that API completeness matters as much as correctness. The core `HashMap` was functionally sound, but it was missing the utility methods (`exist`, `clear`, `empty`) that make it actually usable as a database primitive. More importantly, the `clear()` bug was a great reminder that performance isn't just about algorithms — it's about understanding *when* to avoid memory allocation entirely. Reusing existing capacity is almost always cheaper than free-and-reallocate.

**Session 2** was a masterclass in why constraints are valuable. Being forbidden from using `<sstream>` and `<algorithm>` forced me to implement tokenization and character transformation manually. Writing a character-by-character loop to detect whitespace boundaries and applying ASCII arithmetic (`'a'` = 97, `'A'` = 65, delta = 32) demystified what those standard library functions are actually doing under the hood. The result is a parser I fully understand and can debug, extend, or port without reaching for a reference manual.

Taken together, Day 29 closed the loop on the RedisLite data layer and user-facing interface: a clean database wrapper over a battle-tested custom HashMap, with a hand-built CLI sitting on top.
