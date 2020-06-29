# Partially Ordered Set of Strings
## Description
The purpose of this task is to write a C++ module that handles Partially Ordered Sets(poset) whose elements are strings. The module is to be used in C and is to provide the following functions:
- `unsigned long poset_new(void);` - Creates a new poset and returns its identifier.
- `void poset_delete(unsigned long id);` - If there is a poset with the id `id`, deletes it, in opposite case does nothing.
- `size_t poset_size(unsigned long id);` - If there is a poset with id `id`, the result is its number elements, otherwise `0`.
- `bool poset_insert(unsigned long id, char const * value);` - If there is a poset with id `id` and `value` element does not belong to of this set, adds the element to the set, otherwise does nothing. The new element is not in relation to any element. The result is `true` if the item was added and `false` otherwise.
- `bool poset_remove(unsigned long id, char const * value);` - If there is a poset with id `id` and `value` element belongs to this set, removes the element from the set and removes all relations of that element, otherwise it does nothing. The result is `true` when the item has been removed and `false` otherwise.
- `bool poset_add(unsigned long id, char const * value1, char const * value2);` - If there is a poset with id `id` and `value1` and `value2` elements belong to this set and are not in a relationship, extends the relationship so that the `value1` element precedes the `value2` element (closes the relation transitive), otherwise it does nothing. The result is `true` when the relationship has been extended and `false` otherwise.
- `bool poset_del(unsigned long id, char const * value1, char const * value2);` - If there is a poset with id `id`, elements `value1` and `value2` belong to this set, the `value1` element precedes the `value2` element and deleting the relationship between `value1` and `value2` will not disturb the conditions of being a partial ordering, removes the relationship between these elements, otherwise does nothing. The result is `true` when the relationship has been changed and `false` otherwise.
- `bool poset_test(unsigned long id, char const * value1, char const * value2);` - If there is a poset with id `id`, elements `value1` and `value2` belong to this set, the `value1` element precedes the `value2` element, then the result is `true`, otherwise `false`.
- `void poset_clear(unsigned long id);` - If there is a poset with the id `id`, removes all its elements and the relationship between them, otherwise it does nothing.

## Example Tests
Example Tests can be compiled using:

```shell script
g++ -Wall -Wextra -O2 -std=c++17 -c poset.cc -o poset.o
gcc -Wall -Wextra -O2 -std=c11 -c poset_example1.c -o poset_example1.o
g++ -Wall -Wextra -O2 -std=c++17 -c poset_example2.cc -o poset_example2.o
g++ poset_example1.o poset.o -o poset_example1
g++ poset_example2.o poset.o -o poset_example2
```