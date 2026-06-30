# Daily Design Journal · day_23_journal.md

## Section 1 — Specific Bug
Encountered a variable shadowing and scope error when refactoring the `LinkedList` properties from `_size` to `size` without updating all class methods:
```text
linkedlist.cpp: In destructor 'LinkedList<T>::~LinkedList()':     
linkedlist.cpp:48:9: error: '_size' was not declared in this scope
; did you mean 'size'? [-Wtemplate-body]
   48 |         _size = 0;
      |         ^~~~~
      |         size
```

## Section 2 — Failed Attempt
When transitioning from `LinkedList` to `DoublyLinkedList`, my first thought was to use class inheritance (e.g., `class DoublyLinkedList : public LinkedList`) to save time. This failed conceptually and architecturally. `LinkedList` relies on a unidirectional `Node`, whereas `DoublyLinkedList` requires a bidirectional `Node` with a `prev` pointer. Inheriting would mean either carrying around useless singly-linked node logic or completely overriding every single method in the base class. It would have violated the Liskov Substitution Principle and caused memory bloat due to unused inherited properties. Therefore, I scrapped inheritance and built `DoublyLinkedList` as a completely independent, standalone template class.

## Section 3 — Learning Reflection
My understanding of the memory boundaries between C and C++ completely changed today. I used to view `new` and `delete` merely as the "C++ syntax" for `malloc` and `free`. Today, I realized that memory allocation and object initialization are two strictly separate phases in C++. `malloc` *only* reserves the raw heap space. If you store a complex template object (like `std::string` or a custom class) in that raw space without calling its constructor, the program will exhibit undefined behavior. Placement `new` (`new (&ptr) T(val)`) bridges this exact gap by forcing the constructor to run on pre-allocated raw memory. Furthermore, I learned that you cannot just `free()` an object—you must manually invoke its destructor (`ptr->data.~T()`) first, otherwise any dynamic memory held *inside* that object will leak.
