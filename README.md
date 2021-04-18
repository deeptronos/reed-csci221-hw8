## **Homework 8**
### By Cole Nemec & Evan Sieden

<hr />

### <u>Compilation instructions</u>:
+ To compile, call "make" in the base directory where ```Makefile``` is located.

### <u>On HW7:</u>
The files associated with homework 7 ( ```hforest.hh/.cc```, ```test_hforest.cc```, ```htree.hh/cc```, and ```test_htree.cc```) were sourced from our instructor, Eitan Fracthenberg. The following changes were made as per the instructions of HW8:
- ```compare_trees()``` was modified to prioritize _smaller_ values rather than larger ones. This was done to fulfill the assignment requirements so that ```HForest::pop_top()``` would return the tree node with the lowest value rather than the highest, as had been the case in HW7.
- ```HTree``` was modified to make its data public, rather than private as it was originally, so that it could be modified after initialization (which is an important part of our Huffman algorithm implementation). Accordingly, some functions were made non-```const```, as well.

### <u>Design Decisions:</u>

1. **Adaptive Huffman encoding.** The most distinct decision made in this regard is that our implementation of the Huffman encoding/decoding algorithm differs slightly from the [described implementation](https://moodle.reed.edu/mod/assign/view.php?id=157679) in that it utilizes its Huffman tree as a frequency table, rather than maintaining one separately. A few changes to the ```Huffman``` class were necessary to achieve this: adding a ```tree_ptr_t``` (```myTreePtr```), ```HForest``` (```myForest```), and a "helper" ```tree_ptr_t``` (```navNode```) to the private scope of the class, re-designing the processes for encoding and decoding, and adding a couple "helper functions" () to ```huffman.hh``` that are specific to this different design (namely, ```breakTree()``` and ```buildTree()```—other helper functions are present as well, though they aren't unique to this implementation). The structure for our Huffman tree is as follows: "organization nodes", indicated by ```key_```s of -1, point to a symbol node in their ```right_```  and a further organization node in their ```left_```. This continues to the final organization node, which has the two lowest-frequency symbols in its ```right_``` and ```left_```. As such, the tree is left-oriented. The re-designed processes for encoding/decoding are described via comments in ```huffman.cc```, but here is a condensed version of them:
    * Encoding:
        1. If we haven't encoded any characters yet, we initialize a symbol forest of frequencies of 0, and construct a tree based upon it.
        2. Find the node of the key in the established ```HTree``` at ```myTreePtr```.
        3. Update the value (frequency) of that node in the tree.
        4. "Break" the tree down into a forest that contains each individual symbol node.
        5. "Build" the tree once more.
    * Decoding:
        1. If we haven't decoded any characters yet, we initialize a symbol forest of frequencies of 0, and construct a tree based upon it.
        2. If this is the first bit in a sequence, point our ```navNode``` to the root of the tree.
        3. Get the child of the current ```navNode``` as indicated by the passed ```bit```.
        4. If we're at an "organization node", return -1 to indicate that the decoded ```bit``` didn't return a symbol.
        5. Otherwise, ```navNode``` is at a node corresponding to a symbol. Increment the frequency of that symbol, set ```navNode``` to indicate that we've completed the current sequence of ```bit```s, break/re-build our tree, and return the symbol we've decoded!
       

2. **Bit I/O.** Most notably, we used ```std::bitset``` in the implementation of the ```BitIO``` class since it made many of the operations on and manipulations of binary data trivial. For instance, rather than shifting bits and using masks and or bitwise AND operations to find out the values of individual bits, ```std::bitset<N>::test()``` can be used to find the value of a bit at a given position. What's more, ```std::bitset```'s requirement of allocating the number of bits upon construction was not a problem since the ```BitIO``` class either inputs or outputs bytes of data at a time. Hence, we can safely declare the buffer like so: ```std::bitset<8> buffer_;```. To facilitate the conversion between data of types ```std::bitset``` and ```char```, we implemented the function ```bitset_to_char()``` which does exactly as the name suggests. Conversely, to convert from ```char``` to ```std::bitset``` we explicitly cast the ```char``` into an ```unsigned long``` and assigned that to ```buffer_```; this conversion is another example of operations that were made much easier due to our choice in using ```std::bitset```. As for ```BitIO```'s other methods that were declared in ```bitio.hh```, their implementation is explained in detail within ```bitio.cc```.

3. **Encoder/decoder.** Both ```encoder.cc```  and ```decoder.cc``` begin in very similar ways. In both cases, we check whether there are a correct number of arguments provided and whether the given file to read from actually exists. Further, in creating a new file to write to, we construct the file and set modes ```std::ofstream::out``` and ```std::ofstream::trunc``` so that we can write to the file, and if the file already exists, it is emptied and we can overwrite it without bothering about the previous contents of the file. 
    + In the process of encoding, we take every character in the given input file and encode it with a ```Huffman``` object, outputting the corresponding bit sequence to the output file with a ```BitIO``` object, similarly encoding and outputting ```Huffman::HEOF``` last. We also call the destructor of the ```BitIO``` object to output whatever bits have yet to be outputted. Lastly, we close both files before the program terminates.
        + On a minor note, we also use a helper function called ```output_bits_t()```, which allows us to output a sequence of bits given a reference to a ```BitIO``` object (constructed for output). This provides a degree of abstraction, makes the code (marginally) easier to read, and (most importantly) prevents us from needing to implement its loop twice (first for every character and second for ```Huffman::HEOF```).
    + In the process of decoding, we call ```Huffman::decode()``` until a nonnegative symbol is found. If that symbol is a valid character, we output it to the plaintext file and continue decoding bits. Otherwise, if that symbol is ```Huffman::HEOF```, then we stop the decoding process since we've reached the end of the file. As in ```encoding.cc```, we also close both files before the program terminates.
    + Basic usage:
        + To encode: "./encoder <text_filename>".
        + To decode: "./decoder <encoded_filename>".

4. **Compression tests.**

|       File name      | Raw file size (bytes) | Compressed file size (bytes) | Decompressed matches original |
|:--------------------:|:---------------------:|:----------------------------:|:-----------------------------:|
| ```lorem_ipsum.cc```           |         67,755               |            65,145               |             Yes           |
| ```matrix_97_draft.txt``` | 178,115 | 211,105 | Yes |
| ```h.txt``` | 275,616 | 34,477 | Yes|
| ```d3BFG_Game.h``` | 14,511 | 24,621 | Yes |
| ```vid2vid_README.md``` | 17,535 | 28,667 | Yes |

**Descriptions**
* ```lorem_ipsum.cc``` - A .cc file whose contents were generated at [Lorem Ipsum](https://www.lipsum.com/) with the '\n' characters removed.
* ```matrix_97_draft.txt``` - A text file, a draft of the script for The Matrix (1999). Contains lots of writing, formatted with whitespace to maintain the traditional film script layout. Found on [scifiscripts.com](http://www.scifiscripts.com/scripts/matrix_97_draft.txt)
* ```h.txt``` - An original text file containing 275,616 "h" characters, and absolutely nothing else. 
* ```d3BFG_Game.h``` - ```Game.h```, from the Doom 3 BFG GPL source code release [repository on GitHub](https://github.com/id-Software/DOOM-3-BFG/blob/master/neo/d3xp/Game.h). A C++ header file, containing lots of different symbols in C++ syntax.
* ```vid2vid_README.md``` - The ```README.md``` from NVIDIA's vid2vid [repository on GitHub](https://github.com/NVIDIA/vid2vid). Contains lots of text, along with Markdown-formatting syntax and URLs to image files. 

### <u>Additional Credits:</u>
+ Taylor Blair - Assistance with designing an implementation of adaptive Huffman encoding.
+ Julian Prince - Tip for ```Huffman::decode()``` implementation (```using HTree::tree_ptr_t navNode```—overheard in lab).
+ Eitan Fracthenberg - Decoding loop in ```decoder.cc``` was sourced from the HW8 Moodle page.
