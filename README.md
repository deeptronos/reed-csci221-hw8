# Homework 8

**By:** Cole Nemec & Evan Sieden


Homework 8 time!

### Note:
```makefile``` will be the makefile for the entirety of this project, ```hw 7_makefile``` makes everything from the files associated with HW7.

#### On HW7:
The files associated with homework 7 - hforest.hh/.cc, test_hforest.cc, htree.hh/cc, test_htree.cc, and the HW7 makefile - were written by Taylor Blair and Cole Nemec. For this assignment, HForest needed to have a method, ```pop_tree()```, updated so that it "popped" the tree node with the lowest value, rather than the tree node with the highest value, as was done in HW7.

#### Design Decisions:

1. **Adaptive Huffman encoding.**

2. **Bit I/O.** Most notably, we used ```std::bitset``` in the implementation of the ```BitIO``` class since it made many of the operations on and manipulations of binary data trivial. For instance, rather than shifting bits and using masks and or bitwise AND operations to find out the values of individual bits, ```std::bitset<N>::test()``` can be used to find the value of a bit at a given position. What's more, ```std::bitset```'s requirement of allocating the number of bits upon construction was not a problem since the ```BitIO``` class either inputs or outputs bytes of data at a time. Hence, we can safely declare the buffer like so: ```std::bitset<8> buffer_;```. To facilitate the conversion between data of types ```std::bitset``` and ```char```, we implemented the function ```bitset_to_char()``` which does exactly as the name suggests. Conversely, to convert from ```char``` to ```std::bitset``` we explicitly casted the ```char``` into an ```unsigned long``` and assigned that to ```buffer_```; this conversion is another example of operations that were made much easier due to our choice in using ```std::bitset```. As for ```BitIO```'s other methods that were declared in ```bitio.hh```, their implementation is explained in detail within ```bitio.cc```.

3. **Encoder/decoder.**

4. **Compression tests.**

#### Additional Credits:
+ Taylor Blair - Assistance with designing an implementation of adaptive Huffman encoding.
+ Julian Prince - Tip for Huffman::decode() implementation (using HTree::tree_ptr_t navNode - overheard in lab).
