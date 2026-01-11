# my_alloc
A university exercise

It basically creates a virtual adress space, that handle memory without using malloc() and free().
The size is passed as argument in the main function (a number multiplied by 1024).
A data structure stores the information of every node, related to its segment of memory.

To-do
Implement the merge function. It should merge two consecutive free segments, to be able to alloc a bigger portion of memory.
