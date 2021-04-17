# Homework 8

**By:** Cole Nemec & Evan Sieden


Homework 8 time!

### Note:
```Makefile``` will be the makefile for the entirety of this project, ```hw 7_Makefile``` makes everything from the files associated with HW7.

#### On HW7:
The files associated with homework 7 - hforest.hh/.cc, test_hforest.cc, htree.hh/cc, test_htree.cc, and the HW7 makefile - were sourced from our instructor, Eitan Fracthenberg. Important changes were made to these files:
- ```compare_trees()``` was modified to prioritize _smaller_ values rather than larger ones. This was done to fulfill the assignment requirements so that ```HForest::pop_top()``` would return the tree node with the lowest value rather than the highest, as had been the case in HW7.
- ```HTree``` was modified to make its data public, rather than private as it was originally, so that it could be modified after initialization (which is an important part of our Huffman algorithm implementation). Accordingly, some functions were made non-```const```, as well.

#### Design Decisions:

1. **Adaptive Huffman encoding.** The most distinct decision made in this regard is that our implementation of the Huffman encoding/decoding algorithm differs slightly from the [described implementation](https://moodle.reed.edu/mod/assign/view.php?id=157679) in that it utilizes its Huffman tree as a frequency table, rather than maintaining one separately. A few changes to the ```Huffman``` class were necessary to achieve this: adding a ```tree_ptr_t``` (```myTreePtr```), ```HForest``` (```myForest```), and a "helper" ```tree_ptr_t``` (```navNode```) to the private scope of the class, re-designing the processes for encoding and decoding, and adding a couple "helper functions" () to the ```Huffman```'s implementation (```.hh```) file that are specific to this different design (```breakTree()``` and ```buildTree()``` - other helper functions are present, however they aren't unique to this implementation). The structure for our Huffman tree is as such: "organization nodes", indicated by ```key_```s of -1, point to a symbol node in their ```right_```  and a further organization node in their ```left_```. This continues to the final organization node, which has the two lowest-frequency symbols in its ```right_``` and ```left_```. As such, the tree is left-oriented. The re-designed processes for encoding/decoding are described via comments in ```huffman.cc```, but here is a condensed version of them:
    * Encoding:
        1.(If we haven't encoded any characters yet, we initialize a symbol forest of frequencies of 0, and construct a tree based upon it.) 
        2. Find the node of the key in the established ```HTree``` at```myTreePtr```.
        3. Update the value (frequency) of that node in the tree.
        4. "Break" the tree down into a forest that contains each individual symbol node.
        5. "Build" the tree once more.
    * Decoding:
        1. (If we haven't decoded any characters yet, we initialize a symbol forest of frequencies of 0, and construct a tree based upon it.)
        2. (If this is the first bit in a sequence, point our ```navNode``` to the root of the tree.)
        3. Get the child of the current ```navNode``` as indicated by the passed ```bit```.
        4. If we're at an "organization node", return -1 to indicate that the decoded ```bit``` didn't return a symbol.
        5. Otherwise, ```navNode``` is at a node corresponding to a symbol. Increment the frequency of that symbol, set ```navNode``` to indicate that we've completed the current sequence of ```bit```s, break/re-build our tree, and return the symbol we've decoded!
       

2. **Bit I/O.** Most notably, we used ```std::bitset``` in the implementation of the ```BitIO``` class since it made many of the operations on and manipulations of binary data trivial. For instance, rather than shifting bits and using masks and or bitwise AND operations to find out the values of individual bits, ```std::bitset<N>::test()``` can be used to find the value of a bit at a given position. What's more, ```std::bitset```'s requirement of allocating the number of bits upon construction was not a problem since the ```BitIO``` class either inputs or outputs bytes of data at a time. Hence, we can safely declare the buffer like so: ```std::bitset<8> buffer_;```. To facilitate the conversion between data of types ```std::bitset``` and ```char```, we implemented the function ```bitset_to_char()``` which does exactly as the name suggests. Conversely, to convert from ```char``` to ```std::bitset``` we explicitly casted the ```char``` into an ```unsigned long``` and assigned that to ```buffer_```; this conversion is another example of operations that were made much easier due to our choice in using ```std::bitset```. As for ```BitIO```'s other methods that were declared in ```bitio.hh```, their implementation is explained in detail within ```bitio.cc```.

3. **Encoder/decoder.**

4. **Compression tests.**

|       File name      | Raw file size (bytes) | Compressed file size (bytes) | Decompressed matches original |
|:--------------------:|:---------------------:|:----------------------------:|:-----------------------------:|
| ```lorem_ipsum.cc``` |         67,755        |            65,145            |              Yes              |
|    ```file2.cc```    |                       |                              |                               |
|    ```file3.cc```    |                       |                              |                               |
|    ```file4.cc```    |                       |                              |                               |

#### Additional Credits:
+ Taylor Blair - Assistance with designing an implementation of adaptive Huffman encoding.
+ Julian Prince - Tip for Huffman::decode() implementation (using HTree::tree_ptr_t navNode - overheard in lab).
