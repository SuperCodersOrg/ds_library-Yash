# LinkedList & DoublyLinkedList Implementation Plan

## Section 1 — Public API

We are implementing both a **Singly Linked List** and a **Doubly Linked List**.

### Singly Linked List
- `append(const T& value)`: Adds a node to the end of the list.
- `get(int index)`: Returns a reference to the element at the specified index.
- `insert(int index, const T& value)`: Inserts an element at a specific index.
- `remove(int index)`: Removes an element at a specific index.
- `int size()`: Returns the number of nodes currently stored.
- `bool isEmpty()`: Returns true if the list has no elements.

### Doubly Linked List
- `append(const T& value)`: Adds a node to the end of the list.
- `get(int index)`: Returns a reference to the element at the specified index.
- `insert(int index, const T& value)`: Inserts an element at a specific index.
- `remove(int index)`: Removes an element at a specific index.
- `int size()`: Returns the number of nodes currently stored.
- `bool isEmpty()`: Returns true if the list has no elements.

**Why this decomposition is better:**
The public API deliberately mirrors the `DynamicArray` to maintain a consistent, interchangeable interface across our custom collections library. Instead of forcing an O(N) traversal to count nodes, we maintain a `size` variable for an instant O(1) lookup. Passing `const T& value` avoids unnecessary copying of user-defined data structures. Returning `T&` allows the user to modify elements in place without needing to extract and re-insert them. 

**Rule of Three** (Memory Management):
- **Destructor:** Because memory is allocated node-by-node on the heap, the destructor must manually free every node. It will execute an O(N) traversal: starting from `head`, it will temporarily store the `next` pointer, call `delete current_node;`, and move forward until `head` is `nullptr`. Failing to do this causes severe memory leaks.
- **Copy Constructor & Copy Assignment:** Both lists will implement **deep copying**. This means when copying a list, we traverse the original list and dynamically allocate brand new `Node` objects on the heap for the new list, wiring their pointers accordingly.
  - *Consequence of Deep Copy:* Perfect memory safety. The original and copied lists operate independently. Modifying or destroying one has zero effect on the other.
  - *Consequence of Shallow Copy (Rejected):* A shallow copy would just duplicate the `head` (and `tail`) pointers. Both objects would point to the exact same nodes. When one list goes out of scope, its destructor deletes the nodes. When the second list goes out of scope, it attempts to delete the already-deleted nodes, triggering a fatal "double free" crash.

## Section 3 — Complexity Estimates

**Singly Linked List:**
- **`append(const T& value)`:** O(N) (for all cases worst, best and average)
  - **Why:** The Singly Linked List only tracks the `head` pointer. To add an element to the end, we must start at the head and follow `next` pointers until we reach the last node, taking O(N) time.

- **`get(int index)`:** O(N) (for all cases worst, best and average)
  - **Why:** Unlike a Dynamic Array, memory is non-contiguous. We cannot calculate the address mathematically. We must traverse `index` number of nodes one by one.

- **`insert(int index, const T& value)` / `remove(int index)`:** O(N) worst-case (O(1) best-case and O(N) average-case)
  - **Why:** Reaching the specific index requires O(N) traversal. However, once the target node is found, rewiring the `next` pointers takes only O(1) time.

**Doubly Linked List:**
- **`append(const T& value)`:** O(1)
  - **Why:** The Doubly Linked List maintains a `tail` pointer. We can instantly jump to the end of the list and attach the new node without traversing.

- **`get(int index)`:** O(N) worst-case, O(1) best-case, Average-case O(N/2)
  - **Why:** We still must traverse non-contiguous nodes. However, knowing both `head` and `tail`, we can optimize by checking if the requested index is closer to the start or the end, and traversing from that direction.

- **`insert(int index, const T& value)` / `remove(int index)`:** O(N) worst-case, O(1) best-case, Average-case O(N)
  - **Why:** Traversal to the index still takes O(N) time. However, having a `prev` pointer makes the subsequent O(1) pointer rewiring step safer and cleaner, especially during removal, since we don't need to manually track the "previous" node during traversal.


- **`size()` and `isEmpty()` (Both Lists):** O(1)
  - **Why:** The number of nodes is maintained in a dedicated integer `size` variable which is updated incrementally on every insertion or deletion.

## Section 4 — Design Decisions

**Added tail pointer in doubly linked list to make append operation O(1).**

**Added size variable in both lists:** to make size() and isEmpty() operations O(1).

**Generic implementation:** because it allows the same Linked List to store different data types without modifying the underlying code and also works for user-defined data types.

**Two get methods:**
    - const T& get(int index) const; (to handel the constant user defined objects)
    - T& get(int index); (to handel the non constant user objects)

**use of malloc and free keyword:**
    -malloc() allocates memory without calling constructors and constructor and destructors are called using placment new.

## Why Linked Lists over Dynamic Arrays?
Linked Lists are chosen for scenarios where memory must be allocated strictly "on-demand," avoiding the unused capacity overhead of Dynamic Arrays. Furthermore, they are immune to the O(N) element shifting penalty that arrays suffer when items are inserted or removed in the middle or beginning.




