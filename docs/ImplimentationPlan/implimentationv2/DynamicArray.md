# DynamicArray Implementation Plan

## Section 1 — Public API

List of proposed methods:

- `append(const T& value)`: Adds an element to the end of the array.
- `get(int index)`: Returns a reference to the element at the specified index.
- `insert(int index, const T& value)`: Inserts an element at a specific index.
- `remove(int index)`: Removes an element at a specific index.
- `size()`: Returns the number of elements currently stored.
- `getCapacity()`: Returns the current maximum capacity before resizing is needed.
- `isEmpty()`: Returns true if the array has no elements.

**Why this decomposition is better:**
While the initial suggested interfaces might only require basic operations (`append`, `get`, `remove`, `size`), adding `getCapacity()` allows for better visibility and testing of our memory growth strategy. Additionally, using `const T& value` for insertion parameters is a necessary optimization over pass-by-value; it prevents redundant and expensive copying of complex user-defined objects. Returning a reference (`T&`) from `get()` allows modifying existing elements in place, which is critical when storing structures that need updating without extraction and re-insertion.

## Section 2 — Complexity Estimates

- **`DynamicArray get(int index)`**: O(1)-(For all cases best, worst, average)
  - **Why:** The array is stored in contiguous memory. The index directly calculates the memory address as `data[index]` using pointer arithmetic, jumping instantly to the item without traversing previous elements.

- **`DynamicArray append(const T& value)`**: O(1) amortized-(For all cases best, worst, average)
  - **Why:** In most cases, there is spare capacity and the element is just placed at `data[size]`, taking O(1) time. When the array is full, an occasional resize occurs, forcing us to allocate a new array and copy all $N$ elements, taking O(N) time. Because we double the capacity on each resize, resizes happen logarithmically rarely. When averaged across N appends, the cost distributes evenly, making the amortized time O(1) per append.

- **`DynamicArray insert(int index, const T& value)`**: worst case -O(N), best case - O(1), average case - O(N)
  - **Why:** To insert an element in the middle or beginning of the array, all subsequent elements must be shifted one position to the right to create a gap for the new element. In the worst case (inserting at index 0), we must shift all $N$ elements, resulting in linear time complexity.

- **`DynamicArray remove(int index)`**: worst case -O(N), best case - O(1), average case - O(N)
  - **Why:** Similar to insertion, removing an element leaves a gap. We must shift all elements after the removed index one position to the left to keep the memory contiguous. In the worst case (removing at index 0), this requires $N$ shifts.

- **`DynamicArray size()`, `getCapacity()`, `isEmpty()`**: O(1)
  - **Why:** These operations simply read and return the current value of existing internal integer state variables, which takes a single CPU instruction.

## Section 3 — Design Decisions

Initial Capacity = 4
Reason:  
 because for lightweight applications size don`t grow or waste rapidly and Reduces initial memory waste

I selected resizing factor 2 (starting at capacity 4) because it offers optimal cache locality and memory contiguity, which makes sequential reads blazing fast. It is the mathematically proven(above) way to achieve O(1) amortized bounds on dynamic insertions.

Using malloc() and free(): This approach was considered because malloc() allocates memory without calling constructors and constructor and destructors are called using placment new.

Two get methods:
    - const T& get(int index) const; (to handel the constant user defined objects)
    - T& get(int index); (to handel the non constant user objects)

**Generic implementation:** because it allows the same DynamicArray to store different data types without modifying the underlying code and also works for user-defined data types.





