#Dictionary in C++
The goal of this task is to implement a container having pattern like a dictionary with the expected O(1) search time, at the same time having predictable order of key iterations, in the order in which they were added to the dictionary.

This container should provide as much safety as possible on exceptions and copy-on-write semantics.
As part of this task, you must implement a template

```c++
template<class K, class V, class Hash=std::hash<K>>
class insertion_ordered_map{
};
```

The template is to be parameterized with K, V and Hash types. It can be assumed that the the K key has value semantics, in particular, they are available to its parameterless default constructor, copy constructor, constructor transfer and assignment operators.

Type K may also be required to a hash function which was defined(Hash parameter) and an equality relation(operators == and! =).

Type V can only be assumed to have a copying constructor.

The Hash parameter should be the type of object defining the appropriate operator(),and its default value is `std::hash<K>`.The insertion_ordered_map class should provide the operations described below. At each operation its required time complexity is given, where n is the number of items stored in the dictionary. Expected time complexity of copying operation when saving should be O(n).

All operations must provide at least a strong exception safety,and the transfer constructor and destructor must be no-throw. 
Where possible and justified, add the noexcept qualifier.



`insertion_ordered_map();` - A parameterless constructor creating an empty dictionary.
- Time complexity O(1).


`insertion_ordered_map(insertion_ordered_map const& other);` - Copying constructor.
- It should have copy-on-write semantics, and therefore not copy internal data structures if it is not needed.
- Dictionaries share structures until one of them is modified.
- Time complexity O(1) or expected O(n) if cpoy execution is necessary

`insertion_ordered_map(insertion_ordered_map&& other);` - Transfer constructor.
- Time complexity O(1).

`insertion_ordered_map& operator=(insertion_ordered_map other);` - Assignment operator.
- Should take an argument by value. 
- Time complexity O(1).

`bool insert(K const& k, V const& v);` - Insert into dictionary.
- If the key k is not stored in the dictionary, then inserts the v value under the ki key and returns true.
If the key k already exists in the dictionary, the value under it does not change, but the key stays moved to the end of the iteration order, and the method returns false. if it is possible, avoid copying items already stored in the dictionary.
- Expected time complexity O(1) + possible time of copying the dictionary.

`void erase(K const& k);` - Delete from dictionary.
- Deletes the value under the given key k. If such a key does not exist, it raises the lookup_error exception.
- Expected time complexity O(1) + possible copying time.

`void merge(insertion_ordered_map const& other);` - Merging dictionaries.
- Adds copies of all elements of the specified other dictionary to the current dictionary(this).
Values ​​under the keys already present in the current dictionary are not overwritten. Keys from the other dictionary appear in order iterated from the end, keeping the order relative to each other.
- Expected time complexity O(n + m), where m is the size of the other dictionary.

`V& at(K const & k);`
`V const& at(K const& k) const;` - Value reference.
- Returns a reference to the value stored in the dictionary below given key k.
If no such key exists in the dictionary, it raises exception lookup_error.
This method should be available in the version with the attribute const and without it. 
- Expected time complexity O(1) + possible copying time.

`V& operator [](K const & k);` - Indexing operator.
- Returns a reference to the value in the dictionary under the given key k. As with STL containers, call of this operator with a key not found in the dictionary adds it under that key default value of type V. Of course, this operator should only work if and only if V has a parameterless constructor.
- Expected time complexity O(1).

`size_t size() const;` - Dictionary size.
- Returns the number of key-values ​​in the dictionary.
- Time complexity O(1).

`bool empty() const;` - Check empty.
- Returns true if the dictionary is empty and false otherwise.
- Time complexity O(1).

`void clear ();` - Dictionary cleaning.
- Removes all items from the dictionary.
- Time complexity O(n).

`bool contains (K const & k) const;` - Key check.
 - Returns a boolean value indicating whether the given key k is in dictionary.
 - Expected time complexity O(1).
 
  - An iterator class called iterator from the begin to end methods that allow browsing of the key set in the order in which they were inserted.
  Iterators can be annulled by any successful modification operation.
  Iterator should provide at least the following operations:
    - parameterless and copying constructor
    - prefix operator ++
    - operator == and operator !=
    - operator * (dereferenced)
- All operations during O(1). Review the entire dictionary in O(n).
- Iterators are only for browsing the dictionary and you cannot use them to modify it, so it behaves like a const_iterator from STL.

## Functional Requirements
- The lookup_error class should be defined outside the class insertion_ordered_map and should inherit from std::exception.
- The insertion_ordered_map class should be transparent to exceptions, i.e. it should pass any exceptions thrown by its functions and by operations on its components, and the observable state of the objects should not change.
In particular, unsuccessful modification operations failure should invalidate iterators.



The solution will be compiled using the command
```shell script
g++ -Wall -Wextra -std=c++17 -O2
```
and tested for proper memory handling, e.g. using:
```shell script
valgrind --error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --run-cxx-freeres=yes
```





























