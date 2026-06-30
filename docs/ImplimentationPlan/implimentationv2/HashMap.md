# HashMap Implementation Plan

## Section 1 — Public API

List of proposed methods:

- `set(const K& key, const V& value)`: Inserts a new key-value pair, or updates the value if the key already exists.
- `get(const K& key)`: Retrieves the value associated with the key. Throws an error if the key doesn't exist.
- `remove(const K& key)`: Deletes a key-value pair from the map.
- `getSize()`: Returns the total number of key-value pairs stored.
- `empty()`: Returns true if the map is empty.
- `clear()`: Removes all key-value pairs from the map.
- `rehash()`: Resizes the hash table and rehashes all key-value pairs.
- `exists(const K& key)`: Checks if a specific key exists in the map.

**Why this decomposition is better:**
The `set` method handles both inserting brand new items and updating existing ones, which mirrors how standard maps work and keeps the API simple for the user. Because our map is generic, `get` cannot easily return a "null" value for primitive types like `int` if a key is missing. Therefore, `get` will throw an error for missing keys, making the `exists` method absolutely necessary so users can safely check for existence first. We pass keys and values by reference (`const K&`) to prevent the program from slowing down by copying large user-defined objects. 

## Section 2 — Internal Representation

The HashMap is a composition of our custom collections. The main storage is a `DynamicArray` which acts as our bucket array. Inside each bucket, we store a `DoublyLinkedList` that holds `Node<K, V>` objects. This structure directly handles collisions through a method called "separate chaining."

**Rule of Three (Memory Management):**
- **Destructor:** Because our HashMap is built purely out of our custom `DynamicArray` and `DoublyLinkedList` classes, we actually get memory safety for free. When the `HashMap` goes out of scope, the `DynamicArray`'s destructor is automatically called, which automatically calls the destructor of every `DoublyLinkedList` in its buckets, freeing all the heap-allocated nodes. We do not have to write manual `delete` statements inside the HashMap's destructor.
- **Copy Constructor and Copy Assignment:** We will rely on **deep copying**. Since our `DynamicArray` and `DoublyLinkedList` both have robust deep copy constructors already built, copying a `HashMap` simply invokes those existing deep copies. 
  - *Consequence of Deep Copy:* Complete safety. Two HashMaps can have the exact same data, but entirely different memory addresses for their buckets and nodes.
  - *Consequence of Shallow Copy (Rejected):* A shallow copy would make two HashMaps point to the exact same dynamic array of buckets. When destroyed, the system would attempt to delete the array twice, causing a fatal crash.

## Section 3 — Complexity Estimates

- **`set(const K& key, const V& value)`:**
  - **Best Case:** O(1)
  - **Why:** The hash function instantly calculates the bucket index. If the bucket is empty, we just append the node to the linked list immediately.
  - **Worst Case:** O(N)
  - **Why:** If all keys hash to the exact same bucket (a terrible hash function), the linked list becomes $N$ elements long. We have to traverse the entire list to ensure the key doesn't already exist before adding it. A resize operation (rehashing) also takes O(N) time.
  - **Average Case:** O(1) amortized
  - **Why:** Because we resize when the map gets 75% full, chains stay very short (usually 1 or 2 items). The O(N) cost of rehashing is spread out logarithmically, so the average cost is O(1).

- **`get(const K& key)` and `containsKey(const K& key)`:**
  - **Best Case:** O(1)
  - **Why:** The bucket index is calculated instantly, and the key is the very first item in that bucket's linked list.
  - **Worst Case:** O(N)
  - **Why:** Severe collisions mean we might have to scan a long linked list of $N$ items to find the key.
  - **Average Case:** O(1)
  - **Why:** Proper hashing keeps the linked list chains extremely short, so we typically only check 1 or 2 items.

- **`remove(const K& key)`:**
  - **Best Case:** O(1)
  - **Why:** The item to delete is the first one in the bucket, so the linked list pointer is rewired instantly.
  - **Worst Case:** O(N)
  - **Why:** We must traverse a long list of $N$ items to find the key to remove it.
  - **Average Case:** O(1)-(for all cases)
  - **Why:** Short bucket chains make removal near instant.

- **`size()`:** O(1)-(for all cases)
  - **Why:** The `size` is just an integer variable we increment/decrement that is updated on insertion and deletion. Returning it happens instantly.

## Section 4 — Design Decisions

**Why Choose Separate Chaining over Open Addressing?**
We chose **Separate Chaining** using our `DoublyLinkedList` to handle hash collisions. When two keys end up at the same bucket, we simply add the new entry to the linked list in that bucket. 

**Why Use DynamicArray and DoublyLinkedList?**
   main storage is a `DynamicArray` which acts as our bucket array. Inside each bucket, we store a `DoublyLinkedList` that holds `Node<K, V>` objects. This structure directly handles collisions through a method called "separate chaining."

**Why a Load Factor of 0.75?**
We chose a load factor threshold of 0.75. This means when the table is 75% full (e.g., 6 items in 8 buckets), we trigger a rehash, double the buckets, and redistribute the keys.
- *Alternatives Rejected:* A factor of 1.0 (waiting until it is 100% full) causes far too many collisions before resizing, significantly slowing down lookups. A factor of 0.5 (resizing at 50% full) wastes too much memory by keeping half the array empty at all times. 0.75 perfectly balances memory efficiency and lookup speed.

**Generic implementation:** because it allows the same hashMap to store different data types without modifying the underlying code and also works for user-defined data types.

# HashFunction Implimentation
