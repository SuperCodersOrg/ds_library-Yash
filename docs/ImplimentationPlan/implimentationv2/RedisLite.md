# RedisLite Implementation Plan

## Section 1 — Public API

**List of proposed methods for the Database Engine (`RedisLite` class):**
- `void set(const string& key, const string& value)`: Core method to insert or update data in the database.
- `string get(const string& key)`: Core method to retrieve data.
- `void del(const string& key)`: Deletes a key from the database.
- `void clear()`: Completely flushes all data from the database.
- `int size() const`: Returns the number of active keys.

**List of proposed methods for the Parsing Layer (`CommandParser` module/class):**
- `void parseAndExecute(const string& command)`: A custom REPL interface that processes raw CLI strings (e.g., `"SET name Yash"`) and maps them to the internal engine operations.
- `DynamicArray<string> tokenize(const string& command)`: Manually splits the input string into a list of usable tokens.

**Why this decomposition is better:**
By separating the `CommandParser` from the `RedisLite` engine, we adhere to the Single Responsibility Principle. The engine strictly handles data storage and retrieval, while the parser is solely responsible for string manipulation and user interactions. Instead of exposing raw memory APIs to the user, the `CommandParser` mimics the real-world Redis CLI.

## Section 2 — Internal Representation

The internal state currently relies exclusively on our previously built `HashMap` collection to act as the primary key-value storage layer. 

**Rule of Three (Memory Management):**
- **Destructor:** Because the core data storage relies entirely on our custom `HashMap` (which internally manages its own `DynamicArray` and `LinkedList` destructors safely), the `RedisLite` engine itself does not need to manually manage raw pointers in its destructor. When the engine goes out of scope, the underlying collections clean themselves up automatically, guaranteeing zero memory leaks.
- **Copy Constructor and Copy Assignment (DELETED):** 
  - *Decision:* We will explicitly delete these (`= delete;`) to **prevent** copying.
  - *Why:* `RedisLite` acts as a monolithic database engine. It makes zero architectural sense to copy an entire running database in memory during runtime. If a developer accidentally passes the database object by value to a function, the compiler will throw a strict error, saving the system from a catastrophic O(N) memory duplication crash.

## Section 3 — Complexity Estimates

- **`set(const string& key, const string& value)`:** O(1) amortized
  - **Why:** Finding or inserting the key in the underlying `HashMap` takes O(1) time on average. 
- **`get(const string& key)`:** O(1) amortized
  - **Why:** The `HashMap` locates the key and returns the associated string value instantly (O(1)).
- **`del(const string& key)`:** O(1) amortized
  - **Why:** We instantly locate the key in the `HashMap` and remove its node from the hash bucket in O(1) time.
- **`CommandParser::parseAndExecute()` / `tokenize()`:** O(C)
  - **Why:** $C$ is the number of characters in the command string. Because we manually parse the string character-by-character to extract tokens without heavy standard library utilities, the time taken scales strictly linearly with the length of the string input.

## Section 4 — Design Decisions

**CommandParser and Zero-STL Parsing:**
We introduced a dedicated `CommandParser` to decouple CLI parsing logic from the core database engine. For the implementation, we completely rejected using C++ standard library utilities like `<sstream>`, `std::stringstream`, or `std::transform` for string tokenization. Manually iterating through character arrays to tokenize the commands forces a strict adherence to our core educational philosophy: maintaining complete architectural control and understanding low-level string manipulation without relying on "black-box" standard utilities.

**Why rely solely on HashMap currently?**
For the current iteration, we are focusing on building a rock-solid fundamental key-value store. The custom `HashMap` provides O(1) lookups and handles collisions natively. This ensures the basic `SET`, `GET`, and `DEL` commands operate flawlessly before we introduce more complex caching or eviction logic.

## Section 5 — Future Improvements

**LRU (Least Recently Used) Cache Eviction Implementation**
In the future, we plan to implement a strict memory limit on the database using an **LRU Cache** architecture. 
- **How it will work:** We will introduce our `DoublyLinkedList<Node*>` collection to order the cached items chronologically. The `HashMap` will map string keys directly to these `Node*` pointers. When a node is accessed via `get` or `set`, we will use the `prev` and `next` pointers of the `DoublyLinkedList` to detach it in perfect O(1) time and move it to the "Most Recently Used" front of the list. When the database reaches maximum capacity, the "Least Recently Used" node at the back of the list will be instantly evicted. 
- **Why LRU over LFU:** LRU is significantly simpler to implement than LFU (Least Frequently Used) and provides highly predictable chronological cache eviction without the computational overhead of tracking frequency integers or managing multiple dynamic frequency lists.
